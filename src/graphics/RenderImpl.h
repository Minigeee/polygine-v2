#pragma once

#include <ply/core/HandleArray.h>

#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>

#include <Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Graphics/GraphicsEngine/interface/SwapChain.h>
#include <Graphics/GraphicsTools/interface/ShaderMacroHelper.hpp>

#include <Common/interface/RefCntAutoPtr.hpp>

using namespace Diligent;

namespace ply {

namespace priv {

    ///////////////////////////////////////////////////////////
    /// \brief Renderer implementation details
    ///
    ///////////////////////////////////////////////////////////
    struct DeviceImpl {
        RefCntAutoPtr<IEngineFactory> m_engineFactory; //!< Engine factory
        RefCntAutoPtr<IRenderDevice> m_renderDevice;   //!< Render device
        RefCntAutoPtr<IShaderSourceInputStreamFactory>
            m_shaderFactory;                   //!< Shader source stream factory
        RefCntAutoPtr<ISwapChain> m_swapChain; //!< Swap chain
        RefCntAutoPtr<IDeviceContext> m_deviceContext; //!< Device context

        HandleArray<RefCntAutoPtr<IPipelineState>>
            m_pipelines;                               //!< Pipeline states
        HandleArray<RefCntAutoPtr<IShader>> m_shaders; //!< Shaders
    };

    ///////////////////////////////////////////////////////////
    /// \brief Pipeline object create descriptor
    ///
    ///////////////////////////////////////////////////////////
    struct PipelineDesc : public GraphicsPipelineStateCreateInfo {};

    ///////////////////////////////////////////////////////////
    /// \brief Shader object create descriptor
    ///
    ///////////////////////////////////////////////////////////
    struct ShaderDesc : public ShaderCreateInfo {
        ShaderMacroHelper MacroHelper;
    };

} // namespace priv

} // namespace ply