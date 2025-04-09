#include <ply/graphics/Shader.h>

#include "RenderImpl.h"
#include <ply/core/PoolAllocator.h>

namespace ply {

///////////////////////////////////////////////////////////
Shader::Shader() :
    GpuResource(),
    m_shader(nullptr) {}

///////////////////////////////////////////////////////////
Shader::Shader(priv::DeviceImpl* device, Handle shader, Shader::Type type) :
    GpuResource(device),
    m_handle(shader),
    m_shader(nullptr) {
    m_shader = m_device->m_shaders[shader];
    m_type = type;
}

///////////////////////////////////////////////////////////
Shader::Type Shader::getType() const {
    return m_type;
}

///////////////////////////////////////////////////////////
Shader::~Shader() {}

///////////////////////////////////////////////////////////
ShaderBuilder::ShaderBuilder(RenderDevice* device) :
    GpuResource(device) {
    m_desc = Pool<priv::ShaderDesc>::alloc();

    // Set shader factory
    m_desc->pShaderSourceStreamFactory = m_device->m_shaderFactory;

    // Defaults
    m_desc->EntryPoint = "main";
    m_desc->SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
}

///////////////////////////////////////////////////////////
ShaderBuilder::~ShaderBuilder() {
    if (m_desc) {
        Pool<priv::ShaderDesc>::free(m_desc);
    }
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::type(Shader::Type type) {
    auto& desc = m_desc->Desc;
    m_type = type;

    switch (type) {
    case Shader::Type::Vertex:
        desc.ShaderType = SHADER_TYPE_VERTEX;
        break;
    case Shader::Type::Geometry:
        desc.ShaderType = SHADER_TYPE_GEOMETRY;
        break;
    case Shader::Type::Pixel:
        desc.ShaderType = SHADER_TYPE_PIXEL;
        break;
    }

    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::fromFile(const char* fname) {
    m_desc->FilePath = fname;
    m_desc->Source = nullptr;
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::fromSource(const char* source) {
    m_desc->Source = source;
    m_desc->FilePath = nullptr;
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::entryPoint(const char* entryPoint) {
    m_desc->EntryPoint = entryPoint;
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::language(Shader::Language language) {
    m_desc->SourceLanguage =
        language == Shader::Language::Hlsl
            ? SHADER_SOURCE_LANGUAGE_HLSL
            : SHADER_SOURCE_LANGUAGE_GLSL;
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, bool definition) {
    // m_desc->MacroHelper.AddShaderMacro(name, definition);
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, int definition) {
    // m_desc->MacroHelper.AddShaderMacro(name, definition);
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, uint32_t definition) {
    // m_desc->MacroHelper.AddShaderMacro(name, definition);
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, float definition) {
    // m_desc->MacroHelper.AddShaderMacro(name, definition);
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, double definition) {
    // m_desc->MacroHelper.AddShaderMacro(name, definition);
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, const char* definition) {
    // m_desc->MacroHelper.AddShaderMacro(name, definition);
    return *this;
}

///////////////////////////////////////////////////////////
Shader ShaderBuilder::load() {
    // Finalize macros
    m_desc->Macros = m_desc->MacroHelper;

    // Create shader
    RefCntAutoPtr<IShader> shader;
    m_device->m_renderDevice->CreateShader(*m_desc, &shader);

    // Add to device
    Handle handle = m_device->m_shaders.push(shader);

    return Shader(m_device, handle, m_type);
}

} // namespace ply
