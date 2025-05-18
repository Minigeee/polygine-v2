#pragma once

#include <ply/core/Handle.h>
#include <ply/core/PoolAllocator.h>
#include <ply/ecs/Query.h>
#include <ply/graphics/Buffer.h>
#include <ply/graphics/Camera.h>
#include <ply/graphics/Framebuffer.h>
#include <ply/graphics/Material.h>
#include <ply/graphics/Pipeline.h>
#include <ply/graphics/RenderPass.h>
#include <ply/graphics/RenderSystem.h>
#include <ply/graphics/Shader.h>

#include <vector>

namespace ply {

struct RenderPassContext;
class World;

namespace priv {
    struct RendererImpl;
    struct GBuffer;
} // namespace priv

///////////////////////////////////////////////////////////
/// \brief Buffer size config
///
///////////////////////////////////////////////////////////
struct RendererBufferConfig {
    /// Size of the camera uniform buffer in number of struct elements (default
    /// 10)
    uint32_t cameraBufferSize = 10;

    /// Size of the animation uniform buffer in number of struct elements
    /// (default 20)
    uint32_t animBufferSize = 20;
};

///////////////////////////////////////////////////////////
/// \brief Renderer configuration parameters
///
///////////////////////////////////////////////////////////
struct RendererConfig {
    /// Texture format of the target framebuffer. This can not be changed
    /// after the renderer is initialized. (Default is automatically set to
    /// the default framebuffer format of the render device).
    TextureFormat targetFormat = TextureFormat::Unknown;

    /// Max number of directional lights that can be rendered in a single frame
    uint32_t maxDirLights = 10;
    /// Max number of point lights that can be rendered in a single frame
    uint32_t maxPointLights = 500;
    /// Max number of materials that can be rendered in a single frame
    uint32_t maxMaterials = 100;

    /// Max number of shadow cascade maps
    uint32_t maxShadowCascades = 3;

    /// Buffer configuration for the renderer
    RendererBufferConfig buffer;
};

///////////////////////////////////////////////////////////
/// \brief Manages the 3D rendering pipeline
///
///////////////////////////////////////////////////////////
class Renderer {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    Renderer();

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~Renderer();

    ///////////////////////////////////////////////////////////
    /// \brief Initialize renderer
    ///
    /// Must be called after the main window is created and a render device is
    /// created.
    ///
    /// \param device Render device to use
    /// \param config Renderer configuration parameters
    ///
    ///////////////////////////////////////////////////////////
    void initialize(
        RenderDevice* device,
        const RendererConfig& config = RendererConfig()
    );

    ///////////////////////////////////////////////////////////
    /// \brief Perform graphics related updates
    ///
    /// This function should be called once per frame, before the render()
    /// function. Provide the delta time to perform time based updates, such as
    /// animations.
    ///
    /// \param dt The time since the last frame in seconds
    ///
    ///////////////////////////////////////////////////////////
    void update(float dt);

    ///////////////////////////////////////////////////////////
    /// \brief Render everything currently in render pipeline
    ///
    /// This function renders all render systems in the order they were added.
    /// First, all render systems that have enabled deferred rendering will
    /// be rendered into a g-buffer, and deferred shading will be applied to the
    /// results. Note that transparent objects can't be rendered in this stage.
    /// After finishing the deferred render pass, a forward render pass will be
    /// run for the systems that have enabled forward rendering. This will allow
    /// render systems that require a forward pass to execute, and it will allow
    /// all transparent objects to be rendered. The results are rendered into
    /// the output \a target framebuffer.
    ///
    /// \param target The framebuffer to render to
    ///
    ///////////////////////////////////////////////////////////
    void render(Camera& camera, Framebuffer& target = Framebuffer::Default);

    ///////////////////////////////////////////////////////////
    /// \brief Add a render system
    ///
    /// Render systems are executed in the order they are added.
    /// They are responsible for actually rendering objects, such
    /// as standard models, meshes, skyboxes, etc.
    ///
    /// \link RenderSystem
    ///
    ///////////////////////////////////////////////////////////
    void add(RenderSystem* system);

    ///////////////////////////////////////////////////////////
    /// \brief Set the ECS world to get global rendering data from, including
    /// lights and shadows.
    ///
    /// Setting a world is required for lighting and shadows to work properly.
    ///
    /// \param world Pointer to the world to set
    ///
    ///////////////////////////////////////////////////////////
    void setWorld(World* world);

    ///////////////////////////////////////////////////////////
    /// \brief Create and register a material
    /// \return A pointer to the created material
    ///
    ///////////////////////////////////////////////////////////
    Material* material();

    ///////////////////////////////////////////////////////////
    /// \brief Register an existing material
    /// \param material Material to register
    /// \return The handle to the registered material
    ///
    ///////////////////////////////////////////////////////////
    Handle registerMaterial(const Material& material);

    ///////////////////////////////////////////////////////////
    /// \brief Remove a material
    /// \param handle Handle to the material
    ///
    ///////////////////////////////////////////////////////////
    void removeMaterial(Handle handle);

    ///////////////////////////////////////////////////////////
    /// \brief Set the material for a given handle
    /// \param handle Handle to the material
    /// \param material Material to set
    ///
    ///////////////////////////////////////////////////////////
    void setMaterial(Handle handle, const Material& material);

    ///////////////////////////////////////////////////////////
    /// \brief Get a mutable reference to the material for a given handle
    /// \param handle Handle to the material
    /// \return The material for the given handle
    ///
    ///////////////////////////////////////////////////////////
    Material& getMaterial(Handle handle);

private:
    void createRenderPass(TextureFormat targetFormat);

    void setUpDirLightsPipeline(const RendererConfig& config);

    void setUpLightVolumePipeline(const RendererConfig& config);

    void createPointLightBuffers(uint32_t maxPointLights);

    void setUpShadows();

    ///////////////////////////////////////////////////////////
    /// \brief Perform a render pass
    ///////////////////////////////////////////////////////////
    void
    doRenderPass(Camera& camera, Framebuffer& target, RenderPass::Type pass);

    priv::GBuffer& getGBuffer(Framebuffer& target);

    void startRenderPass(const priv::GBuffer& gbuffer);

    void applyLighting(priv::GBuffer& gbuffer, RenderPassContext& context);

    void updateDirLights();

    void updatePointLights();

private:
    RenderDevice* m_device; //!< Pointer to render device
    World* m_world;         //!< Ecs world
    std::unique_ptr<priv::RendererImpl>
        m_impl;         //!< Pointer to renderer implementation members
    Vector3f m_ambient; //!< Ambient color
    ContextBufferBlockSizes m_bufferBlockSizes; //!< Buffer block sizes

    std::vector<RenderSystem*> m_systems; //!< A list of render systems
    HandleArray<Material> m_materials;    //!< A list of materials

    RenderPass m_renderPass; //!< Render pass wrapper
    bool m_usingGlsl;       //!< Whether we are using GLSL

    Pipeline m_shadowPipeline; //!< Pipeline used for shadow mapping
    Shader m_shadowShaderV;    //!< Shader used for shadow mapping (vertex)
    Framebuffer m_shadowMap;   //!< Shadow map framebuffer

    Pipeline
        m_ambientPipeline;  //!< Pipeline used for deferred ambient rendering
    Shader m_quadShader;    //!< Shader used to render quad
    Shader m_ambientShader; //!< Shader used to render ambient lighting

    Pipeline m_dirLightPipeline; //!< Pipeline used for directional lights
    Shader m_dirLightShaderV;  //!< Shader used for directional lights (vertex)
    Shader m_dirLightShaderP;  //!< Shader used for directional lights (pixel)
    Buffer m_dirLightInstance; //!< Instance buffer used for directional lights
    uint32_t m_numDirLights;   //!< Number of directional lights

    Pipeline m_lightVolumePipeline; //!< Pipeline used for light volumes
    Shader m_lightVolumeShaderV;    //!< Shader used for light volumes (vertex)
    Shader m_lightVolumeShaderP;    //!< Shader used for light volumes (pixel)
    Buffer m_pointLightVertex;      //!< Vertex buffer used for point lights
    Buffer m_pointLightIndex;       //!< Index buffer used for point lights
    Buffer m_pointLightInstance;    //!< Instance buffer used for point lights
    uint32_t m_numPointLights;      //!< Number of point lights

    Pipeline m_shadowVisPipeline; //!< Pipeline used for shadow map visualization
    ResourceBinding m_shadowVisBinding; //!< Resource binding used for shadow map visualization
    Shader m_shadowVisShaderV;    //!< Shader used for shadow map visualization (vertex)
    Shader m_shadowVisShaderP;    //!< Shader used for shadow map visualization (pixel)

    Buffer m_cameraBuffer;    //!< Buffer used to store camera uniforms
    Buffer m_lightsBuffer;    //!< Buffer used to store dynamic light uniforms
    Buffer m_materialBuffer;  //!< Buffer used to store material uniforms
    Buffer m_shadowBuffer;    //!< Buffer used to store shadow data
    Buffer m_animationBuffer; //!< Buffer used to store animation data

    // World
    Query m_queryPointLights; //!< Query for point lights in the world
    Query m_queryDirLights;   //!< Query for directional lights in the world
};

} // namespace ply