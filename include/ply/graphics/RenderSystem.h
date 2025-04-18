
#pragma once

#include <ply/core/Macros.h>
#include <ply/graphics/Buffer.h>
#include <ply/graphics/Camera.h>
#include <ply/graphics/RenderPass.h>

namespace ply {

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
    RenderPass::Type pass;        //!< Current render pass
    ContextUniformBuffers buffers; //!< List of shared uniform buffers
    bool isDeferredPass; //!< Is the current pass a deferred lighting pass
                         //!< (can't render transparent)
};

///////////////////////////////////////////////////////////
/// \brief The base class for all rendering procedure classes
///
///////////////////////////////////////////////////////////
class RenderSystem {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Default destructor
    ///
    ///////////////////////////////////////////////////////////
    RenderSystem() {}

    ///////////////////////////////////////////////////////////
    /// \brief This function should initialize anything that is scene dependent,
    ///        such as access to entities
    ///
    /// \param scene A pointer to the scene to initialize the system for
    ///
    ///////////////////////////////////////////////////////////
    virtual void initialize() = 0;

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
