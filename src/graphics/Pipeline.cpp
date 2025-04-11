#include <ply/graphics/Pipeline.h>

#include "RenderImpl.h"
#include <ply/core/PoolAllocator.h>
#include <ply/graphics/RenderDevice.h>

#define PIPELINE(x) static_cast<Diligent::IPipelineState*>(x)
#define RESOURCE_BINDING(x) static_cast<Diligent::IShaderResourceBinding*>(x)

namespace ply {

///////////////////////////////////////////////////////////
ResourceBinding::~ResourceBinding() {
    if (m_device && m_resource) {
        m_device->m_resourceBindings.remove(m_handle);
    }
}

///////////////////////////////////////////////////////////
void ResourceBinding::set(
    Shader::Type stages,
    const char* name,
    const Buffer& resource
) {
    CHECK_F(m_device && m_resource, "resource binding not initialized");
    RESOURCE_BINDING(m_resource)
        ->GetVariableByName(static_cast<Diligent::SHADER_TYPE>(stages), name)
        ->Set(static_cast<Diligent::IBuffer*>(resource.getResource()));
}

///////////////////////////////////////////////////////////
Pipeline::~Pipeline() {
    if (m_device && m_resource) {
        m_device->m_pipelines.remove(m_handle);
    }
}

///////////////////////////////////////////////////////////
void Pipeline::setStaticVariable(
    Shader::Type stages,
    const char* name,
    const Buffer& resource
) {
    CHECK_F(m_device && m_resource, "pipeline not initialized");
    PIPELINE(m_resource)
        ->GetStaticVariableByName(
            static_cast<Diligent::SHADER_TYPE>(stages),
            name
        )
        ->Set(static_cast<Diligent::IBuffer*>(resource.getResource()));
}

///////////////////////////////////////////////////////////
ResourceBinding Pipeline::createResourceBinding() {
    CHECK_F(m_device && m_resource, "pipeline not initialized");

    // Create binding
    RefCntAutoPtr<IShaderResourceBinding> binding;
    PIPELINE(m_resource)->CreateShaderResourceBinding(&binding, true);

    // Register
    Handle handle = m_device->m_resourceBindings.push(binding);

    return ResourceBinding(m_device, binding, handle);
}

///////////////////////////////////////////////////////////
PipelineBuilder::PipelineBuilder(RenderDevice* device, PipelineType type) :
    GpuResourceBuilder(device),
    m_desc(nullptr),
    m_numTargets(0) {
    m_desc = Pool<priv::PipelineDesc>::alloc();

    // TEMP
    CHECK_F(type == PipelineType::Graphics, "Compute pipeline not implemented");

    // Set type
    auto& desc = m_desc->PSODesc;
    desc.PipelineType =
        type == PipelineType::Graphics
            ? PIPELINE_TYPE_GRAPHICS
            : PIPELINE_TYPE_COMPUTE;

    // Defaults
    auto& graphics = m_desc->GraphicsPipeline;
    auto swapChain = m_device->m_swapChain;

    graphics.NumRenderTargets = 1;
    graphics.RTVFormats[0] = swapChain->GetDesc().ColorBufferFormat;
    graphics.DSVFormat = swapChain->GetDesc().DepthBufferFormat;
    graphics.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
}

///////////////////////////////////////////////////////////
PipelineBuilder::~PipelineBuilder() {
    if (m_desc) {
        Pool<priv::PipelineDesc>::free(m_desc);
    }
}

///////////////////////////////////////////////////////////
PipelineBuilder::PipelineBuilder(PipelineBuilder&& other) noexcept :
    m_desc(std::exchange(other.m_desc, nullptr)),
    m_numTargets(std::exchange(other.m_numTargets, 0)),
    m_inputLayouts(std::move(other.m_inputLayouts)),
    m_variables(std::move(other.m_variables)),
    m_samplers(std::move(other.m_samplers)) {}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::operator=(PipelineBuilder&& other) noexcept {
    if (this != &other) {
        m_desc = std::exchange(other.m_desc, nullptr);
        m_numTargets = std::exchange(other.m_numTargets, 0);
        m_inputLayouts = std::move(other.m_inputLayouts);
        m_variables = std::move(other.m_variables);
        m_samplers = std::move(other.m_samplers);
    }
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::addTargetFormat(TextureFormat format) {
    auto& graphics = m_desc->GraphicsPipeline;
    if (m_numTargets < MAX_RENDER_TARGETS) { // Maximum of 8 render targets
        graphics.RTVFormats[m_numTargets] = static_cast<TEXTURE_FORMAT>(format);
        m_numTargets++;
        graphics.NumRenderTargets = m_numTargets;
    }
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::depthFormat(TextureFormat format) {
    auto& graphics = m_desc->GraphicsPipeline;
    graphics.DSVFormat = static_cast<TEXTURE_FORMAT>(format);
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::topology(PrimitiveTopology topology) {
    auto& graphics = m_desc->GraphicsPipeline;
    graphics.PrimitiveTopology = static_cast<PRIMITIVE_TOPOLOGY>(topology);
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::fill(FillMode fillMode) {
    auto& rasterizerDesc = m_desc->GraphicsPipeline.RasterizerDesc;
    rasterizerDesc.FillMode = static_cast<FILL_MODE>(fillMode);
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::cull(CullMode cullMode) {
    auto& rasterizerDesc = m_desc->GraphicsPipeline.RasterizerDesc;
    rasterizerDesc.CullMode = static_cast<CULL_MODE>(cullMode);
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::depth(bool enabled) {
    auto& depthStencilDesc = m_desc->GraphicsPipeline.DepthStencilDesc;
    depthStencilDesc.DepthEnable = enabled;
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::depthWrite(bool enabled) {
    auto& depthStencilDesc = m_desc->GraphicsPipeline.DepthStencilDesc;
    depthStencilDesc.DepthWriteEnable = enabled;
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::scissor(bool enabled) {
    auto& rasterizerDesc = m_desc->GraphicsPipeline.RasterizerDesc;
    rasterizerDesc.ScissorEnable = enabled;
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::blend(bool enabled, uint32_t index) {
    auto& blendDesc = m_desc->GraphicsPipeline.BlendDesc;
    blendDesc.RenderTargets[index].BlendEnable = enabled;
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::blendFactors(
    BlendFactor src,
    BlendFactor dst,
    BlendOperation operation,
    uint32_t index
) {
    auto& rt0 = m_desc->GraphicsPipeline.BlendDesc.RenderTargets[index];
    rt0.SrcBlend = static_cast<BLEND_FACTOR>(src);
    rt0.DestBlend = static_cast<BLEND_FACTOR>(dst);
    rt0.BlendOp = static_cast<BLEND_OPERATION>(operation);
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::blendFactorsAlpha(
    BlendFactor srcAlpha,
    BlendFactor dstAlpha,
    BlendOperation operation,
    uint32_t index
) {
    auto& rt0 = m_desc->GraphicsPipeline.BlendDesc.RenderTargets[index];
    rt0.SrcBlendAlpha = static_cast<BLEND_FACTOR>(srcAlpha);
    rt0.DestBlendAlpha = static_cast<BLEND_FACTOR>(dstAlpha);
    rt0.BlendOpAlpha = static_cast<BLEND_OPERATION>(operation);
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::addInputLayout(
    uint32_t index,
    uint32_t slot,
    uint32_t components,
    Type type,
    bool normalized
) {
    InputLayout layout;
    layout.index = index;
    layout.slot = slot;
    layout.components = components;
    layout.type = type;
    layout.normalized = normalized;
    m_inputLayouts.push_back(layout);

    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::shader(Shader* shader) {
    Shader::Type type = shader->getType();
    IShader* impl = static_cast<IShader*>(shader->m_resource);

    // Set shader
    switch (type) {
    case Shader::Type::Vertex:
        m_desc->pVS = impl;
        break;
    case Shader::Type::Geometry:
        m_desc->pGS = impl;
        break;
    case Shader::Type::Pixel:
        m_desc->pPS = impl;
        break;
    default:
        CHECK_F(false, "invalid shader type");
        break;
    }

    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::addVariable(
    const std::string& name,
    Shader::Type stages,
    ShaderResourceType type
) {
    ShaderVariableDesc desc;
    desc.name = name;
    desc.stages = stages;
    desc.type = type;
    m_variables.push_back(desc);
    return *this;
}

///////////////////////////////////////////////////////////
PipelineBuilder& PipelineBuilder::addSampler(
    const std::string& name,
    Shader::Type stages,
    TextureFilter filter,
    TextureAddress address
) {
    ShaderSamplerDesc desc;
    desc.name = name;
    desc.stages = stages;
    desc.filter = filter;
    desc.address = address;
    m_samplers.push_back(desc);
    return *this;
}

///////////////////////////////////////////////////////////
Pipeline PipelineBuilder::create() {
    // Set up input layout
    auto& inputLayout = m_desc->GraphicsPipeline.InputLayout;

    // Create layout elements from stored input layouts
    std::vector<LayoutElement> layoutElements;
    for (const auto& layout : m_inputLayouts) {
        VALUE_TYPE valueType;

        // Map type to Diligent Engine VALUE_TYPE
        switch (layout.type) {
        case Type::Int8:
            valueType = VT_INT8;
            break;
        case Type::Int16:
            valueType = VT_INT16;
            break;
        case Type::Int32:
            valueType = VT_INT32;
            break;
        case Type::Uint8:
            valueType = VT_UINT8;
            break;
        case Type::Uint16:
            valueType = VT_UINT16;
            break;
        case Type::Uint32:
            valueType = VT_UINT32;
            break;
        case Type::Float16:
            valueType = VT_FLOAT16;
            break;
        case Type::Float32:
            valueType = VT_FLOAT32;
            break;
        case Type::Float64:
            valueType = VT_FLOAT64;
            break;
        default:
            valueType = VT_UNDEFINED;
            break;
        }

        layoutElements.emplace_back(
            layout.index,
            layout.slot,
            layout.components,
            valueType,
            layout.normalized
        );
    }

    // Set layout elements
    if (!layoutElements.empty()) {
        inputLayout.LayoutElements = layoutElements.data();
        inputLayout.NumElements = static_cast<uint32_t>(layoutElements.size());
    }

    // Set variables
    std::vector<Diligent::ShaderResourceVariableDesc> variables;
    for (const auto& variable : m_variables) {
        Diligent::ShaderResourceVariableDesc desc;
        desc.Name = variable.name.c_str();
        desc.ShaderStages = static_cast<SHADER_TYPE>(variable.stages);
        desc.Type = static_cast<SHADER_RESOURCE_VARIABLE_TYPE>(variable.type);
        variables.emplace_back(desc);
    }

    if (!variables.empty()) {
        m_desc->PSODesc.ResourceLayout.Variables = variables.data();
        m_desc->PSODesc.ResourceLayout.NumVariables =
            static_cast<uint32_t>(variables.size());
    }

    // Set immutable samplers
    std::vector<Diligent::ImmutableSamplerDesc> immutableSamplers;
    for (const auto& sampler : m_samplers) {
        Diligent::ImmutableSamplerDesc desc;
        desc.SamplerOrTextureName = sampler.name.c_str();
        desc.ShaderStages = static_cast<SHADER_TYPE>(sampler.stages);
        desc.Desc.MinFilter = static_cast<FILTER_TYPE>(sampler.filter);
        desc.Desc.MagFilter = static_cast<FILTER_TYPE>(sampler.filter);
        desc.Desc.MipFilter = static_cast<FILTER_TYPE>(sampler.filter);
        desc.Desc.AddressU = static_cast<TEXTURE_ADDRESS_MODE>(sampler.address);
        desc.Desc.AddressV = static_cast<TEXTURE_ADDRESS_MODE>(sampler.address);
        desc.Desc.AddressW = static_cast<TEXTURE_ADDRESS_MODE>(sampler.address);
        immutableSamplers.emplace_back(desc);
    }

    if (!immutableSamplers.empty()) {
        m_desc->PSODesc.ResourceLayout.ImmutableSamplers =
            immutableSamplers.data();
        m_desc->PSODesc.ResourceLayout.NumImmutableSamplers =
            static_cast<uint32_t>(immutableSamplers.size());
    }

    // Set render target write mask for all render targets
    auto& blendDesc = m_desc->GraphicsPipeline.BlendDesc;
    for (uint32_t i = 0; i < m_numTargets; ++i) {
        blendDesc.RenderTargets[i].RenderTargetWriteMask = COLOR_MASK_ALL;
    }

    // Create the pipeline state object
    RefCntAutoPtr<IPipelineState> pipelineState;
    m_device->m_renderDevice->CreateGraphicsPipelineState(
        *m_desc,
        &pipelineState
    );

    // Add to device
    Handle handle = m_device->m_pipelines.push(pipelineState);

    // Create and return the Pipeline object
    return Pipeline(m_device, pipelineState.RawPtr(), handle);
}

} // namespace ply