#include <ply/graphics/Renderer.h>

#include <ply/components/Lights.h>
#include <ply/components/Spatial.h>
#include <ply/ecs/World.h>
#include <ply/graphics/RenderDevice.h>
#include <ply/graphics/RenderSystem.h>

#include "RenderImpl.h"
#include <Graphics/GraphicsEngine/interface/RenderPass.h>

using namespace Diligent;

///////////////////////////////////////////////////////////
#define MAX_NUM_SKELETAL_BONES 50

#define CAMERA_BUFFER_SIZE 10

namespace ply {

namespace priv {

    ///////////////////////////////////////////////////////////
    /// \brief Internal G-buffer structure used by the renderer.
    ///
    /// Contains all textures and framebuffers used for deferred rendering.
    ///
    /// \li Color texture: RGBA8_UNORM, RGB = albedo/base color, A = roughness
    /// \li Depth Z texture: R32_FLOAT, Z value of the scene
    /// \li Normal texture: RGBA16_FLOAT, RGB = normal vector, A = emission
    /// strength
    /// \li Material texture: RGBA8_UNORM, R = metallic, G = rim light
    /// intensity, B = subsurface scattering, A = material id
    ///
    ///////////////////////////////////////////////////////////
    struct GBuffer {
        Vector2u m_size; //!< Size of the G-buffer in pixels
        RefCntAutoPtr<IFramebuffer>
            m_framebuffer;         //!< Framebuffer for the G-buffer
        Texture m_colorTexture;    //!< Color texture for the G-buffer
        Texture m_depthZTexture;   //!< Depth texture for the G-buffer
        Texture m_normalTexture;   //!< Normal texture for the G-buffer
        Texture m_materialTexture; //!< Material texture for the G-buffer
        RefCntAutoPtr<ITexture>
            m_depthBuffer; //!< Depth buffer to use for the G-buffer (this can't
        //!< be used as input attachment)
        Texture m_intermediateTexture; //!< Offscreen intermediate texture for
                                       //!< holding render result before copying
                                       //!< to main back buffer

        ResourceBinding m_ambientBinding; //!< Ambient pipeline binding
        ResourceBinding
            m_dirLightBinding; //!< Directional light pipeline binding
        ResourceBinding m_lightVolumeBinding; //!< Light volume pipeline binding
    };

    struct RendererImpl {
        RefCntAutoPtr<IRenderPass> m_renderPass; //!< Render pass object
        HashMap<Framebuffer*, GBuffer>
            m_gBuffers; //!< Map of framebuffer targets to corresponding
                        //!< internal framebuffer
    };

} // namespace priv

///////////////////////////////////////////////////////////
struct CB_Camera {
    Matrix4f m_projView;
    Matrix4f m_invProjView;
    Vector3f m_cameraPos;
    float m_pad1;
    Vector4f m_viewportSize; //!< Viewport size in pixels [w, h, 1 / w, 1 / h]
};

///////////////////////////////////////////////////////////
struct CB_Lights {
    Vector3f m_ambient;
    float m_pad1;
};

///////////////////////////////////////////////////////////
struct CS_Material {
    Vector3f m_albedoColor;
    float m_metallic;

    Vector3f m_emissionColor;
    float m_roughness;

    Vector3f m_rimColor;
    float m_specularIntensity;

    Vector3f m_subsurfaceColor;
    float m_emissionIntensity;

    float m_rimIntensity;
    float m_rimPower;
    float m_subsurface;
    int m_transparent;
};

///////////////////////////////////////////////////////////
struct CS_Shadow {
    Matrix4f m_lightProjView;
    float m_shadowMapSize;
};

///////////////////////////////////////////////////////////
struct CB_Skeleton {
    Matrix4f m_bones[MAX_NUM_SKELETAL_BONES];
};

///////////////////////////////////////////////////////////
struct DirLightAttribs {
    Vector3f m_direction; //!< xyz: direction
    Vector3f m_diffuse;
};

///////////////////////////////////////////////////////////
struct PointLightAttribs {
    Vector4f m_position; //!< xyz: position, w: radius
    Vector3f m_diffuse;
    float m_attenuation; //!< Attenuation factor
};

///////////////////////////////////////////////////////////
Renderer::Renderer() :
    m_impl(std::make_unique<priv::RendererImpl>()),
    m_ambient(0.1f),
    m_numDirLights(0),
    m_numPointLights(0) {}

///////////////////////////////////////////////////////////
Renderer::~Renderer() {}

///////////////////////////////////////////////////////////
void Renderer::createRenderPass(TextureFormat targetFormat) {
    // Get target format
    TEXTURE_FORMAT format;
    if (targetFormat == TextureFormat::Unknown) {
        // Use default framebuffer format
        format = m_device->getImpl()->m_swapChain->GetDesc().ColorBufferFormat;
    } else {
        // Convert polygine texture format to Diligent format
        format = priv::convertTextureFormat(targetFormat);
    }

    // Set up attachments
    constexpr uint32_t NUM_ATTACHMENTS = 6;
    RenderPassAttachmentDesc attachments[NUM_ATTACHMENTS];

    attachments[0].Format = TEX_FORMAT_RGBA8_UNORM;
    attachments[0].InitialState = RESOURCE_STATE_RENDER_TARGET;
    attachments[0].FinalState = RESOURCE_STATE_INPUT_ATTACHMENT;
    attachments[0].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].StoreOp = ATTACHMENT_STORE_OP_DISCARD;

    attachments[1].Format = TEX_FORMAT_R32_FLOAT;
    attachments[1].InitialState = RESOURCE_STATE_RENDER_TARGET;
    attachments[1].FinalState = RESOURCE_STATE_INPUT_ATTACHMENT;
    attachments[1].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].StoreOp = ATTACHMENT_STORE_OP_DISCARD;

    attachments[2].Format = TEX_FORMAT_RGBA16_FLOAT;
    attachments[2].InitialState = RESOURCE_STATE_RENDER_TARGET;
    attachments[2].FinalState = RESOURCE_STATE_INPUT_ATTACHMENT;
    attachments[2].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[2].StoreOp = ATTACHMENT_STORE_OP_DISCARD;

    attachments[3].Format = TEX_FORMAT_RGBA8_UNORM;
    attachments[3].InitialState = RESOURCE_STATE_RENDER_TARGET;
    attachments[3].FinalState = RESOURCE_STATE_INPUT_ATTACHMENT;
    attachments[3].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[3].StoreOp = ATTACHMENT_STORE_OP_DISCARD;

    attachments[4].Format = TEX_FORMAT_D24_UNORM_S8_UINT;
    attachments[4].InitialState = RESOURCE_STATE_DEPTH_WRITE;
    attachments[4].FinalState = RESOURCE_STATE_DEPTH_WRITE;
    attachments[4].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[4].StoreOp = ATTACHMENT_STORE_OP_DISCARD;

    attachments[5].Format = format;
    attachments[5].InitialState = RESOURCE_STATE_RENDER_TARGET;
    attachments[5].FinalState = RESOURCE_STATE_RENDER_TARGET;
    attachments[5].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[5].StoreOp = ATTACHMENT_STORE_OP_STORE;

    // Set up subpasses
    constexpr uint32_t NUM_SUBPASSES = 2;
    SubpassDesc subpasses[NUM_SUBPASSES];

    // First subpass
    AttachmentReference rtAttachmentRefs0[] = {
        {0, RESOURCE_STATE_RENDER_TARGET},
        {1, RESOURCE_STATE_RENDER_TARGET},
        {2, RESOURCE_STATE_RENDER_TARGET},
        {3, RESOURCE_STATE_RENDER_TARGET}
    };

    AttachmentReference depthAttachmentRef0 = {4, RESOURCE_STATE_DEPTH_WRITE};

    subpasses[0].RenderTargetAttachmentCount = _countof(rtAttachmentRefs0);
    subpasses[0].pRenderTargetAttachments = rtAttachmentRefs0;
    subpasses[0].pDepthStencilAttachment = &depthAttachmentRef0;

    // Second subpass
    AttachmentReference rtAttachmentRefs1[] = {{5, RESOURCE_STATE_RENDER_TARGET}
    };

    AttachmentReference depthAttachmentRef1 = {4, RESOURCE_STATE_DEPTH_WRITE};

    AttachmentReference inputAttachmentRefs1[] = {
        {0, RESOURCE_STATE_INPUT_ATTACHMENT},
        {1, RESOURCE_STATE_INPUT_ATTACHMENT},
        {2, RESOURCE_STATE_INPUT_ATTACHMENT},
        {3, RESOURCE_STATE_INPUT_ATTACHMENT}
    };

    subpasses[1].RenderTargetAttachmentCount = _countof(rtAttachmentRefs1);
    subpasses[1].pRenderTargetAttachments = rtAttachmentRefs1;
    subpasses[1].pDepthStencilAttachment = &depthAttachmentRef1;
    subpasses[1].InputAttachmentCount = _countof(inputAttachmentRefs1);
    subpasses[1].pInputAttachments = inputAttachmentRefs1;

    // Dependencies
    SubpassDependencyDesc dependencies[1];
    dependencies[0].SrcSubpass = 0;
    dependencies[0].DstSubpass = 1;
    dependencies[0].SrcStageMask = PIPELINE_STAGE_FLAG_RENDER_TARGET;
    dependencies[0].DstStageMask = PIPELINE_STAGE_FLAG_PIXEL_SHADER;
    dependencies[0].SrcAccessMask = ACCESS_FLAG_RENDER_TARGET_WRITE;
    dependencies[0].DstAccessMask = ACCESS_FLAG_SHADER_READ;

    RenderPassDesc RPDesc;
    RPDesc.Name = "Deferred shading render pass desc";
    RPDesc.AttachmentCount = _countof(attachments);
    RPDesc.pAttachments = attachments;
    RPDesc.SubpassCount = _countof(subpasses);
    RPDesc.pSubpasses = subpasses;
    RPDesc.DependencyCount = _countof(dependencies);
    RPDesc.pDependencies = dependencies;

    // Create render pass
    m_device->getImpl()->m_renderDevice->CreateRenderPass(
        RPDesc,
        &m_impl->m_renderPass
    );

    // Set wrapper
    m_renderPass =
        RenderPass(m_device->getImpl(), m_impl->m_renderPass, Handle());
}

///////////////////////////////////////////////////////////
void Renderer::setUpDirLightsPipeline(const RendererConfig& config) {
    // Shaders
    m_dirLightShaderV =
        m_device->shader()
            .name("Directional light vertex shader")
            .type(Shader::Vertex)
            .file("dir_light.vsh")
            .load();
    m_dirLightShaderP =
        m_device->shader()
            .name("Directional light pixel shader")
            .language(
                m_usingGlsl ? Shader::Language::Glsl : Shader::Language::Hlsl
            )
            .type(Shader::Pixel)
            .file(m_usingGlsl ? "dir_light_glsl.psh" : "dir_light_hlsl.psh")
            .addMacro("MAX_NUM_MATERIALS", config.maxMaterials)
            .load();

    // Pipeline
    m_dirLightPipeline =
        m_device->pipeline()
            .name("Directional light pipeline")
            .renderPass(m_renderPass, 1)
            .shader(&m_dirLightShaderV)
            .shader(&m_dirLightShaderP)
            .topology(PrimitiveTopology::TriangleStrip)
            .cull(CullMode::None)
            .depth(false)
            .blend(true)
            .blendFactors(
                BlendFactor::One,
                BlendFactor::One,
                BlendOperation::Add
            )
            .addInputLayout(0, 0, 3, Type::Float32, true) // Light direction
            .addInputLayout(1, 0, 3, Type::Float32, true) // Light color
            .addVariable(
                "sp_colorTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Color texture
            .addVariable(
                "sp_depthTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Depth texture
            .addVariable(
                "sp_normalTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Normal texture
            .addVariable(
                "sp_materialTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Material texture
            .addVariable(
                "Camera",
                ply::Shader::Pixel,
                ply::ShaderResourceType::Mutable
            ) // Camera
            .addVariable(
                "Shadows",
                ply::Shader::Pixel,
                ply::ShaderResourceType::Mutable
            ) // Shadow
            .addVariable(
                "g_shadowMap",
                ply::Shader::Pixel,
                ply::ShaderResourceType::Mutable
            ) // Shadow map
            .addSampler(
                "g_shadowMap",
                ply::Shader::Pixel,
                TextureFilter::Point
            ) // Shadow map sampler
            .create();

    m_dirLightPipeline
        .setStaticVariable(Shader::Pixel, "Materials", m_materialBuffer);

    // Create instance buffer
    m_dirLightInstance =
        m_device->buffer()
            .name("Directional light instance buffer")
            .access(ResourceAccess::Write)
            .bind(ResourceBind::VertexBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(sizeof(DirLightAttribs) * config.maxDirLights)
            .create();

    // Transition resources
    StateTransitionDesc barriers[] = {
        {BUFFER(m_dirLightInstance.getResource()),
         RESOURCE_STATE_UNKNOWN,
         RESOURCE_STATE_VERTEX_BUFFER,
         STATE_TRANSITION_FLAG_UPDATE_STATE},
    };

    m_device->getImpl()->m_deviceContext->TransitionResourceStates(
        _countof(barriers),
        barriers
    );
}

///////////////////////////////////////////////////////////
void Renderer::setUpLightVolumePipeline(const RendererConfig& config) {
    // Shaders
    m_lightVolumeShaderV =
        m_device->shader()
            .name("Light volume vertex shader")
            .type(Shader::Vertex)
            .file("light_volume.vsh")
            .load();
    m_lightVolumeShaderP =
        m_device->shader()
            .name("Light volume pixel shader")
            .language(
                m_usingGlsl ? Shader::Language::Glsl : Shader::Language::Hlsl
            )
            .type(Shader::Pixel)
            .file(
                m_usingGlsl ? "light_volume_glsl.psh" : "light_volume_hlsl.psh"
            )
            .addMacro("MAX_NUM_MATERIALS", config.maxMaterials)
            .load();

    // Pipeline
    m_lightVolumePipeline =
        m_device->pipeline()
            .name("Light volume pipeline")
            .renderPass(m_renderPass, 1)
            .shader(&m_lightVolumeShaderV)
            .shader(&m_lightVolumeShaderP)
            .topology(PrimitiveTopology::TriangleList)
            .cull(CullMode::Front)
            .depth(false)
            .blend(true)
            .blendFactors(
                BlendFactor::One,
                BlendFactor::One,
                BlendOperation::Add
            )
            .addInputLayout(0, 0, 3, Type::Float32)       // Position
            .addInputLayout(1, 1, 4, Type::Float32, true) // Light position +
                                                          // size
            .addInputLayout(2, 1, 3, Type::Float32, true) // Light color
            .addInputLayout(3, 1, 1, Type::Float32, true) // Light attenuation
            .addVariable(
                "sp_colorTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Color texture
            .addVariable(
                "sp_depthTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Depth texture
            .addVariable(
                "sp_normalTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Normal texture
            .addVariable(
                "sp_materialTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Material texture
            .addVariable(
                "Camera",
                ply::Shader::Vertex,
                ply::ShaderResourceType::Mutable
            ) // Camera
            .addVariable(
                "Camera",
                ply::Shader::Pixel,
                ply::ShaderResourceType::Mutable
            ) // Camera
            .create();

    // Point lights
    createPointLightBuffers(config.maxPointLights);

    m_lightVolumePipeline
        .setStaticVariable(Shader::Pixel, "Materials", m_materialBuffer);

    // Transition resources
    StateTransitionDesc Barriers[] = {
        {BUFFER(m_pointLightVertex.getResource()),
         RESOURCE_STATE_UNKNOWN,
         RESOURCE_STATE_VERTEX_BUFFER,
         STATE_TRANSITION_FLAG_UPDATE_STATE},
        {BUFFER(m_pointLightIndex.getResource()),
         RESOURCE_STATE_UNKNOWN,
         RESOURCE_STATE_INDEX_BUFFER,
         STATE_TRANSITION_FLAG_UPDATE_STATE},
        {BUFFER(m_pointLightInstance.getResource()),
         RESOURCE_STATE_UNKNOWN,
         RESOURCE_STATE_VERTEX_BUFFER,
         STATE_TRANSITION_FLAG_UPDATE_STATE},
    };

    m_device->getImpl()->m_deviceContext->TransitionResourceStates(
        _countof(Barriers),
        Barriers
    );
}

///////////////////////////////////////////////////////////
void Renderer::createPointLightBuffers(uint32_t maxPointLights) {
    // Vertex
    constexpr Vector3f cubeVerts[] = {
        Vector3f{-1, -1, -1},
        Vector3f{-1, +1, -1},
        Vector3f{+1, +1, -1},
        Vector3f{+1, -1, -1},

        Vector3f{-1, -1, +1},
        Vector3f{-1, +1, +1},
        Vector3f{+1, +1, +1},
        Vector3f{+1, -1, +1},
    };

    m_pointLightVertex =
        m_device->buffer()
            .name("Point light vertex buffer")
            .bind(ply::ResourceBind::VertexBuffer)
            .usage(ply::ResourceUsage::Immutable)
            .data(cubeVerts, sizeof(cubeVerts))
            .create();

    // Index
    // clang-format off
    constexpr uint32_t Indices[] = {
        2,0,1, 2,3,0,
        4,6,5, 4,7,6,
        0,7,4, 0,3,7,
        1,0,4, 1,4,5,
        1,5,2, 5,6,2,
        3,6,7, 3,2,6
    };
    // clang-format on

    m_pointLightIndex =
        m_device->buffer()
            .name("Point light index buffer")
            .bind(ply::ResourceBind::IndexBuffer)
            .usage(ply::ResourceUsage::Immutable)
            .data(Indices, sizeof(Indices))
            .create();

    // Create instance buffer
    m_pointLightInstance =
        m_device->buffer()
            .name("Point light instance buffer")
            .access(ResourceAccess::Write)
            .bind(ResourceBind::VertexBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(sizeof(PointLightAttribs) * maxPointLights)
            .create();
}

///////////////////////////////////////////////////////////
void Renderer::setUpShadows() {
    // Create shadow map
    m_shadowMap = m_device->framebuffer();
    m_shadowMap.attachDepth(Vector2u(1024), TextureFormat::D32f);

    // Set up shadow map visualization
    m_shadowVisShaderV =
        m_device->shader()
            .name("Shadow map visualization vertex shader")
            .type(Shader::Vertex)
            .file("shadow_vis.vsh")
            .load();
    m_shadowVisShaderP =
        m_device->shader()
            .name("Shadow map visualization pixel shader")
            .type(Shader::Pixel)
            .file("shadow_vis.psh")
            .load();

    m_shadowVisPipeline =
        m_device->pipeline()
            .name("Shadow map visualization pipeline")
            .shader(&m_shadowVisShaderV)
            .shader(&m_shadowVisShaderP)
            .topology(PrimitiveTopology::TriangleStrip)
            .cull(CullMode::None)
            .depth(false)
            .addVariable(
                "g_shadowMap",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Shadow map
            .addSampler(
                "g_shadowMap",
                Shader::Pixel
            ) // Shadow map sampler
            .create();

    m_shadowVisBinding = m_shadowVisPipeline.createResourceBinding();
    m_shadowVisBinding.setVariable(
        Shader::Pixel,
        "g_shadowMap",
        *m_shadowMap.getDepthTexture()
    );
}

///////////////////////////////////////////////////////////
void Renderer::initialize(RenderDevice* device, const RendererConfig& config) {
    m_device = device;

    // Check if glsl should be used
    const auto& deviceInfo =
        m_device->getImpl()->m_renderDevice->GetDeviceInfo();
    m_usingGlsl = deviceInfo.IsVulkanDevice() || deviceInfo.IsMetalDevice();

    // Create material array
    m_materials = HandleArray<Material>(config.maxMaterials);

    uint32_t align = device->getConstantBufferAlignment();
    m_bufferBlockSizes.camera = (sizeof(CB_Camera) + align - 1) / align * align;

    // Set up render pass
    createRenderPass(config.targetFormat);

    // Allocate camera buffer
    uint32_t size = m_bufferBlockSizes.camera;
    m_cameraBuffer =
        device->buffer()
            .name("Camera buffer")
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * config.buffer.cameraBufferSize)
            .create();

    // Allocate lights buffer
    size = (sizeof(CB_Lights) + align - 1) / align * align;
    m_lightsBuffer =
        device->buffer()
            .name("Lights buffer")
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size)
            .create();

    // Allocate materials buffer
    size =
        (config.maxMaterials * sizeof(CS_Material) + align - 1) / align * align;
    m_materialBuffer =
        device->buffer()
            .name("Materials buffer")
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size)
            .create();

    // Allocate animation buffer
    size = (sizeof(CB_Skeleton) + align - 1) / align * align;
    m_animationBuffer =
        device->buffer()
            .name("Animation buffer")
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * config.buffer.animBufferSize)
            .create();

    // Allocate shadow buffer
    size = (sizeof(CS_Shadow) + align - 1) / align * align;
    m_shadowBuffer =
        device->buffer()
            .name("Shadow buffer")
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size)
            .create();

    // Deferred lighting pipeline
    m_quadShader =
        device->shader()
            .name("Quad shader")
            .type(Shader::Vertex)
            .file("quad.vsh")
            .load();
    m_ambientShader =
        device->shader()
            .name("Ambient light shader")
            .language(
                m_usingGlsl ? Shader::Language::Glsl : Shader::Language::Hlsl
            )
            .type(Shader::Pixel)
            .file(m_usingGlsl ? "ambient_glsl.psh" : "ambient_hlsl.psh")
            .addMacro("MAX_NUM_MATERIALS", config.maxMaterials)
            .load();

    m_ambientPipeline =
        device->pipeline()
            .name("Deferred ambient pipeline")
            .renderPass(m_renderPass, 1)
            .shader(&m_quadShader)
            .shader(&m_ambientShader)
            .addVariable(
                "sp_colorTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Color texture
            .addVariable(
                "sp_depthTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Depth texture
            .addVariable(
                "sp_normalTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Normal texture
            .addVariable(
                "sp_materialTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Material texture
            .addVariable(
                "Camera",
                ply::Shader::Pixel,
                ply::ShaderResourceType::Mutable
            ) // Camera
            .topology(PrimitiveTopology::TriangleStrip)
            .cull(CullMode::None)
            .depth(false)
            .create();

    // Set variables
    m_ambientPipeline
        .setStaticVariable(Shader::Pixel, "Lights", m_lightsBuffer);
    m_ambientPipeline
        .setStaticVariable(Shader::Pixel, "Materials", m_materialBuffer);

    // Set up shadows
    setUpShadows();

    // Directional light pipeline
    setUpDirLightsPipeline(config);

    // Light volume pipeline
    setUpLightVolumePipeline(config);
}

///////////////////////////////////////////////////////////
void Renderer::setWorld(World* world) {
    if (!world)
        return;

    m_world = world;

    // Directional light query
    m_queryDirLights = m_world->query().match<DirectionalLight>().compile();
    // Point light query
    m_queryPointLights =
        m_world->query().match<Transform, PointLight>().compile();
}

///////////////////////////////////////////////////////////
void Renderer::add(RenderSystem* system) {
    m_systems.push_back(system);
    system->m_device = m_device;
    system->m_renderer = this;

    ContextConstantBuffers buffers({m_cameraBuffer, m_lightsBuffer});
    RenderSystem::Init
        data{m_device, buffers, m_bufferBlockSizes, m_renderPass, m_shadowMap};
    system->initialize(data);
}

///////////////////////////////////////////////////////////
Material* Renderer::material() {
    Handle handle = m_materials.push({});
    return &m_materials[handle];
}

///////////////////////////////////////////////////////////
Handle Renderer::registerMaterial(const Material& material) {
    return m_materials.push(material);
}

///////////////////////////////////////////////////////////
void Renderer::removeMaterial(Handle handle) {
    m_materials.remove(handle);
}

///////////////////////////////////////////////////////////
void Renderer::setMaterial(Handle handle, const Material& material) {
    m_materials[handle] = material;
}

///////////////////////////////////////////////////////////
Material& Renderer::getMaterial(Handle handle) {
    return m_materials[handle];
}

///////////////////////////////////////////////////////////
void Renderer::update(float dt) {
    // Discard previous
    m_cameraBuffer.discard();
    m_lightsBuffer.discard();
    m_materialBuffer.discard();
    m_shadowBuffer.discard();

    // Update all render systems
    for (RenderSystem* system : m_systems) {
        system->update(dt);
    }

    // Update light buffer
    {
        CB_Lights lightsBlock;
        lightsBlock.m_ambient = Vector3f(0.1f);

        m_lightsBuffer.push(
            lightsBlock,
            m_device->getConstantBufferAlignment()
        );
    }

    // Update material buffer
    {
        CS_Material* materialBlock = (CS_Material*)m_materialBuffer.map(
            MapMode::Write,
            MapFlag::Discard
        );

        // Default material
        materialBlock[0].m_albedoColor = Vector3f(1.0f);
        materialBlock[0].m_metallic = 0.0f;
        materialBlock[0].m_emissionColor = Vector3f(1.0f);
        materialBlock[0].m_roughness = 1.0f;
        materialBlock[0].m_rimColor = Vector3f(1.0f);
        materialBlock[0].m_specularIntensity = 1.0f;
        materialBlock[0].m_subsurfaceColor = Vector3f(1.0f, 0.2f, 0.2f);
        materialBlock[0].m_emissionIntensity = 1.0f;
        materialBlock[0].m_rimIntensity = 1.0f;
        materialBlock[0].m_rimPower = 3.0f;
        materialBlock[0].m_subsurface = 0.0f;
        materialBlock[0].m_transparent = 0;

        // Push materials
        const auto& materialArray = m_materials.data();
        for (uint32_t i = 0; i < materialArray.size(); ++i) {
            const auto& material = materialArray[i];

            CS_Material& mat = materialBlock[i + 1];
            mat.m_albedoColor = material.albedoColor;
            mat.m_metallic = material.metallic;
            mat.m_emissionColor = material.emissionColor;
            mat.m_roughness = material.roughness;
            mat.m_rimColor = material.rimColor;
            mat.m_specularIntensity = material.specularIntensity;
            mat.m_subsurfaceColor = material.subsurfaceColor;
            mat.m_emissionIntensity = material.emissionIntensity;
            mat.m_rimIntensity = material.rimIntensity;
            mat.m_rimPower = material.rimPower;
            mat.m_subsurface = material.subsurface;
            mat.m_transparent = material.transparent ? 1 : 0;
        }

        m_materialBuffer.unmap();
    }

    // World updates
    if (m_world) {
        updateDirLights();
        updatePointLights();
    }
}

///////////////////////////////////////////////////////////
void Renderer::updateDirLights() {
    // Update directional lights in the world
    DirLightAttribs* dirLights = (DirLightAttribs*)m_dirLightInstance.map(
        MapMode::Write,
        MapFlag::Discard
    );

    m_numDirLights = 0;
    m_queryDirLights.each(
        [&, this, dirLights](Transform& t, DirectionalLight& light) {
            // Update directional light attributes
            DirLightAttribs attribs;
            attribs.m_direction = normalize(light.direction);
            attribs.m_diffuse = light.color;

            // Write to instance buffer
            dirLights[m_numDirLights++] = attribs;
        }
    );

    m_dirLightInstance.unmap();
}

///////////////////////////////////////////////////////////
void Renderer::updatePointLights() {
    // Update point lights in the world
    PointLightAttribs* pointLights =
        (PointLightAttribs*)
            m_pointLightInstance.map(MapMode::Write, MapFlag::Discard);

    m_numPointLights = 0;
    m_queryPointLights.each(
        [&, this, pointLights](Transform& t, PointLight& light) {
            // Update point light position
            PointLightAttribs attribs;
            attribs.m_position =
                Vector4f(t.position, std::max(light.range, 0.0f));
            attribs.m_diffuse = light.color;
            attribs.m_attenuation = light.attenuation;

            // Write to instance buffer
            pointLights[m_numPointLights++] = attribs;
        }
    );

    m_pointLightInstance.unmap();
}

///////////////////////////////////////////////////////////
void Renderer::render(Camera& camera, Framebuffer& target) {
    // TEMP : Shadow camera
    Camera shadowCamera;
    shadowCamera.setPosition(Vector3f{-1.0f, 10.0f, 0.0f});
    shadowCamera.setDirection(Vector3f{0.1f, -1.0f, 0.0f});
    shadowCamera.setOrthographic(-1.5f, 1.5f, -1.5f, 1.5f, 0.1f, 100.0f);

    CS_Shadow shadowBlock;
    shadowBlock.m_lightProjView =
        shadowCamera.getProjMatrix() * shadowCamera.getViewMatrix();
    shadowBlock.m_shadowMapSize = m_shadowMap.getSize().x;
    if (m_usingGlsl) {
        shadowBlock.m_lightProjView = transpose(shadowBlock.m_lightProjView);
    }
    m_shadowBuffer.push(shadowBlock, m_device->getConstantBufferAlignment());

    // Render shadow pass
    doRenderPass(shadowCamera, m_shadowMap, RenderPass::Shadow);

    // Render standard pass
    doRenderPass(camera, target, RenderPass::Default);

    auto& context = m_device->context;
    context.setRenderTarget(target);
    context.setPipeline(m_shadowVisPipeline);
    context.setResourceBinding(m_shadowVisBinding);
    context.draw(4);
}

///////////////////////////////////////////////////////////
void Renderer::doRenderPass(
    Camera& camera,
    Framebuffer& target,
    RenderPass::Type pass
) {
    auto* device = m_device->getImpl();
    auto deviceContext = device->m_deviceContext;

    // Create render context
    ContextBufferOffsets offsets({m_cameraBuffer.offset()});
    RenderPassContext context(camera, pass, offsets);
    context.isDeferredPass = true;

    // Update camera buffer
    Matrix4f projView = camera.getProjMatrix() * camera.getViewMatrix();

    CB_Camera cameraBlock;
    cameraBlock.m_projView = projView;
    cameraBlock.m_invProjView = inverse(projView);
    cameraBlock.m_cameraPos = camera.getPosition();
    if (m_usingGlsl) {
        cameraBlock.m_invProjView = transpose(cameraBlock.m_invProjView);
    }

    // Get viewport size
    if (&target == &Framebuffer::Default) {
        // Use swap chain size for viewport size
        const auto& swapChainDesc = device->m_swapChain->GetDesc();
        cameraBlock.m_viewportSize = Vector4f(
            static_cast<float>(swapChainDesc.Width),
            static_cast<float>(swapChainDesc.Height),
            1.0f / static_cast<float>(swapChainDesc.Width),
            1.0f / static_cast<float>(swapChainDesc.Height)
        );
    } else {
        const auto& viewportSize = target.getSize();
        cameraBlock.m_viewportSize = Vector4f(
            static_cast<float>(viewportSize.x),
            static_cast<float>(viewportSize.y),
            1.0f / static_cast<float>(viewportSize.x),
            1.0f / static_cast<float>(viewportSize.y)
        );
    }

    // Push data
    context.offsets.camera = m_cameraBuffer.push(
        cameraBlock,
        m_device->getConstantBufferAlignment()
    );

    // Shadow pass only renders once
    if (pass == RenderPass::Shadow) {
        // TEMP : Transition shadow map
        StateTransitionDesc transitions[1];
        transitions[0].pResource =
            TEXTURE(m_shadowMap.getDepthTexture()->getResource());
        transitions[0].OldState = RESOURCE_STATE_UNKNOWN;
        transitions[0].NewState = RESOURCE_STATE_DEPTH_WRITE;
        transitions[0].Flags = STATE_TRANSITION_FLAG_UPDATE_STATE;
        deviceContext->TransitionResourceStates(1, &transitions[0]);

        m_device->context.setRenderTarget(target);
        m_device->context.clear(ClearFlags::Depth);

        // Invoke all render systems
        for (RenderSystem* system : m_systems) {
            system->render(context);
        }

        return;
    }

    // Transition resource states
    for (RenderSystem* system : m_systems) {
        const auto& transitions = system->m_impl->m_transitions;
        if (transitions.size() > 0) {
            deviceContext->TransitionResourceStates(
                transitions.size(),
                &transitions[0]
            );
        }
    }

    // TEMP : Transition shadow map
    StateTransitionDesc transitions[1];
    transitions[0].pResource =
        TEXTURE(m_shadowMap.getDepthTexture()->getResource());
    transitions[0].OldState = RESOURCE_STATE_UNKNOWN;
    transitions[0].NewState = RESOURCE_STATE_DEPTH_READ;
    transitions[0].Flags = STATE_TRANSITION_FLAG_UPDATE_STATE;
    deviceContext->TransitionResourceStates(1, &transitions[0]);

    // Start render pass
    auto& gbuffer = getGBuffer(target);
    startRenderPass(gbuffer);
    m_device->context.setRenderPassMode(true);

    // Invoke all render systems
    for (RenderSystem* system : m_systems) {
        system->render(context);
    }

    deviceContext->NextSubpass();

    // Do lighting stage
    applyLighting(gbuffer, context);

    deviceContext->EndRenderPass();
    m_device->context.setRenderPassMode(false);

    // Copy intermediate texture to back buffer if needed
    if (&target == &Framebuffer::Default) {
        auto* gbuffer = &getGBuffer(target);
        auto* intermediateTex =
            TEXTURE(gbuffer->m_intermediateTexture.getResource());
        if (intermediateTex) {
            CopyTextureAttribs copyAttribs;
            copyAttribs.pSrcTexture = intermediateTex;
            copyAttribs.pDstTexture =
                device->m_swapChain->GetCurrentBackBufferRTV()->GetTexture();
            copyAttribs.SrcTextureTransitionMode =
                RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
            copyAttribs.DstTextureTransitionMode =
                RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
            deviceContext->CopyTexture(copyAttribs);
        }
    }
}

///////////////////////////////////////////////////////////
ResourceBinding createDeferredResourceBinding(
    Pipeline& pipeline,
    const priv::GBuffer& gbuffer
) {
    ResourceBinding binding = pipeline.createResourceBinding();
    binding
        .setVariable(Shader::Pixel, "sp_colorTexture", gbuffer.m_colorTexture);
    binding
        .setVariable(Shader::Pixel, "sp_depthTexture", gbuffer.m_depthZTexture);
    binding.setVariable(
        Shader::Pixel,
        "sp_normalTexture",
        gbuffer.m_normalTexture
    );
    binding.setVariable(
        Shader::Pixel,
        "sp_materialTexture",
        gbuffer.m_materialTexture
    );
    return binding;
}

///////////////////////////////////////////////////////////
priv::GBuffer& Renderer::getGBuffer(Framebuffer& target) {
    auto* device = m_device->getImpl();

    // Get current target size
    Vector2u currentSize = target.getSize();
    if (&target == &Framebuffer::Default) {
        // Use swap chain size for default framebuffer
        const auto& scDesc = device->m_swapChain->GetDesc();
        currentSize.x = scDesc.Width;
        currentSize.y = scDesc.Height;
    }

    // Check if we have a G-buffer for this target
    auto it = m_impl->m_gBuffers.find(&target);

    // Create gbuffer if not found or size changed
    if (it == m_impl->m_gBuffers.end() || currentSize != it->second.m_size) {
        // Create new G-buffer
        priv::GBuffer gbuffer;

        auto* rtv = device->m_swapChain->GetCurrentBackBufferRTV();
        const auto& rpDesc = m_impl->m_renderPass->GetDesc();
        const auto& scDesc = device->m_swapChain->GetDesc();

#if PLATFORM_MACOS || PLATFORM_IOS || PLATFORM_TVOS
        // In Metal and Vulkan on top of Metal, there are no native subpasses,
        // and attachments can't be preserved between subpasses without saving
        // them to global memory. Thus they can't be memoryless in this usage
        // scenario.
        constexpr auto MemorylessTexBindFlags = BIND_NONE;
#else
        const auto MemorylessTexBindFlags =
            device->m_renderDevice->GetAdapterInfo()
                .Memory.MemorylessTextureBindFlags;
#endif

        // Create window-size offscreen render target
        TextureDesc texDesc;
        texDesc.Name = "Color G-buffer";
        texDesc.Type = RESOURCE_DIM_TEX_2D;
        texDesc.BindFlags = BIND_RENDER_TARGET | BIND_INPUT_ATTACHMENT;
        texDesc.Format = rpDesc.pAttachments[0].Format;
        texDesc.Width = scDesc.Width;
        texDesc.Height = scDesc.Height;
        texDesc.MipLevels = 1;
        texDesc.MiscFlags =
            ((MemorylessTexBindFlags & texDesc.BindFlags) == texDesc.BindFlags)
                ? MISC_TEXTURE_FLAG_MEMORYLESS
                : MISC_TEXTURE_FLAG_NONE;

        // Define optimal clear value
        texDesc.ClearValue.Format = texDesc.Format;
        texDesc.ClearValue.Color[0] = 0.f;
        texDesc.ClearValue.Color[1] = 0.f;
        texDesc.ClearValue.Color[2] = 0.f;
        texDesc.ClearValue.Color[3] = 1.f;

        RefCntAutoPtr<ITexture> colorTexture;
        device->m_renderDevice->CreateTexture(texDesc, nullptr, &colorTexture);
        Handle colorHandle = device->m_textures.push(colorTexture);
        gbuffer.m_colorTexture = Texture(device, colorTexture, colorHandle);

        // Create intermediate texture if rendering to main back buffer (back
        // buffer texture switches every frame, it's easier to render to
        // intermediate buffer then copy to current back buffer)
        ITexture* targetTexture = nullptr;
        if (&target == &Framebuffer::Default) {
            texDesc.Name = "G-buffer intermediate texture";
            texDesc.Format = device->m_swapChain->GetDesc().ColorBufferFormat;
            texDesc.BindFlags = BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
            texDesc.Usage = USAGE_DEFAULT;

            RefCntAutoPtr<ITexture> intermediateTexture;
            device->m_renderDevice
                ->CreateTexture(texDesc, nullptr, &intermediateTexture);

            Handle intermediateHandle =
                device->m_textures.push(intermediateTexture);
            gbuffer.m_intermediateTexture =
                Texture(device, intermediateTexture, intermediateHandle);

            targetTexture = intermediateTexture;
        }

        // Use first color attachment
        else if (target.getNumColorTextures() > 0) {
            targetTexture = TEXTURE(target.getColorTexture(0)->getResource());
        }

        // Depth Z
        texDesc.Name = "Depth Z G-buffer";
        texDesc.Format = rpDesc.pAttachments[1].Format;
        texDesc.BindFlags = BIND_RENDER_TARGET | BIND_INPUT_ATTACHMENT;
        texDesc.MiscFlags =
            ((MemorylessTexBindFlags & texDesc.BindFlags) == texDesc.BindFlags)
                ? MISC_TEXTURE_FLAG_MEMORYLESS
                : MISC_TEXTURE_FLAG_NONE;

        texDesc.ClearValue.Format = texDesc.Format;
        texDesc.ClearValue.Color[0] = 1.f;
        texDesc.ClearValue.Color[1] = 1.f;
        texDesc.ClearValue.Color[2] = 1.f;
        texDesc.ClearValue.Color[3] = 1.f;

        RefCntAutoPtr<ITexture> depthZTexture;
        device->m_renderDevice->CreateTexture(texDesc, nullptr, &depthZTexture);
        Handle depthZHandle = device->m_textures.push(depthZTexture);
        gbuffer.m_depthZTexture = Texture(device, depthZTexture, depthZHandle);

        // Normal texture
        texDesc.Name = "Normal G-buffer";
        texDesc.Format = rpDesc.pAttachments[2].Format;
        texDesc.MiscFlags =
            ((MemorylessTexBindFlags & texDesc.BindFlags) == texDesc.BindFlags)
                ? MISC_TEXTURE_FLAG_MEMORYLESS
                : MISC_TEXTURE_FLAG_NONE;

        texDesc.ClearValue.Format = texDesc.Format;
        texDesc.ClearValue.Color[0] = 0.f;
        texDesc.ClearValue.Color[1] = 0.f;
        texDesc.ClearValue.Color[2] = 0.f;
        texDesc.ClearValue.Color[3] = 1.f;

        RefCntAutoPtr<ITexture> normalTexture;
        device->m_renderDevice->CreateTexture(texDesc, nullptr, &normalTexture);
        Handle normalHandle = device->m_textures.push(normalTexture);
        gbuffer.m_normalTexture = Texture(device, normalTexture, normalHandle);

        // Material texture
        texDesc.Name = "Material G-buffer";
        texDesc.Format = rpDesc.pAttachments[3].Format;
        texDesc.MiscFlags =
            ((MemorylessTexBindFlags & texDesc.BindFlags) == texDesc.BindFlags)
                ? MISC_TEXTURE_FLAG_MEMORYLESS
                : MISC_TEXTURE_FLAG_NONE;

        texDesc.ClearValue.Format = texDesc.Format;

        RefCntAutoPtr<ITexture> materialTexture;
        device->m_renderDevice
            ->CreateTexture(texDesc, nullptr, &materialTexture);
        Handle materialHandle = device->m_textures.push(materialTexture);
        gbuffer.m_materialTexture =
            Texture(device, materialTexture, materialHandle);

        // Depth buffer
        texDesc.Name = "Depth buffer";
        texDesc.Format = rpDesc.pAttachments[4].Format;
        texDesc.BindFlags = BIND_DEPTH_STENCIL;
        texDesc.MiscFlags =
            ((MemorylessTexBindFlags & texDesc.BindFlags) == texDesc.BindFlags)
                ? MISC_TEXTURE_FLAG_MEMORYLESS
                : MISC_TEXTURE_FLAG_NONE;

        texDesc.ClearValue.Format = texDesc.Format;
        texDesc.ClearValue.DepthStencil.Depth = 1.f;
        texDesc.ClearValue.DepthStencil.Stencil = 0;

        device->m_renderDevice
            ->CreateTexture(texDesc, nullptr, &gbuffer.m_depthBuffer);

        // Create framebuffer for G-buffer
        ITextureView* pAttachments[] = //
            {colorTexture->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET),
             depthZTexture->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET),
             normalTexture->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET),
             materialTexture->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET),
             gbuffer.m_depthBuffer->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL),
             targetTexture
                 ? targetTexture->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET)
                 : nullptr};

        FramebufferDesc FBDesc;
        FBDesc.Name = "G-buffer framebuffer";
        FBDesc.pRenderPass = m_impl->m_renderPass;
        FBDesc.AttachmentCount = _countof(pAttachments);
        FBDesc.ppAttachments = pAttachments;

        device->m_renderDevice->CreateFramebuffer(
            FBDesc,
            &gbuffer.m_framebuffer
        );

        // Set size
        gbuffer.m_size = Vector2u(scDesc.Width, scDesc.Height);

        // Create resource bindings
        gbuffer.m_ambientBinding =
            createDeferredResourceBinding(m_ambientPipeline, gbuffer);
        gbuffer.m_dirLightBinding =
            createDeferredResourceBinding(m_dirLightPipeline, gbuffer);
        gbuffer.m_lightVolumeBinding =
            createDeferredResourceBinding(m_lightVolumePipeline, gbuffer);

        // Set variables
        gbuffer.m_ambientBinding.setVariable(
            Shader::Pixel,
            "Camera",
            m_cameraBuffer,
            0,
            sizeof(CB_Camera)
        );
        gbuffer.m_dirLightBinding.setVariable(
            Shader::Pixel,
            "Camera",
            m_cameraBuffer,
            0,
            sizeof(CB_Camera)
        );
        gbuffer.m_dirLightBinding.setVariable(
            Shader::Pixel,
            "Shadows",
            m_shadowBuffer,
            0,
            sizeof(CS_Shadow)
        );
        gbuffer.m_dirLightBinding.setVariable(
            Shader::Pixel,
            "g_shadowMap",
            *m_shadowMap.getDepthTexture()
        );
        gbuffer.m_lightVolumeBinding.setVariable(
            Shader::Vertex,
            "Camera",
            m_cameraBuffer,
            0,
            sizeof(CB_Camera)
        );
        gbuffer.m_lightVolumeBinding.setVariable(
            Shader::Pixel,
            "Camera",
            m_cameraBuffer,
            0,
            sizeof(CB_Camera)
        );

        // Add to map
        it = m_impl->m_gBuffers.insert_or_assign(&target, std::move(gbuffer))
                 .first;
    }

    return it.value();
}

///////////////////////////////////////////////////////////
void Renderer::startRenderPass(const priv::GBuffer& gbuffer) {
    auto* device = m_device->getImpl();

    BeginRenderPassAttribs rpBeginInfo;
    rpBeginInfo.pRenderPass = m_impl->m_renderPass;
    rpBeginInfo.pFramebuffer = gbuffer.m_framebuffer;

    OptimizedClearValue clearValues[6];
    // Color
    clearValues[0].Color[0] = 0.f;
    clearValues[0].Color[1] = 0.f;
    clearValues[0].Color[2] = 0.f;
    clearValues[0].Color[3] = 1.f;

    // Depth Z
    clearValues[1].Color[0] = 1.f;
    clearValues[1].Color[1] = 1.f;
    clearValues[1].Color[2] = 1.f;
    clearValues[1].Color[3] = 1.f;

    // Normal
    clearValues[2].Color[0] = 0.f;
    clearValues[2].Color[1] = 0.f;
    clearValues[2].Color[2] = 0.f;
    clearValues[2].Color[3] = 0.f;

    // Material
    clearValues[3].Color[0] = 0.f;
    clearValues[3].Color[1] = 0.f;
    clearValues[3].Color[2] = 0.f;
    clearValues[3].Color[3] = 0.f;

    // Depth buffer
    clearValues[4].DepthStencil.Depth = 1.f;

    // Final color buffer
    clearValues[5].Color[0] = 0.0f;
    clearValues[5].Color[1] = 0.0f;
    clearValues[5].Color[2] = 0.0f;
    clearValues[5].Color[3] = 1.f;

    rpBeginInfo.pClearValues = clearValues;
    rpBeginInfo.ClearValueCount = _countof(clearValues);
    rpBeginInfo.StateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
    device->m_deviceContext->BeginRenderPass(rpBeginInfo);
}

///////////////////////////////////////////////////////////
void Renderer::applyLighting(
    priv::GBuffer& gbuffer,
    RenderPassContext& context
) {
    auto& deviceContext = m_device->context;

    // Ambient shading pass
    gbuffer.m_ambientBinding
        .setDynamicOffset(Shader::Pixel, "Camera", context.offsets.camera);

    deviceContext.setPipeline(m_ambientPipeline);
    deviceContext.setResourceBinding(gbuffer.m_ambientBinding);
    deviceContext.draw(4);

    // Directional lights
    if (m_numDirLights > 0) {
        gbuffer.m_dirLightBinding
            .setDynamicOffset(Shader::Pixel, "Camera", context.offsets.camera);

        deviceContext.setPipeline(m_dirLightPipeline);
        deviceContext.setResourceBinding(gbuffer.m_dirLightBinding);
        deviceContext.setVertexBuffers({&m_dirLightInstance});
        deviceContext.draw(4, m_numDirLights);
    }

    // Point lights
    if (m_numPointLights > 0) {
        gbuffer.m_lightVolumeBinding
            .setDynamicOffset(Shader::Vertex, "Camera", context.offsets.camera);
        gbuffer.m_lightVolumeBinding
            .setDynamicOffset(Shader::Pixel, "Camera", context.offsets.camera);

        deviceContext.setPipeline(m_lightVolumePipeline);
        deviceContext.setResourceBinding(gbuffer.m_lightVolumeBinding);
        deviceContext.setVertexBuffers(
            {&m_pointLightVertex, &m_pointLightInstance}
        );
        deviceContext.setIndexBuffer(m_pointLightIndex);
        deviceContext.drawIndexed(36, m_numPointLights);
    }
}

} // namespace ply