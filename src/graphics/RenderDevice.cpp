#include <ply/graphics/RenderDevice.h>

#include "RenderImpl.h"
#include <ply/engine/Window.h>

#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>

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
        "shaders;shaders\\inc;",
        &m_device->m_shaderFactory
    );

    return true;
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
void RenderDevice::bindBackBuffer() {
    auto* pRTV = m_device->m_swapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_device->m_swapChain->GetDepthBufferDSV();
    m_device->m_deviceContext->SetRenderTargets(
        1,
        &pRTV,
        pDSV,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

///////////////////////////////////////////////////////////
void RenderDevice::clear(const Vector3f& color) {
    auto* pRTV = m_device->m_swapChain->GetCurrentBackBufferRTV();
    m_device->m_deviceContext->ClearRenderTarget(
        pRTV,
        &color,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

///////////////////////////////////////////////////////////
void RenderDevice::clearDepth(float depth) {
    auto* pDSV = m_device->m_swapChain->GetDepthBufferDSV();
    m_device->m_deviceContext->ClearDepthStencil(
        pDSV,
        CLEAR_DEPTH_FLAG,
        depth,
        0,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

///////////////////////////////////////////////////////////
void RenderDevice::clearStencil(uint8_t stencil) {
    auto* pDSV = m_device->m_swapChain->GetDepthBufferDSV();
    m_device->m_deviceContext->ClearDepthStencil(
        pDSV,
        CLEAR_STENCIL_FLAG,
        0.0f,
        stencil,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );
}

///////////////////////////////////////////////////////////
void RenderDevice::draw(Pipeline* pipeline, uint32_t numVertices) {
    auto context = m_device->m_deviceContext;
    auto pso = static_cast<IPipelineState*>(pipeline->m_pipeline);
    context->SetPipelineState(pso);

    DrawAttribs drawAttrs;
    drawAttrs.NumVertices = numVertices;
    context->Draw(drawAttrs);
}

///////////////////////////////////////////////////////////
void RenderDevice::present(uint32_t sync) {
    m_device->m_swapChain->Present(sync);
}

} // namespace ply