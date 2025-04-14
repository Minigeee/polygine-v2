#pragma once

#include <ply/core/Handle.h>
#include <ply/graphics/GpuResource.h>
#include <ply/graphics/Shader.h>
#include <ply/graphics/Types.h>

#include <cstdint>
#include <vector>

namespace ply {

class RenderDevice;
class Buffer;
class Texture;

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
    Type type;
    bool normalized;
};

///////////////////////////////////////////////////////////
/// \brief Shader variable descriptor
///
///////////////////////////////////////////////////////////
struct ShaderVariableDesc {
    std::string name;
    Shader::Type stages;
    ShaderResourceType type;
};

///////////////////////////////////////////////////////////
/// \brief Shader sampler descriptor
///
///////////////////////////////////////////////////////////
struct ShaderSamplerDesc {
    std::string name;
    Shader::Type stages;
    TextureFilter filter;
    TextureAddress address;
};

///////////////////////////////////////////////////////////
/// \brief Shader resource binding
///
///////////////////////////////////////////////////////////
class ResourceBinding : public GpuResource {
public:
    GPU_RESOURCE(ResourceBinding);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~ResourceBinding();

    ///////////////////////////////////////////////////////////
    /// \brief Set a mutable or dynamic variable
    ///
    /// Mutable resources can only be set once per resource binding.
    /// Dynamic resources can be set multiple times.
    ///
    /// \param stages - shader stages to which the variable belongs
    /// \param name - name of the variable
    /// \param resource - buffer object to bind to the variable
    ///
    ///////////////////////////////////////////////////////////
    void set(
        Shader::Type stages,
        const char* name,
        const Buffer& resource
    );

    ///////////////////////////////////////////////////////////
    /// \brief Set a mutable or dynamic variable
    ///
    /// Mutable resources can only be set once per resource binding.
    /// Dynamic resources can be set multiple times.
    ///
    /// \param stages - shader stages to which the variable belongs
    /// \param name - name of the variable
    /// \param resource - texture object to bind to the variable
    ///
    ///////////////////////////////////////////////////////////
    void set(
        Shader::Type stages,
        const char* name,
        const Texture& resource
    );
};

///////////////////////////////////////////////////////////
/// \brief A gpu pipeline object
///////////////////////////////////////////////////////////
class Pipeline : public GpuResource {
    friend RenderDevice;

public:
    GPU_RESOURCE(Pipeline);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~Pipeline();

    ///////////////////////////////////////////////////////////
    /// \brief Set a static variable
    ///
    /// \param stages - shader stages to which the variable belongs
    /// \param name - name of the variable
    /// \param resource - buffer object to bind to the variable
    ///
    /// \remarks This method is used to set static variables, which are bound
    /// once and cannot be changed later.
    ///
    ///////////////////////////////////////////////////////////
    void setStaticVariable(
        Shader::Type stages,
        const char* name,
        const Buffer& resource
    );

    ///////////////////////////////////////////////////////////
    /// \brief Set a static variable
    ///
    /// \param stages - shader stages to which the variable belongs
    /// \param name - name of the variable
    /// \param resource - texture object to bind to the variable
    ///
    /// \remarks This method is used to set static variables, which are bound
    /// once and cannot be changed later.
    ///
    ///////////////////////////////////////////////////////////
    void setStaticVariable(
        Shader::Type stages,
        const char* name,
        const Texture& resource
    );

    ResourceBinding createResourceBinding();
};

///////////////////////////////////////////////////////////
/// \brief A gpu pipeline object builder
///
///////////////////////////////////////////////////////////
class PipelineBuilder : public GpuResourceBuilder {
public:
    PipelineBuilder(
        RenderDevice* device,
        PipelineType type = PipelineType::Graphics
    );
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
        Type type,
        bool normalized = false
    );

    PipelineBuilder& shader(Shader* shader);

    PipelineBuilder& addVariable(
        const std::string& name,
        Shader::Type stages,
        ShaderResourceType type
    );

    PipelineBuilder& addSampler(
        const std::string& name,
        Shader::Type stages,
        TextureFilter filter = TextureFilter::Linear,
        TextureAddress address = TextureAddress::Clamp
    );

    Pipeline create();

private:
    priv::PipelineDesc* m_desc;
    std::vector<InputLayout> m_inputLayouts;
    std::vector<ShaderVariableDesc> m_variables;
    std::vector<ShaderSamplerDesc> m_samplers;
    uint32_t m_numTargets;
};

} // namespace ply