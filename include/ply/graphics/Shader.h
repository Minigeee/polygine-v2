#pragma once

#include <ply\core\Handle.h>
#include <ply\graphics\GpuResource.h>

namespace ply {

namespace priv {
    struct ShaderDesc;
}

class PipelineBuilder;

///////////////////////////////////////////////////////////
/// \brief Shader
///
///////////////////////////////////////////////////////////
class Shader : public GpuResource {
    friend PipelineBuilder;

public:
    enum Type { Vertex, Geometry, Pixel };
    enum Language { Glsl, Hlsl };

public:
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    Shader();

    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    Shader(priv::DeviceImpl* device, Handle shader, Shader::Type type);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~Shader();

    GPU_RESOURCE(Shader, m_shader);
    
    ///////////////////////////////////////////////////////////
    /// \brief Get shader type
    ///
    /// \return Shader type
    ///
    ///////////////////////////////////////////////////////////
    Type getType() const;

private:
    Handle m_handle;
    void* m_shader;
    Shader::Type m_type;
};

///////////////////////////////////////////////////////////
/// \brief A shader builder
///
///////////////////////////////////////////////////////////
class ShaderBuilder : public GpuResource {
public:
    ShaderBuilder(RenderDevice* device);
    ~ShaderBuilder();

    // Ctor and assignment
    GPU_RESOURCE(ShaderBuilder, m_desc);

    ShaderBuilder& type(Shader::Type type);

    ShaderBuilder& fromFile(const char* fname);

    ShaderBuilder& fromSource(const char* source);

    ShaderBuilder& entryPoint(const char* entryPoint);

    ShaderBuilder& language(Shader::Language language);

    ShaderBuilder& addMacro(const char* name, bool definition);
    ShaderBuilder& addMacro(const char* name, int definition);
    ShaderBuilder& addMacro(const char* name, uint32_t definition);
    ShaderBuilder& addMacro(const char* name, float definition);
    ShaderBuilder& addMacro(const char* name, double definition);
    ShaderBuilder& addMacro(const char* name, const char* definition);

    Shader load();

private:
    priv::ShaderDesc* m_desc;
    Shader::Type m_type;
};

} // namespace ply
