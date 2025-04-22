#pragma once

#include <ply/core/Handle.h>
#include <ply/graphics/GpuResource.h>
#include <ply/graphics/Shader.h>
#include <ply/graphics/Types.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace ply {

class RenderDevice;
class Buffer;
class Texture;
class Framebuffer;
class RenderPass;

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
    bool instance;
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
    /// \brief Destructor. Releases the resource binding.
    ///
    ///////////////////////////////////////////////////////////
    ~ResourceBinding();

    ///////////////////////////////////////////////////////////
    /// \brief Set a mutable or dynamic buffer variable for a shader stage.
    /// \param stages Shader stages to which the variable belongs.
    /// \param name Name of the variable.
    /// \param resource Buffer object to bind to the variable.
    /// \note Mutable resources can only be set once per resource binding.
    /// Dynamic resources can be set multiple times.
    ///
    ///////////////////////////////////////////////////////////
    void set(Shader::Type stages, const char* name, const Buffer& resource);

    ///////////////////////////////////////////////////////////
    /// \brief Set a mutable or dynamic texture variable for a shader stage.
    /// \param stages Shader stages to which the variable belongs.
    /// \param name Name of the variable.
    /// \param resource Texture object to bind to the variable.
    /// \note Mutable resources can only be set once per resource binding.
    /// Dynamic resources can be set multiple times.
    ///
    ///////////////////////////////////////////////////////////
    void set(Shader::Type stages, const char* name, const Texture& resource);
    
    ///////////////////////////////////////////////////////////
    /// \brief Set the buffer offset for a buffer variable.
    /// \param stages Shader stages to which the variable belongs.
    /// \param name Name of the variable.
    /// \param offset Offset in bytes to set for the buffer variable (in bytes)
    ///
    ///////////////////////////////////////////////////////////
    void setOffset(
        Shader::Type stages,
        const char* name,
        uint32_t offset
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
    /// \brief Destructor. Releases the pipeline resource.
    ///
    ///////////////////////////////////////////////////////////
    ~Pipeline();

    ///////////////////////////////////////////////////////////
    /// \brief Set a static buffer variable for a shader stage.
    /// \param stages Shader stages to which the variable belongs.
    /// \param name Name of the variable.
    /// \param resource Buffer object to bind to the variable.
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
    /// \brief Set a static texture variable for a shader stage.
    /// \param stages Shader stages to which the variable belongs.
    /// \param name Name of the variable.
    /// \param resource Texture object to bind to the variable.
    /// \remarks This method is used to set static variables, which are bound
    /// once and cannot be changed later.
    ///
    ///////////////////////////////////////////////////////////
    void setStaticVariable(
        Shader::Type stages,
        const char* name,
        const Texture& resource
    );

    ///////////////////////////////////////////////////////////
    /// \brief Create a resource binding object for this pipeline.
    /// \return A new ResourceBinding instance.
    ///
    /// Resource binding objects are needed to set shader resources.
    /// Bindings should contain a resource for every mutable and dynamic
    /// variable in the pipeline. Static variables are set directly on the
    /// pipeline object and do not require a binding.
    ///
    ///////////////////////////////////////////////////////////
    ResourceBinding createResourceBinding();
};

///////////////////////////////////////////////////////////
/// \brief A gpu pipeline object builder
///
///////////////////////////////////////////////////////////
class PipelineBuilder : public GpuResourceBuilder {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Construct a pipeline builder for a render device.
    /// \param device The render device pointer.
    /// \param type Pipeline type (Graphics or Compute).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder(
        RenderDevice* device,
        PipelineType type = PipelineType::Graphics
    );

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~PipelineBuilder();

    ///////////////////////////////////////////////////////////
    /// \brief Set the resource name for debugging purposes.
    /// \param name Resource name for debugging.
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& name(const char* name);

    ///////////////////////////////////////////////////////////
    /// \brief Set the framebuffer target format for the pipeline.
    /// \param target The framebuffer to use as the render target.
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& targetFormat(const Framebuffer& target);

    ///////////////////////////////////////////////////////////
    /// \brief Set the framebuffer render pass target for the pipeline.
    /// \param pass The render pass this pipeline should render to.
    /// \param subpass The subpass index within the render pass (default 0).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& renderPass(const RenderPass& pass, uint32_t subpass = 0);

    ///////////////////////////////////////////////////////////
    /// \brief Set the primitive topology for the pipeline.
    /// \param topology Primitive topology (e.g., TriangleList).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& topology(PrimitiveTopology topology);

    ///////////////////////////////////////////////////////////
    /// \brief Set the fill mode for rasterization.
    /// \param fillMode Fill mode (e.g., Solid, Wireframe).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& fill(FillMode fillMode);

    ///////////////////////////////////////////////////////////
    /// \brief Set the cull mode for rasterization.
    /// \param cullMode Cull mode (e.g., Back, Front).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& cull(CullMode cullMode);

    ///////////////////////////////////////////////////////////
    /// \brief Enable or disable depth testing.
    /// \param enabled True to enable depth testing.
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& depth(bool enabled);

    ///////////////////////////////////////////////////////////
    /// \brief Enable or disable depth writing.
    /// \param enabled True to enable depth writing.
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& depthWrite(bool enabled);

    ///////////////////////////////////////////////////////////
    /// \brief Enable or disable scissor testing.
    /// \param enabled True to enable scissor test.
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& scissor(bool enabled);

    ///////////////////////////////////////////////////////////
    /// \brief Enable or disable blending for a render target.
    /// \param enabled True to enable blending.
    /// \param index Render target index (default 0).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& blend(bool enabled, uint32_t index = 0);

    ///////////////////////////////////////////////////////////
    /// \brief Set blend factors and operation for a render target.
    /// \param src Source blend factor.
    /// \param dst Destination blend factor.
    /// \param operation Blend operation.
    /// \param index Render target index (default 0).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& blendFactors(
        BlendFactor src,
        BlendFactor dst,
        BlendOperation operation,
        uint32_t index = 0
    );

    ///////////////////////////////////////////////////////////
    /// \brief Set alpha blend factors and operation for a render target.
    /// \param srcAlpha Source alpha blend factor.
    /// \param dstAlpha Destination alpha blend factor.
    /// \param operation Blend operation.
    /// \param index Render target index (default 0).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& blendFactorsAlpha(
        BlendFactor srcAlpha,
        BlendFactor dstAlpha,
        BlendOperation operation,
        uint32_t index = 0
    );

    ///////////////////////////////////////////////////////////
    /// \brief Add an input layout element for vertex attributes.
    /// \param index Input index.
    /// \param slot Buffer slot.
    /// \param components Number of components.
    /// \param type Data type.
    /// \param instance True if per-instance data (default false).
    /// \param normalized True if data should be normalized (default false).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& addInputLayout(
        uint32_t index,
        uint32_t slot,
        uint32_t components,
        Type type,
        bool instance = false,
        bool normalized = false
    );

    ///////////////////////////////////////////////////////////
    /// \brief Attach a shader to the pipeline.
    /// \param shader Pointer to the shader object.
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& shader(Shader* shader);

    ///////////////////////////////////////////////////////////
    /// \brief Add a shader variable to the pipeline.
    /// \param name Name of the variable.
    /// \param stages Shader stages to which the variable belongs.
    /// \param type Resource binding type (Static, Mutable, Dynamic).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& addVariable(
        const std::string& name,
        Shader::Type stages,
        ShaderResourceType type
    );

    ///////////////////////////////////////////////////////////
    /// \brief Add a sampler to the pipeline.
    /// \param name Name of the sampler.
    /// \param stages Shader stages to which the sampler belongs.
    /// \param filter Texture filter type (default Linear).
    /// \param address Texture address mode (default Clamp).
    ///
    ///////////////////////////////////////////////////////////
    PipelineBuilder& addSampler(
        const std::string& name,
        Shader::Type stages,
        TextureFilter filter = TextureFilter::Linear,
        TextureAddress address = TextureAddress::Clamp
    );

    ///////////////////////////////////////////////////////////
    /// \brief Create the pipeline resource with the specified parameters.
    /// \return The created Pipeline object.
    ///
    ///////////////////////////////////////////////////////////
    Pipeline create();

private:
    std::unique_ptr<priv::PipelineDesc> m_desc;
    std::vector<InputLayout> m_inputLayouts;
    std::vector<ShaderVariableDesc> m_variables;
    std::vector<ShaderSamplerDesc> m_samplers;
    uint32_t m_numTargets;
};

} // namespace ply