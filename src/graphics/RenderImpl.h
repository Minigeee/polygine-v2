#pragma once

#include <ply/core/HandleArray.h>
#include <ply/graphics/Types.h>

#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>

#include <Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Graphics/GraphicsEngine/interface/SwapChain.h>
#include <Graphics/GraphicsTools/interface/ShaderMacroHelper.hpp>

#include <Common/interface/RefCntAutoPtr.hpp>

using namespace Diligent;

#define BUFFER(x) static_cast<Diligent::IBuffer*>(x)
#define TEXTURE(x) static_cast<Diligent::ITexture*>(x)
#define PIPELINE(x) static_cast<Diligent::IPipelineState*>(x)
#define RESOURCE_BINDING(x) static_cast<Diligent::IShaderResourceBinding*>(x)

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
            m_pipelines;                                 //!< Pipeline states
        HandleArray<RefCntAutoPtr<IShader>> m_shaders;   //!< Shaders
        HandleArray<RefCntAutoPtr<IBuffer>> m_buffers;   //!< Buffers
        HandleArray<RefCntAutoPtr<ITexture>> m_textures; //!< Textures
        HandleArray<RefCntAutoPtr<IShaderResourceBinding>> m_resourceBindings;

        RESOURCE_STATE_TRANSITION_MODE m_transitionMode =
            RESOURCE_STATE_TRANSITION_MODE_TRANSITION; //!< Transition mode
    };

    ///////////////////////////////////////////////////////////
    /// \brief Render system implementation details
    ///
    ///////////////////////////////////////////////////////////
    struct RenderSystemImpl {
        std::vector<StateTransitionDesc> m_transitions;
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
        std::vector<std::pair<std::string, std::string>> MacroList;
    };

    ///////////////////////////////////////////////////////////
    /// \brief Buffer object create descriptor
    ///
    ///////////////////////////////////////////////////////////
    struct BufferDesc : public Diligent::BufferDesc {
        BufferData Data;
    };

    ///////////////////////////////////////////////////////////
    /// \brief Texture object create descriptor
    ///
    ///////////////////////////////////////////////////////////
    struct TextureDesc : public Diligent::TextureDesc {
        std::vector<TextureSubResData> Data;
    };

    ///////////////////////////////////////////////////////////
    /// \brief Convert polygine texture format to Diligent texture format
    ///
    ///////////////////////////////////////////////////////////
    TEXTURE_FORMAT convertTextureFormat(TextureFormat format);

    ///////////////////////////////////////////////////////////
    /// \brief Convert Diligent texture format to polygine texture format
    ///
    ///////////////////////////////////////////////////////////
    TextureFormat convertTextureFormat(TEXTURE_FORMAT format);

} // namespace priv

} // namespace ply