
#pragma once

#include <ply/core/Macros.h>
#include <ply/graphics/Buffer.h>
#include <ply/graphics/Camera.h>
#include <ply/graphics/RenderPass.h>

namespace ply {

class Renderer;

namespace priv {
    struct RenderSystemImpl;
}

///////////////////////////////////////////////////////////
/// \brief Contains references to shared uniform buffers
///
///////////////////////////////////////////////////////////
struct ContextUniformBuffers {
    Buffer& camera; //!< Camera data
    Buffer& lights; //!< Dynamic lights buffer
};

///////////////////////////////////////////////////////////
/// \brief Used to pass shared render data to each system
///
///////////////////////////////////////////////////////////
struct RenderPassContext {
    RenderPassContext(
        Camera& camera,
        RenderPass::Type pass,
        ContextUniformBuffers buffers
    );

    Camera& camera;                //!< Camera being used to render scene
    RenderPass::Type pass;         //!< Current render pass
    ContextUniformBuffers buffers; //!< List of shared uniform buffers
    bool isDeferredPass; //!< Is the current pass a deferred lighting pass
                         //!< (can't render transparent)
};

///////////////////////////////////////////////////////////
/// \brief The base class for all rendering procedure classes
///
///////////////////////////////////////////////////////////
class RenderSystem {
    friend Renderer;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Data passed to system initialization
    ///
    ///////////////////////////////////////////////////////////
    struct Init {
        RenderDevice* device;          //!< Render device to use for rendering
        ContextUniformBuffers buffers; //!< Shared uniform buffers
        RenderPass& renderPass;        //!< Render pass to use for rendering
    };

public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    RenderSystem();

    ///////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ///////////////////////////////////////////////////////////
    virtual ~RenderSystem();

    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;
    RenderSystem(RenderSystem&&) noexcept;
    RenderSystem& operator=(RenderSystem&&) noexcept;

    ///////////////////////////////////////////////////////////
    /// \brief This function should initialize anything that is scene dependent,
    ///        such as access to entities
    ///
    /// \param context Initialization context containing useful initializtion
    /// data
    ///
    ///////////////////////////////////////////////////////////
    virtual void initialize(const RenderSystem::Init& context) = 0;

    ///////////////////////////////////////////////////////////
    /// \brief This function should be used to perform any graphics related
    /// updates (such as animations)
    ///
    /// This function will be called once per frame before rendering.
    ///
    /// \param dt The time since the last frame in seconds
    ///
    ///////////////////////////////////////////////////////////
    virtual void update(float dt);

    ///////////////////////////////////////////////////////////
    /// \brief Execute the rendering procedures
    ///
    /// This function is run once for every render pass, meaning
    /// it is run multiple times per frame.
    ///
    /// \param camera The camera to render from the perspective of
    /// \param pass The render pass that is being executed
    ///
    ///////////////////////////////////////////////////////////
    virtual void render(RenderPassContext& context) = 0;

    ///////////////////////////////////////////////////////////
    /// \brief Check if the render system has a deferred render pass
    ///
    /// Override this to return false to disable deferred rendering
    /// for this render system.
    ///
    /// \return True if this system handles deferred rendering
    ///
    ///////////////////////////////////////////////////////////
    virtual bool hasDeferredPass() const;

    ///////////////////////////////////////////////////////////
    /// \brief Check if the render system has a forward render pass
    ///
    /// Override this to return true to enable forward rendering
    /// for this render system.
    ///
    /// \return True if this system handles forward rendering
    ///
    ///////////////////////////////////////////////////////////
    virtual bool hasForwardPass() const;

protected:
    ///////////////////////////////////////////////////////////
    /// \brief Register a resource with the state it should be in during the
    /// render phase
    ///
    /// \param resource The resource to register
    /// \param state The state the resource should be in during the render phase
    ///
    ///////////////////////////////////////////////////////////
    void addResource(GpuResource& resource, ResourceState state);

    RenderDevice* m_device; //!< Pointer to the render device used for rendering

private:
    priv::RenderSystemImpl*
        m_impl; //!< Pointer to implementation details of the render system
};

} // namespace ply

///////////////////////////////////////////////////////////
/// \class poly::RenderSystem
/// \ingroup Graphics
///
/// The RenderSystem is the base class for any class that needs
/// a custom rendering procedure, such as terrain, the skybox,
/// a spatial structure for rendering (i.e. an octree), and anything
/// else that needs custom render procedures. Advanced knowledge
/// of rendering back ends will be helpful if custom render systems are needed.
///
/// Look at the source code of Octree, Terrain, or Skybox for examples
/// of how to implement a render system.
///
///////////////////////////////////////////////////////////
