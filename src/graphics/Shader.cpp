#include <ply/graphics/Shader.h>

#include "RenderImpl.h"
#include <ply/core/PoolAllocator.h>
#include <ply/graphics/RenderDevice.h>

namespace ply {

///////////////////////////////////////////////////////////
Shader::Shader(
    priv::DeviceImpl* device,
    void* resource,
    Handle handle,
    Shader::Type type
) :
    GpuResource(device, resource, handle),
    m_type(type) {}

///////////////////////////////////////////////////////////
Shader::Shader(RenderDevice* device) :
    GpuResource(device) {}

///////////////////////////////////////////////////////////
Shader::~Shader() {
    if (m_device && m_resource) {
        m_device->m_shaders.remove(m_handle);
    }
}

///////////////////////////////////////////////////////////
Shader::Type Shader::getType() const {
    return m_type;
}

///////////////////////////////////////////////////////////
bool Shader::loadFromSource(const std::string& source, Shader::Type type) {
    ShaderBuilder builder(m_device);
    builder.type(type).source(source.c_str());
    *this = builder.load();
    return true;
}

///////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& fname, Shader::Type type) {
    ShaderBuilder builder(m_device);
    builder.type(type).file(fname.c_str());
    *this = builder.load();
    return true;
}

///////////////////////////////////////////////////////////
ShaderBuilder::ShaderBuilder(RenderDevice* device) :
    ShaderBuilder(device->m_device) {}

///////////////////////////////////////////////////////////
ShaderBuilder::ShaderBuilder(priv::DeviceImpl* device) :
    GpuResourceBuilder(device) {
    m_desc = Pool<priv::ShaderDesc>::alloc();

    // Set shader factory
    m_desc->pShaderSourceStreamFactory = m_device->m_shaderFactory;

    // Defaults
    m_desc->EntryPoint = "main";
    m_desc->SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    m_desc->Desc.UseCombinedTextureSamplers = true;

    // idk why but this seems to be necessary (at least with vulkan + glm)
    m_desc->CompileFlags = SHADER_COMPILE_FLAG_PACK_MATRIX_ROW_MAJOR;
}

///////////////////////////////////////////////////////////
ShaderBuilder::~ShaderBuilder() {
    if (m_desc) {
        Pool<priv::ShaderDesc>::free(m_desc);
    }
}

///////////////////////////////////////////////////////////
ShaderBuilder::ShaderBuilder(ShaderBuilder&& other) noexcept {
    m_desc = other.m_desc;
    other.m_desc = nullptr;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::operator=(ShaderBuilder&& other) noexcept {
    if (this != &other) {
        m_desc = other.m_desc;
        other.m_desc = nullptr;
    }
    return *this;
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
ShaderBuilder& ShaderBuilder::file(const char* fname) {
    m_desc->FilePath = fname;
    m_desc->Source = nullptr;
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::source(const char* source) {
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
    if (language == Shader::Language::Hlsl) {
        m_desc->SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    } else {
        m_desc->SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL;
        // Enable includes
        m_desc->GLSLExtensions = "#extension GL_ARB_shading_language_include : enable\n";
    }
    
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, bool definition) {
    m_desc->MacroList.push_back({name, definition ? "1" : "0"});
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, int definition) {
    m_desc->MacroList.push_back({name, std::to_string(definition)});
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, uint32_t definition) {
    m_desc->MacroList.push_back({name, std::to_string(definition)});
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, float definition) {
    m_desc->MacroList.push_back({name, std::to_string(definition)});
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder& ShaderBuilder::addMacro(const char* name, double definition) {
    m_desc->MacroList.push_back({name, std::to_string(definition)});
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder&
ShaderBuilder::addMacro(const char* name, const char* definition) {
    m_desc->MacroList.push_back({name, definition});
    return *this;
}

///////////////////////////////////////////////////////////
ShaderBuilder&
ShaderBuilder::addMacro(const char* name, const std::string& definition) {
    m_desc->MacroList.push_back({name, definition});
    return *this;
}

///////////////////////////////////////////////////////////
Shader ShaderBuilder::load() {
    // Finalize macros
    std::vector<ShaderMacro> macros;
    macros.reserve(m_desc->MacroList.size());
    for (const auto& macro : m_desc->MacroList) {
        macros.push_back({macro.first.c_str(), macro.second.c_str()});
    }
    m_desc->Macros.Count = (uint32_t)macros.size();
    m_desc->Macros.Elements = macros.data();

    // Create shader
    RefCntAutoPtr<IShader> shader;
    m_device->m_renderDevice->CreateShader(*m_desc, &shader);

    // Add to device
    Handle handle = m_device->m_shaders.push(shader);

    return Shader(m_device, shader, handle, m_type);
}

} // namespace ply
