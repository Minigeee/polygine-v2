
#include <ply/graphics/RenderSystem.h>

namespace ply {

///////////////////////////////////////////////////////////
RenderPassContext::RenderPassContext(
    Camera& camera,
    RenderPass::Type pass,
    ContextUniformBuffers buffers
) :
    camera(camera),
    pass(pass),
    buffers(buffers),
    isDeferredPass(true) {}

///////////////////////////////////////////////////////////
bool RenderSystem::hasDeferredPass() const {
    return true;
}

///////////////////////////////////////////////////////////
bool RenderSystem::hasForwardPass() const {
    return false;
}

} // namespace ply