#pragma once

#include <ply/core/Handle.h>
#include <ply/graphics/GpuResource.h>
#include <ply/graphics/Shader.h>
#include <ply/graphics/Types.h>

#include <cstdint>
#include <vector>

namespace ply {

class RenderDevice;

namespace priv {
    struct PipelineDesc;
}

///////////////////////////////////////////////////////////
/// \brief Type of gpu pipeline
///
///////////////////////////////////////////////////////////
enum class PipelineType : uint8_t { Graphics = 0, Compute };

///////////////////////////////////////////////////////////
/// \brief Input layout descriptor
///
///////////////////////////////////////////////////////////
struct InputLayout {
    uint32_t index;
    uint32_t slot;
    uint32_t components;
    GpuType type;
    bool normalized;
};

///////////////////////////////////////////////////////////
/// \brief A gpu pipeline object
///////////////////////////////////////////////////////////
class Pipeline : public GpuResource {
    friend RenderDevice;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    Pipeline() : m_pipeline(nullptr) {}

    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    Pipeline(priv::DeviceImpl* device, Handle pipeline);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~Pipeline();

    GPU_RESOURCE(Pipeline, m_pipeline);

private:
    Handle m_handle;
    void* m_pipeline;
};

///////////////////////////////////////////////////////////
/// \brief A gpu pipeline object builder
///
///////////////////////////////////////////////////////////
class PipelineBuilder : public GpuResource {
public:
    PipelineBuilder(RenderDevice* device, PipelineType type = PipelineType::Graphics);
    ~PipelineBuilder();

    PipelineBuilder(const PipelineBuilder&) = delete;
    PipelineBuilder& operator=(const PipelineBuilder&) = delete;
    PipelineBuilder(PipelineBuilder&&) noexcept;
    PipelineBuilder& operator=(PipelineBuilder&&) noexcept;

    PipelineBuilder& addTargetFormat(TextureFormat format);

    PipelineBuilder& depthFormat(TextureFormat format);

    PipelineBuilder& topology(PrimitiveTopology topology);

    PipelineBuilder& fill(FillMode fillMode);

    PipelineBuilder& cull(CullMode cullMode);

    PipelineBuilder& depth(bool enabled);

    PipelineBuilder& depthWrite(bool enabled);

    PipelineBuilder& scissor(bool enabled);

    PipelineBuilder& blend(bool enabled, uint32_t index = 0);

    PipelineBuilder& blendFactors(
        BlendFactor src,
        BlendFactor dst,
        BlendOperation operation,
        uint32_t index = 0
    );

    PipelineBuilder& blendFactorsAlpha(
        BlendFactor srcAlpha,
        BlendFactor dstAlpha,
        BlendOperation operation,
        uint32_t index = 0
    );

    PipelineBuilder& addInputLayout(
        uint32_t index,
        uint32_t slot,
        uint32_t components,
        GpuType type,
        bool normalized
    );

    PipelineBuilder& addShader(Shader* shader);

    Pipeline create();

private:
    priv::PipelineDesc* m_desc;
    std::vector<InputLayout> m_inputLayouts;
    uint32_t m_numTargets;
};

} // namespace ply