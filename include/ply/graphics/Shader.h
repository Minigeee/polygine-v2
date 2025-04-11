#pragma once

#include <ply\core\Handle.h>
#include <ply\core\Macros.h>
#include <ply\graphics\GpuResource.h>

namespace ply {

namespace priv {
    struct ShaderDesc;
}

class PipelineBuilder;
class ShaderBuilder;

///////////////////////////////////////////////////////////
/// \brief Shader
///
///////////////////////////////////////////////////////////
class Shader : public GpuResource {
    friend PipelineBuilder;
    friend ShaderBuilder;

public:
    enum Type : uint32_t {
        Unknown = 0x0000,       ///< Unknown shader type
        Vertex = 0x0001,        ///< Vertex shader
        Pixel = 0x0002,         ///< Pixel (fragment) shader
        Geometry = 0x0004,      ///< Geometry shader
        Hull = 0x0008,          ///< Hull (tessellation control) shader
        Domain = 0x0010,        ///< Domain (tessellation evaluation) shader
        Compute = 0x0020,       ///< Compute shader
        Amplification = 0x0040, ///< Amplification (task) shader
        Mesh = 0x0080,          ///< Mesh shader
    };

    enum Language { Glsl, Hlsl };

public:
    GPU_RESOURCE(Shader);

    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    Shader(RenderDevice* device);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~Shader();

    ///////////////////////////////////////////////////////////
    /// \brief Load shader from source code
    ///
    /// \param source Source code
    /// \param type Shader type
    /// \return True if the shader was loaded successfully, false otherwise
    ///
    /// \note This function should be called from the main thread.
    ///////////////////////////////////////////////////////////
    bool loadFromSource(const std::string& source, Shader::Type type);

    ///////////////////////////////////////////////////////////
    /// \brief Load shader from a file
    ///
    /// The base path is set by the render device RenderDevice::setShaderPath.
    ///
    /// \param fname File name of the shader
    /// \param type Shader type
    /// \return True if the shader was loaded successfully, false otherwise
    ///
    /// \note This function should be called from the main thread.
    ///////////////////////////////////////////////////////////
    bool loadFromFile(const std::string& fname, Shader::Type type);

    ///////////////////////////////////////////////////////////
    /// \brief Get shader type
    ///
    /// \return Shader type
    ///
    ///////////////////////////////////////////////////////////
    Type getType() const;

private:
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    Shader(
        priv::DeviceImpl* device,
        void* resource,
        Handle handle,
        Shader::Type type
    );

private:
    Shader::Type m_type;
};

BIT_OPERATOR(Shader::Type);

///////////////////////////////////////////////////////////
/// \brief A shader builder
///
///////////////////////////////////////////////////////////
class ShaderBuilder : public GpuResourceBuilder {
public:
    ShaderBuilder(RenderDevice* device);
    ShaderBuilder(priv::DeviceImpl* device);
    ~ShaderBuilder();

    ShaderBuilder(const ShaderBuilder&) = delete;
    ShaderBuilder& operator=(const ShaderBuilder&) = delete;
    ShaderBuilder(ShaderBuilder&&) noexcept;
    ShaderBuilder& operator=(ShaderBuilder&&) noexcept;

    ShaderBuilder& type(Shader::Type type);

    ShaderBuilder& file(const char* fname);

    ShaderBuilder& source(const char* source);

    ShaderBuilder& entryPoint(const char* entryPoint);

    ShaderBuilder& language(Shader::Language language);

    ShaderBuilder& addMacro(const char* name, bool definition);
    ShaderBuilder& addMacro(const char* name, int definition);
    ShaderBuilder& addMacro(const char* name, uint32_t definition);
    ShaderBuilder& addMacro(const char* name, float definition);
    ShaderBuilder& addMacro(const char* name, double definition);
    ShaderBuilder& addMacro(const char* name, const char* definition);
    ShaderBuilder& addMacro(const char* name, const std::string& definition);

    Shader load();

private:
    priv::ShaderDesc* m_desc;
    Shader::Type m_type;
};

} // namespace ply
