#include <ply/graphics/RenderDevice.h>

#include "RenderImpl.h"
#include <ply/engine/Window.h>

#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>

#define TRANSITION_MODE(x) \
    static_cast<Diligent::RESOURCE_STATE_TRANSITION_MODE>(x)

namespace ply {

///////////////////////////////////////////////////////////
RenderDevice::RenderDevice() :
    m_device(nullptr) {
    m_device = new priv::DeviceImpl();
}

///////////////////////////////////////////////////////////
RenderDevice::~RenderDevice() {
    if (m_device) {
        delete m_device;
    }

    m_device = nullptr;
}

///////////////////////////////////////////////////////////
RenderDevice::RenderDevice(RenderDevice&& other) noexcept :
    m_device(other.m_device) {
    other.m_device = nullptr;
}

///////////////////////////////////////////////////////////
RenderDevice& RenderDevice::operator=(RenderDevice&& other) noexcept {
    if (this != &other) {
        m_device = other.m_device;
        other.m_device = nullptr;
    }

    return *this;
}

///////////////////////////////////////////////////////////
bool RenderDevice::initialize(Window* window) {
    SwapChainDesc SCDesc;
    WindowHandle handle = window->getNativeHandle();

#ifdef VULKAN_SUPPORTED
    #if EXPLICITLY_LOAD_ENGINE_VK_DLL
    // Load the dll and import GetEngineFactoryVk() function
    auto GetEngineFactoryVk = LoadGraphicsEngineVk();
    #endif
    EngineVkCreateInfo EngineCI;

    auto* pFactoryVk = GetEngineFactoryVk();
    pFactoryVk->CreateDeviceAndContextsVk(
        EngineCI,
        &m_device->m_renderDevice,
        &m_device->m_deviceContext
    );

    if (!m_device->m_swapChain && handle != nullptr) {
        Win32NativeWindow Window{handle};
        pFactoryVk->CreateSwapChainVk(
            m_device->m_renderDevice,
            m_device->m_deviceContext,
            SCDesc,
            Window,
            &m_device->m_swapChain
        );
    }
    const auto& info = m_device->m_renderDevice->GetDeviceInfo();

    m_device->m_engineFactory = pFactoryVk;

#elif D3D12_SUPPORTED
    #if ENGINE_DLL
    // Load the dll and import GetEngineFactoryD3D12() function
    auto GetEngineFactoryD3D12 = LoadGraphicsEngineD3D12();
    #endif
    EngineD3D12CreateInfo EngineCI;

    auto* pFactoryD3D12 = GetEngineFactoryD3D12();
    pFactoryD3D12->CreateDeviceAndContextsD3D12(
        EngineCI,
        &m_device->m_renderDevice,
        &m_device->m_deviceContext
    );
    Win32NativeWindow Window{handle};
    pFactoryD3D12->CreateSwapChainD3D12(
        m_device->m_renderDevice,
        m_device->m_deviceContext,
        SCDesc,
        FullScreenModeDesc{},
        Window,
        &m_device->m_swapChain
    );

    m_device->m_engineFactory = pFactoryD3D12;

#elif GL_SUPPORTED
    #if EXPLICITLY_LOAD_ENGINE_GL_DLL
    // Load the dll and import GetEngineFactoryOpenGL() function
    auto GetEngineFactoryOpenGL = LoadGraphicsEngineOpenGL();
    #endif
    auto* pFactoryOpenGL = GetEngineFactoryOpenGL();

    EngineGLCreateInfo EngineCI;
    EngineCI.Window.hWnd = handle;

    pFactoryOpenGL->CreateDeviceAndSwapChainGL(
        EngineCI,
        &m_device->m_renderDevice,
        &m_device->m_deviceContext,
        SCDesc,
        &m_device->m_swapChain
    );

    m_device->m_engineFactory = pFactoryOpenGL;

#endif

    // Create default shader factory
    m_device->m_engineFactory->CreateDefaultShaderSourceStreamFactory(
        "shaders;shaders/inc;",
        &m_device->m_shaderFactory
    );

    // Set up context
    context.m_device = m_device;

    return true;
}

///////////////////////////////////////////////////////////
void RenderDevice::setShaderPath(const std::string& path) {
    m_device->m_engineFactory->CreateDefaultShaderSourceStreamFactory(
        path.c_str(),
        &m_device->m_shaderFactory
    );
}

///////////////////////////////////////////////////////////
ShaderBuilder RenderDevice::shader() {
    return ShaderBuilder(this);
}

///////////////////////////////////////////////////////////
PipelineBuilder RenderDevice::pipeline() {
    return PipelineBuilder(this);
}

///////////////////////////////////////////////////////////
BufferBuilder RenderDevice::buffer() {
    return BufferBuilder(this);
}

///////////////////////////////////////////////////////////
TextureBuilder RenderDevice::texture() {
    return TextureBuilder(this);
}

///////////////////////////////////////////////////////////
Texture RenderDevice::texture(const Image& image, uint32_t mips) {
    return TextureBuilder(this).from(image).mips(mips).create();
}

///////////////////////////////////////////////////////////
Framebuffer RenderDevice::framebuffer() {
    return Framebuffer(this);
}

///////////////////////////////////////////////////////////
priv::DeviceImpl* RenderDevice::getImpl() const {
    return m_device;
}

///////////////////////////////////////////////////////////
uint32_t RenderDevice::getConstantBufferAlignment() const {
    return m_device->m_renderDevice->GetAdapterInfo()
        .Buffer.ConstantBufferOffsetAlignment;
}

///////////////////////////////////////////////////////////
uint32_t RenderDevice::getStructuredBufferAlignment() const {
    return m_device->m_renderDevice->GetAdapterInfo()
        .Buffer.StructuredBufferOffsetAlignment;
}

///////////////////////////////////////////////////////////
bool RenderDevice::isGlDevice() const {
    return m_device->m_renderDevice->GetDeviceInfo().IsGLDevice();
}

///////////////////////////////////////////////////////////
bool RenderDevice::isVulkanDevice() const {
    return m_device->m_renderDevice->GetDeviceInfo().IsVulkanDevice();
}

///////////////////////////////////////////////////////////
bool RenderDevice::isWebGpuDevice() const {
    return m_device->m_renderDevice->GetDeviceInfo().IsWebGPUDevice();
}

///////////////////////////////////////////////////////////
RenderContext::RenderContext() :
    m_device(nullptr),
    m_clearColor{0.0f, 0.0f, 0.0f, 1.0f},
    m_clearDepth(1.0f),
    m_clearStencil(0),
    m_transitionMode(RESOURCE_STATE_TRANSITION_MODE_TRANSITION),
    m_currentFramebuffer(nullptr) {}

///////////////////////////////////////////////////////////
void RenderContext::clear(ClearFlags flags) {
    CHECK_F(
        m_currentFramebuffer != nullptr,
        "must set a valid framebuffer before clearing "
        "RenderContext::setRenderTarget."
    );

    bool isDefaultFramebuffer = m_currentFramebuffer == &Framebuffer::Default;

    if ((bool)(flags & ClearFlags::Color)) {
        if (isDefaultFramebuffer) {
            auto* pRTV = m_device->m_swapChain->GetCurrentBackBufferRTV();
            m_device->m_deviceContext->ClearRenderTarget(
                pRTV,
                &m_clearColor,
                TRANSITION_MODE(m_transitionMode)
            );
        } else {
            // If custom framebuffer, clear all color attachments
            for (size_t i = 0; i < m_currentFramebuffer->getNumColorTextures();
                 ++i) {
                auto* pRTV = static_cast<ITextureView*>(
                    m_currentFramebuffer->m_colorTextureViews[i]
                );

                m_device->m_deviceContext->ClearRenderTarget(
                    pRTV,
                    &m_clearColor,
                    TRANSITION_MODE(m_transitionMode)
                );
            }
        }
    }

    auto dsMask = ClearFlags::Depth | ClearFlags::Stencil;
    if ((bool)(flags & dsMask)) {
        auto dsFlags = (uint8_t)dsMask >> 1;

        auto* pDSV = m_device->m_swapChain->GetDepthBufferDSV();
        if (!isDefaultFramebuffer) {
            // If custom framebuffer, use the depth stencil view from the
            // framebuffer
            pDSV = static_cast<ITextureView*>(
                m_currentFramebuffer->m_depthTextureView
            );
        }

        m_device->m_deviceContext->ClearDepthStencil(
            pDSV,
            static_cast<CLEAR_DEPTH_STENCIL_FLAGS>(dsFlags),
            m_clearDepth,
            m_clearStencil,
            TRANSITION_MODE(m_transitionMode)
        );
    }
}

///////////////////////////////////////////////////////////
void RenderContext::setClearColor(const Vector4f& color) {
    m_clearColor = color;
}

///////////////////////////////////////////////////////////
void RenderContext::setClearDepth(float depth) {
    m_clearDepth = depth;
}

///////////////////////////////////////////////////////////
void RenderContext::setClearStencil(uint8_t stencil) {
    m_clearStencil = stencil;
}

///////////////////////////////////////////////////////////
void RenderContext::setVertexBuffers(
    const BufferList& list,
    uint32_t slot,
    const uint64_t* offsets
) {
    m_device->m_deviceContext->SetVertexBuffers(
        slot,
        list.size(),
        reinterpret_cast<IBuffer**>(list.data()),
        offsets,
        TRANSITION_MODE(m_transitionMode)
    );
}

///////////////////////////////////////////////////////////
void RenderContext::setIndexBuffer(const Buffer& buffer, uint64_t offset) {
    m_device->m_deviceContext->SetIndexBuffer(
        static_cast<IBuffer*>(buffer.getResource()),
        offset,
        TRANSITION_MODE(m_transitionMode)
    );
}

///////////////////////////////////////////////////////////
void RenderContext::setPipeline(const Pipeline& pipeline) {
    m_device->m_deviceContext->SetPipelineState(
        static_cast<IPipelineState*>(pipeline.getResource())
    );
}

///////////////////////////////////////////////////////////
void RenderContext::setResourceBinding(const ResourceBinding& binding) {
    m_device->m_deviceContext->CommitShaderResources(
        static_cast<IShaderResourceBinding*>(binding.getResource()),
        TRANSITION_MODE(m_transitionMode)
    );
}

///////////////////////////////////////////////////////////
void RenderContext::setRenderTarget(Framebuffer& framebuffer) {
    framebuffer.bind(m_device);
    m_currentFramebuffer = &framebuffer;
}

///////////////////////////////////////////////////////////
void RenderContext::setResourceStates(ResourceBinding& binding) {
    // Set resource states for the binding
    m_device->m_deviceContext->TransitionShaderResources(
        static_cast<IShaderResourceBinding*>(binding.getResource())
    );
}

///////////////////////////////////////////////////////////
void RenderContext::setRenderPassMode(bool enabled) {
    m_device->m_transitionMode =
        enabled ? RESOURCE_STATE_TRANSITION_MODE_VERIFY
                : RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
    m_transitionMode = m_device->m_transitionMode;
}

///////////////////////////////////////////////////////////
void RenderContext::draw(uint32_t numVertices, uint32_t instances) {
    DrawAttribs drawAttrs;
    drawAttrs.NumVertices = numVertices;
    drawAttrs.NumInstances = instances;
    drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    m_device->m_deviceContext->Draw(drawAttrs);
}

///////////////////////////////////////////////////////////
void RenderContext::drawIndexed(
    uint32_t numVertices,
    uint32_t instances,
    Type dtype
) {
    DrawIndexedAttribs drawAttrs;
    drawAttrs.NumIndices = numVertices;
    drawAttrs.NumInstances = instances;
    drawAttrs.IndexType = dtype == Type::Uint32 ? VT_UINT32 : VT_UINT16;
    drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    m_device->m_deviceContext->DrawIndexed(drawAttrs);
}

///////////////////////////////////////////////////////////
void RenderContext::present(uint32_t sync) {
    m_device->m_swapChain->Present(sync);
    m_currentFramebuffer = nullptr;
}

} // namespace ply