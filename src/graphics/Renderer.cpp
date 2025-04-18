#include <ply/graphics/Renderer.h>

#include <ply/graphics/RenderDevice.h>
#include <ply/graphics/RenderSystem.h>

#include "RenderImpl.h"
#include <Graphics/GraphicsEngine/interface/RenderPass.h>

using namespace Diligent;

///////////////////////////////////////////////////////////
#define MAX_NUM_DIR_LIGHTS 2
#define MAX_NUM_SKELETAL_BONES 50

namespace ply {

namespace priv {

    struct GBuffer {
        RefCntAutoPtr<IFramebuffer>
            m_framebuffer;       //!< Framebuffer for the G-buffer
        Texture m_colorTexture;  //!< Color texture for the G-buffer
        Texture m_depthZTexture; //!< Depth texture for the G-buffer
        RefCntAutoPtr<ITexture>
            m_depthBuffer; //!< Depth buffer to use for the G-buffer (this can't
        //!< be used as input attachment)
        Texture m_intermediateTexture; //!< Offscreen intermediate texture for
                                       //!< holding render result before copying
                                       //!< to main back buffer

        ResourceBinding m_deferredBinding;    //!< Deferred pipeline binding
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
    Vector3f m_cameraPos;
    float m_pad;
};

///////////////////////////////////////////////////////////
struct CS_DirLight {
    Vector3f m_diffuse;
    float m_pad1;
    Vector3f m_specular;
    float m_pad2;
    Vector3f m_direction;
    int m_hasShadows;
};

///////////////////////////////////////////////////////////
struct CB_Lights {
    Vector3f m_ambient;
    float m_pad1;
    CS_DirLight m_dirLights[MAX_NUM_DIR_LIGHTS];
    // UniformStruct_PointLight		m_pointLights[MAX_NUM_POINT_LIGHTS];
    int m_numDirLights;
    float m_pad2[3];
};

///////////////////////////////////////////////////////////
struct CB_Skeleton {
    Matrix4f m_bones[MAX_NUM_SKELETAL_BONES];
};

///////////////////////////////////////////////////////////
Renderer::Renderer() :
    m_impl(std::make_unique<priv::RendererImpl>()),
    m_ambient(0.1f) {}

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
    constexpr uint32_t NUM_ATTACHMENTS = 4;
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

    attachments[2].Format = TEX_FORMAT_D24_UNORM_S8_UINT;
    attachments[2].InitialState = RESOURCE_STATE_DEPTH_WRITE;
    attachments[2].FinalState = RESOURCE_STATE_DEPTH_WRITE;
    attachments[2].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[2].StoreOp = ATTACHMENT_STORE_OP_DISCARD;

    attachments[3].Format = format;
    attachments[3].InitialState = RESOURCE_STATE_RENDER_TARGET;
    attachments[3].FinalState = RESOURCE_STATE_RENDER_TARGET;
    attachments[3].LoadOp = ATTACHMENT_LOAD_OP_CLEAR;
    attachments[3].StoreOp = ATTACHMENT_STORE_OP_STORE;

    // Set up subpasses
    constexpr uint32_t NUM_SUBPASSES = 2;
    SubpassDesc subpasses[NUM_SUBPASSES];

    // First subpass
    AttachmentReference rtAttachmentRefs0[] = {
        {0, RESOURCE_STATE_RENDER_TARGET},
        {1, RESOURCE_STATE_RENDER_TARGET}
    };

    AttachmentReference depthAttachmentRef0 = {2, RESOURCE_STATE_DEPTH_WRITE};

    subpasses[0].RenderTargetAttachmentCount = _countof(rtAttachmentRefs0);
    subpasses[0].pRenderTargetAttachments = rtAttachmentRefs0;
    subpasses[0].pDepthStencilAttachment = &depthAttachmentRef0;

    // Second subpass
    AttachmentReference rtAttachmentRefs1[] = {{3, RESOURCE_STATE_RENDER_TARGET}
    };

    AttachmentReference depthAttachmentRef1 = {2, RESOURCE_STATE_DEPTH_WRITE};

    AttachmentReference inputAttachmentRefs1[] = {
        {0, RESOURCE_STATE_INPUT_ATTACHMENT},
        {1, RESOURCE_STATE_INPUT_ATTACHMENT}
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
void Renderer::initialize(RenderDevice* device, const RendererConfig& config) {
    m_device = device;

    uint32_t align = device->getConstantBufferAlignment();

    // Set up render pass
    createRenderPass(config.targetFormat);

    // Allocate camera buffer
    uint32_t size = (sizeof(CB_Camera) + align - 1) / align * align;
    m_cameraBuffer =
        device->buffer()
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * config.buffer.cameraBufferSize)
            .create();

    // Allocate lights buffer
    size = (sizeof(CB_Lights) + align - 1) / align * align;
    m_lightsBuffer =
        device->buffer()
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * config.buffer.lightBufferSize)
            .create();

    // Allocate animation buffer
    size = (sizeof(CB_Skeleton) + align - 1) / align * align;
    m_animationBuffer =
        device->buffer()
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * config.buffer.animBufferSize)
            .create();

    // Check if glsl should be used
    const auto& deviceInfo = device->getImpl()->m_renderDevice->GetDeviceInfo();
    const auto useGlsl =
        deviceInfo.IsVulkanDevice() || deviceInfo.IsMetalDevice();

    // Deferred lighting pipeline
    m_quadShader =
        device->shader().type(Shader::Vertex).file("quad.vsh").load();
    m_deferredShader =
        device->shader()
            .language(useGlsl ? Shader::Language::Glsl : Shader::Language::Hlsl)
            .type(Shader::Pixel)
            .file(useGlsl ? "deferred_glsl.psh" : "deferred_hlsl.psh")
            .load();

    m_deferredPipeline =
        device->pipeline()
            .renderPass(m_renderPass, 1)
            .shader(&m_quadShader)
            .shader(&m_deferredShader)
            .addVariable(
                "g_colorTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Color texture
            .addVariable(
                "g_depthTexture",
                Shader::Pixel,
                ShaderResourceType::Mutable
            ) // Depth texture
            .topology(PrimitiveTopology::TriangleStrip)
            .cull(CullMode::None)
            .depth(false)
            .create();
}

///////////////////////////////////////////////////////////
void Renderer::add(RenderSystem* system) {
    m_systems.push_back(system);
    system->m_device = m_device;

    ContextUniformBuffers buffers({m_cameraBuffer, m_lightsBuffer});
    RenderSystem::Init data{m_device, buffers, m_renderPass};
    system->initialize(data);
}

///////////////////////////////////////////////////////////
void Renderer::update(float dt) {
    // Update all render systems
    for (RenderSystem* system : m_systems) {
        system->update(dt);
    }
}

///////////////////////////////////////////////////////////
void Renderer::render(Camera& camera, Framebuffer& target) {
    // Render standard pass
    doRenderPass(camera, target, RenderPass::Default);
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
    ContextUniformBuffers buffers({m_cameraBuffer, m_lightsBuffer});
    RenderPassContext context(camera, pass, buffers);
    context.isDeferredPass = true;

    // Update camera buffer
    Matrix4f projView = camera.getProjMatrix() * camera.getViewMatrix();

    CB_Camera cameraBlock;
    cameraBlock.m_cameraPos = camera.getPosition();
    cameraBlock.m_projView = projView;

    {
        CB_Camera* mapped =
            (CB_Camera*)m_cameraBuffer.map(MapMode::Write, MapFlag::Discard);
        mapped[0] = cameraBlock;
        m_cameraBuffer.unmap();
    }

    // Shadow pass only renders once
    if (pass == RenderPass::Shadow) {
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
    applyLighting(gbuffer);

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
priv::GBuffer& Renderer::getGBuffer(Framebuffer& target) {
    auto* device = m_device->getImpl();

    // Check if we have a G-buffer for this target
    auto it = m_impl->m_gBuffers.find(&target);
    if (it == m_impl->m_gBuffers.end()) {
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

        // Create intermediate texture if needed
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

        // Depth buffer
        texDesc.Name = "Depth buffer";
        texDesc.Format = rpDesc.pAttachments[2].Format;
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

        // Create deferred resource binding
        gbuffer.m_deferredBinding = m_deferredPipeline.createResourceBinding();
        gbuffer.m_deferredBinding
            .set(Shader::Pixel, "g_colorTexture", gbuffer.m_colorTexture);
        gbuffer.m_deferredBinding
            .set(Shader::Pixel, "g_depthTexture", gbuffer.m_depthZTexture);

        // Add to map
        it = m_impl->m_gBuffers
                 .emplace(std::make_pair(&target, std::move(gbuffer)))
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

    OptimizedClearValue clearValues[4];
    // Color
    clearValues[0].Color[0] = 0.f;
    clearValues[0].Color[1] = 0.f;
    clearValues[0].Color[2] = 0.f;
    clearValues[0].Color[3] = 0.f;

    // Depth Z
    clearValues[1].Color[0] = 1.f;
    clearValues[1].Color[1] = 1.f;
    clearValues[1].Color[2] = 1.f;
    clearValues[1].Color[3] = 1.f;

    // Depth buffer
    clearValues[2].DepthStencil.Depth = 1.f;

    // Final color buffer
    clearValues[3].Color[0] = 0.0625f;
    clearValues[3].Color[1] = 0.0625f;
    clearValues[3].Color[2] = 0.0625f;
    clearValues[3].Color[3] = 1.f;

    rpBeginInfo.pClearValues = clearValues;
    rpBeginInfo.ClearValueCount = _countof(clearValues);
    rpBeginInfo.StateTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
    device->m_deviceContext->BeginRenderPass(rpBeginInfo);
}

///////////////////////////////////////////////////////////
void Renderer::applyLighting(const priv::GBuffer& gbuffer) {
    auto& deviceContext = m_device->context;

    // Bind deferred pipeline
    deviceContext.setPipeline(m_deferredPipeline);
    // Bind resource binding
    deviceContext.setResourceBinding(gbuffer.m_deferredBinding);

    // Draw quad
    deviceContext.draw(4);
}

} // namespace ply