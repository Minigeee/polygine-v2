#pragma once

#include <ply/core/Handle.h>
#include <ply/core/PoolAllocator.h>
#include <ply/graphics/Buffer.h>
#include <ply/graphics/Camera.h>
#include <ply/graphics/Framebuffer.h>
#include <ply/graphics/Pipeline.h>
#include <ply/graphics/Shader.h>

#include <vector>

namespace ply {

class RenderSystem;
enum class RenderPass;

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
    ///
    ///////////////////////////////////////////////////////////
    void initialize(RenderDevice* device);

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
    /// \brief Set ambient color
    ///
    ///////////////////////////////////////////////////////////
    void setAmbient(const Vector3f& color);

    ///////////////////////////////////////////////////////////
    /// \brief Get ambient color
    ///
    /// \return Ambient color
    ///
    ///////////////////////////////////////////////////////////
    const Vector3f& getAmbient() const;

private:
    ///////////////////////////////////////////////////////////
    // Lights
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    /// \brief Update lights buffer by retrieving light information from from
    /// scene
    ///////////////////////////////////////////////////////////
    void updateLightsBuffer();

    ///////////////////////////////////////////////////////////
    // Shadows
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    /// \brief Do shadow passes based on main dir light
    ///////////////////////////////////////////////////////////
    void doShadowPasses(Camera& camera);

    ///////////////////////////////////////////////////////////
    // Renderer
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    /// \brief Get deferred lighting shader (load if not yet)
    ///////////////////////////////////////////////////////////
    Shader& getDeferredShader();

    ///////////////////////////////////////////////////////////
    /// \brief Get deferred framebuffer for given target
    ///////////////////////////////////////////////////////////
    Framebuffer* getDeferredFramebuffer(Framebuffer& target);

    ///////////////////////////////////////////////////////////
    /// \brief Perform a render pass
    ///////////////////////////////////////////////////////////
    void renderPass(Camera& camera, Framebuffer& target, RenderPass pass);

private:
    Vector3f m_ambient; //!< Ambient color

    std::vector<Framebuffer*>
        m_shadowMaps; //!< Shadow maps, one for each cascade level

    std::vector<RenderSystem*> m_systems; //!< A list of render systems
    HashMap<uint32_t, Framebuffer*>
        m_gBuffers; //!< Map each output target framebuffer to its own deferred
    //!< pipeline framebuffer (to minimize resizing buffers
    //!< between different targets)

    Pipeline m_deferredPipeline; //!< Pipeline used for deferred rendering
    Shader m_deferredShader;     //!< Shader used to render lighting
    Buffer m_cameraBuffer;       //!< Buffer used to store camrea uniforms
    Buffer m_lightsBuffer;    //!< Buffer used to store dynamic light uniforms
    Buffer m_shadowBuffer;    //!< Buffer used to store shadow data
    Buffer m_animationBuffer; //!< Buffer used to store animation data
};

} // namespace ply