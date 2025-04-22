#pragma once

#include <ply\core\Handle.h>
#include <ply\core\Macros.h>
#include <ply\graphics\GpuResource.h>

#include <memory>

namespace ply {

namespace priv {
    struct ShaderDesc;
}

class PipelineBuilder;
class ShaderBuilder;

///////////////////////////////////////////////////////////
/// \brief Shader
///
/// The Shader class represents a GPU shader object, such as a vertex,
/// pixel (fragment), geometry, or compute shader. Shaders are a core
/// part of the graphics pipeline and are used to process vertices,
/// fragments, and perform general-purpose computations on the GPU.
///
/// Shaders can be loaded from source code or from files, and are
/// typically created using the ShaderBuilder for flexibility and
/// convenience.
///
/// Usage example:
/// \code
/// // Load a vertex shader from file
/// ply::ShaderBuilder builder(device);
/// builder.type(ply::Shader::Vertex).file("shader.vsh");
/// ply::Shader vertexShader = builder.load();
/// \endcode
///
///////////////////////////////////////////////////////////
class Shader : public GpuResource {
    friend PipelineBuilder;
    friend ShaderBuilder;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Shader type enumeration
    ///
    /// Specifies the type of shader (vertex, pixel, geometry, etc.).
    ///////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////
    /// \brief Shader language enumeration
    ///
    /// Specifies the shading language (GLSL or HLSL).
    ///////////////////////////////////////////////////////////
    enum Language { Glsl, Hlsl };

public:
    GPU_RESOURCE(Shader);

    ///////////////////////////////////////////////////////////
    /// \brief Construct a shader object for a render device
    ///
    /// \param device The render device to associate with this shader
    ///////////////////////////////////////////////////////////
    Shader(RenderDevice* device);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Releases the GPU shader resource.
    ///////////////////////////////////////////////////////////
    ~Shader();

    ///////////////////////////////////////////////////////////
    /// \brief Load shader from source code
    ///
    /// Loads and compiles a shader from a string containing the shader source.
    ///
    /// \param source Source code string
    /// \param type Shader type (vertex, pixel, etc.)
    /// \return True if the shader was loaded successfully, false otherwise
    ///
    /// \note This function should be called from the main thread.
    ///////////////////////////////////////////////////////////
    bool loadFromSource(const std::string& source, Shader::Type type);

    ///////////////////////////////////////////////////////////
    /// \brief Load shader from a file
    ///
    /// Loads and compiles a shader from a file. The base path is set by the
    /// render device (RenderDevice::setShaderPath).
    ///
    /// \param fname File name of the shader
    /// \param type Shader type (vertex, pixel, etc.)
    /// \return True if the shader was loaded successfully, false otherwise
    ///
    /// \note This function should be called from the main thread.
    ///////////////////////////////////////////////////////////
    bool loadFromFile(const std::string& fname, Shader::Type type);

    ///////////////////////////////////////////////////////////
    /// \brief Get shader type
    ///
    /// \return The type of this shader (vertex, pixel, etc.)
    ///////////////////////////////////////////////////////////
    Type getType() const;

private:
    ///////////////////////////////////////////////////////////
    /// \brief Internal constructor for advanced usage
    ///
    /// Used internally by builders and the render device.
    ///////////////////////////////////////////////////////////
    Shader(
        priv::DeviceImpl* device,
        void* resource,
        Handle handle,
        Shader::Type type
    );

private:
    Shader::Type m_type; //!< Shader type
};

BIT_OPERATOR(Shader::Type);

///////////////////////////////////////////////////////////
/// \brief A shader builder
///
/// The ShaderBuilder provides a fluent interface for configuring and
/// creating Shader objects. It allows you to specify the shader type,
/// source or file, entry point, language, and preprocessor macros.
///
/// Usage example:
/// \code
/// // Create a pixel shader from file with a macro
/// ply::Shader shader = device.shader()
///     .type(ply::Shader::Pixel)
///     .file("shader.psh")
///     .addMacro("USE_SHADOWS", true)
///     .load();
/// \endcode
///
///////////////////////////////////////////////////////////
class ShaderBuilder : public GpuResourceBuilder {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Construct a shader builder for a render device
    ///
    /// \param device The render device pointer
    ///////////////////////////////////////////////////////////
    ShaderBuilder(RenderDevice* device);
    ///////////////////////////////////////////////////////////
    /// \brief Construct a shader builder for a device implementation
    ///
    /// \param device The device implementation pointer
    ///////////////////////////////////////////////////////////
    ShaderBuilder(priv::DeviceImpl* device);
    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///////////////////////////////////////////////////////////
    ~ShaderBuilder();

    ///////////////////////////////////////////////////////////
    /// \brief Set the resource name for debugging purposes.
    /// \param name Resource name for debugging.
    ///
    ///////////////////////////////////////////////////////////
    ShaderBuilder& name(const char* name);

    ///////////////////////////////////////////////////////////
    /// \brief Set the shader type
    ///
    /// \param type Shader type (vertex, pixel, etc.)
    /// \return Reference to this builder
    ///////////////////////////////////////////////////////////
    ShaderBuilder& type(Shader::Type type);

    ///////////////////////////////////////////////////////////
    /// \brief Set the shader file to load
    ///
    /// \param fname File name of the shader
    /// \return Reference to this builder
    ///////////////////////////////////////////////////////////
    ShaderBuilder& file(const char* fname);

    ///////////////////////////////////////////////////////////
    /// \brief Set the shader source code
    ///
    /// \param source Source code string
    /// \return Reference to this builder
    ///////////////////////////////////////////////////////////
    ShaderBuilder& source(const char* source);

    ///////////////////////////////////////////////////////////
    /// \brief Set the shader entry point
    ///
    /// \param entryPoint Name of the entry point function
    /// \return Reference to this builder
    ///////////////////////////////////////////////////////////
    ShaderBuilder& entryPoint(const char* entryPoint);

    ///////////////////////////////////////////////////////////
    /// \brief Set the shader language
    ///
    /// \param language Shader language (GLSL or HLSL)
    /// \return Reference to this builder
    ///////////////////////////////////////////////////////////
    ShaderBuilder& language(Shader::Language language);

    ///////////////////////////////////////////////////////////
    /// \brief Add a preprocessor macro definition
    ///
    /// \param name Macro name
    /// \param definition Macro value (various overloads)
    /// \return Reference to this builder
    ///////////////////////////////////////////////////////////
    ShaderBuilder& addMacro(const char* name, bool definition);
    ShaderBuilder& addMacro(const char* name, int definition);
    ShaderBuilder& addMacro(const char* name, uint32_t definition);
    ShaderBuilder& addMacro(const char* name, float definition);
    ShaderBuilder& addMacro(const char* name, double definition);
    ShaderBuilder& addMacro(const char* name, const char* definition);
    ShaderBuilder& addMacro(const char* name, const std::string& definition);

    ///////////////////////////////////////////////////////////
    /// \brief Load and create the shader
    ///
    /// Compiles and creates the shader object using the parameters
    /// specified in the builder.
    ///
    /// \return The created Shader object
    ///////////////////////////////////////////////////////////
    Shader load();

private:
    std::unique_ptr<priv::ShaderDesc> m_desc;
    Shader::Type m_type;
};

} // namespace ply
