#include <ply/graphics/Framebuffer.h>

#include "RenderImpl.h"
#include <ply/core/PoolAllocator.h>
#include <ply/graphics/RenderDevice.h>
#include <ply/graphics/Texture.h>

namespace ply {

Framebuffer Framebuffer::Default;

///////////////////////////////////////////////////////////
Framebuffer::Framebuffer() :
    m_device(nullptr) {
    // No device, no attachments
}

///////////////////////////////////////////////////////////
Framebuffer::Framebuffer(RenderDevice* device) :
    m_device(device) {
    // No device, no attachments
}

///////////////////////////////////////////////////////////
Framebuffer::~Framebuffer() {
    // Free color textures
    for (auto& attachment : m_colorTextures) {
        if (attachment.m_owner && attachment.m_texture) {
            Pool<Texture>::free(attachment.m_texture);
        }
    }
    m_colorTextures.clear();
    m_colorTextureViews.clear();

    // Free depth texture
    if (m_depthTexture.m_texture && m_depthTexture.m_owner) {
        Pool<Texture>::free(m_depthTexture.m_texture);
        m_depthTexture.m_texture = nullptr;
    }
}

///////////////////////////////////////////////////////////
Framebuffer::Framebuffer(Framebuffer&& other) noexcept {
    m_device = other.m_device;
    m_colorTextures = std::move(other.m_colorTextures);
    m_colorTextureViews = std::move(other.m_colorTextureViews);
    m_depthTexture = other.m_depthTexture;
    m_depthTextureView = other.m_depthTextureView;

    other.m_device = nullptr;
    other.m_depthTexture = {};
    other.m_depthTextureView = nullptr;
}

///////////////////////////////////////////////////////////
Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
    if (this != &other) {
        m_device = other.m_device;
        m_colorTextures = std::move(other.m_colorTextures);
        m_colorTextureViews = std::move(other.m_colorTextureViews);
        m_depthTexture = other.m_depthTexture;
        m_depthTextureView = other.m_depthTextureView;

        other.m_device = nullptr;
        other.m_depthTexture = {};
        other.m_depthTextureView = nullptr;
    }
    return *this;
}

///////////////////////////////////////////////////////////
void Framebuffer::attachColor(Texture* texture) {
    m_colorTextures.push_back({texture, false});
    m_colorTextureViews.push_back(
        static_cast<ITexture*>(texture->getResource())
            ->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET)
    );
}

///////////////////////////////////////////////////////////
Texture* Framebuffer::attachColor(
    const Vector2u& size,
    TextureFormat format,
    ResourceBind bind
) {
    CHECK_F(
        m_device != nullptr,
        "Framebuffer must be created with a valid render device."
    );

    // Create texture
    auto* texture = Pool<Texture>::alloc();
    *texture =
        m_device->texture()
            .size(size.x, size.y)
            .format(format)
            .bind(bind)
            .usage(ResourceUsage::Default)
            .create();

    m_colorTextures.push_back({texture, true});
    m_colorTextureViews.push_back(
        static_cast<ITexture*>(texture->getResource())
            ->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET)
    );

    return texture;
}

///////////////////////////////////////////////////////////
void Framebuffer::attachDepth(Texture* texture) {
    m_depthTexture = {texture, false};
    m_depthTextureView = static_cast<ITexture*>(texture->getResource())
                             ->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);
}

///////////////////////////////////////////////////////////
Texture* Framebuffer::attachDepth(
    const Vector2u& size,
    TextureFormat format,
    ResourceBind bind
) {
    CHECK_F(
        m_device != nullptr,
        "Framebuffer must be created with a valid render device."
    );

    // Create texture
    auto* texture = Pool<Texture>::alloc();
    *texture =
        m_device->texture()
            .size(size.x, size.y)
            .format(format)
            .bind(bind)
            .usage(ResourceUsage::Default)
            .create();

    m_depthTexture = {texture, false};
    m_depthTextureView = static_cast<ITexture*>(texture->getResource())
                             ->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL);

    return texture;
}

///////////////////////////////////////////////////////////
size_t Framebuffer::getNumColorTextures() const {
    return m_colorTextures.size();
}

///////////////////////////////////////////////////////////
Texture* Framebuffer::getColorTexture(size_t index) const {
    if (index < m_colorTextures.size())
        return m_colorTextures[index].m_texture;
    return nullptr;
}

///////////////////////////////////////////////////////////
Texture* Framebuffer::getDepthTexture() const {
    return m_depthTexture.m_texture;
}

///////////////////////////////////////////////////////////
void Framebuffer::bind(RenderDevice* device) {
    bind(device->getImpl());
}

///////////////////////////////////////////////////////////
void Framebuffer::bind(priv::DeviceImpl* device) {
    // If default framebuffer, use swapchain
    if (this == &Framebuffer::Default) {
        auto* pRTV = device->m_swapChain->GetCurrentBackBufferRTV();
        auto* pDSV = device->m_swapChain->GetDepthBufferDSV();
        device->m_deviceContext->SetRenderTargets(
            1,
            &pRTV,
            pDSV,
            RESOURCE_STATE_TRANSITION_MODE_TRANSITION
        );
    } else {
        device->m_deviceContext->SetRenderTargets(
            m_colorTextureViews.size(),
            reinterpret_cast<ITextureView**>(m_colorTextureViews.data()),
            static_cast<ITextureView*>(m_depthTextureView),
            RESOURCE_STATE_TRANSITION_MODE_TRANSITION
        );
    }
}

} // namespace ply