
#include <ply/graphics/RenderSystem.h>

#include "RenderImpl.h"
#include <ply/core/PoolAllocator.h>

namespace ply {

///////////////////////////////////////////////////////////
RenderPassContext::RenderPassContext(
    Camera& camera,
    RenderPass::Type pass,
    ContextConstantBuffers buffers,
    ContextBufferOffsets offsets
) :
    camera(camera),
    pass(pass),
    buffers(buffers),
    isDeferredPass(true),
    offsets(offsets) {}

///////////////////////////////////////////////////////////
RenderSystem::RenderSystem() :
    m_device(nullptr) {
    m_impl = Pool<priv::RenderSystemImpl>::alloc();
}

///////////////////////////////////////////////////////////
RenderSystem::~RenderSystem() {
    if (m_impl) {
        Pool<priv::RenderSystemImpl>::free(m_impl);
    }
    m_impl = nullptr;
}

///////////////////////////////////////////////////////////
RenderSystem::RenderSystem(RenderSystem&& other) noexcept :
    m_impl(std::exchange(other.m_impl, nullptr)),
    m_device(other.m_device) {
    other.m_device = nullptr;
}

///////////////////////////////////////////////////////////
RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept {
    if (this != &other) {
        m_impl = std::exchange(other.m_impl, nullptr);
        m_device = other.m_device;
        other.m_device = nullptr;
    }
    return *this;
}

///////////////////////////////////////////////////////////
void RenderSystem::update(float dt) {}

///////////////////////////////////////////////////////////
bool RenderSystem::hasDeferredPass() const {
    return true;
}

///////////////////////////////////////////////////////////
bool RenderSystem::hasForwardPass() const {
    return false;
}

///////////////////////////////////////////////////////////
void RenderSystem::addResource(GpuResource& resource, ResourceState state) {
    StateTransitionDesc desc;
    desc.pResource = static_cast<IDeviceObject*>(resource.getResource());
    desc.OldState = RESOURCE_STATE_UNKNOWN;
    desc.NewState = static_cast<Diligent::RESOURCE_STATE>(state);
    desc.Flags = STATE_TRANSITION_FLAG_UPDATE_STATE;
    m_impl->m_transitions.push_back(desc);
}

} // namespace ply