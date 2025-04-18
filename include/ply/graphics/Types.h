#pragma once

#include <cstdint>
#include <ply/core/Macros.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief Texture formats
///
/// From Diligent Engine enum
///
///////////////////////////////////////////////////////////
enum class TextureFormat : uint16_t {
    /// Unknown format
    Unknown = 0,

    /// Four-component 128-bit floating-point format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32B32A32_FLOAT. OpenGL counterpart:
    /// GL_RGBA32F.
    Rgba32f,

    /// Four-component 128-bit unsigned-integer format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32B32A32_UINT. OpenGL counterpart:
    /// GL_RGBA32UI.
    Rgba32ui,

    /// Four-component 128-bit signed-integer format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32B32A32_SINT. OpenGL counterpart:
    /// GL_RGBA32I.
    Rgba32i,

    /// Three-component 96-bit floating-point format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32B32_FLOAT. OpenGL counterpart:
    /// GL_RGB32F.
    /// \warning This format has weak hardware support and is not recommended
    Rgb32f,

    /// Three-component 96-bit unsigned-integer format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32B32_UINT. OpenGL counterpart:
    /// GL_RGB32UI.
    /// \warning This format has weak hardware support and is not recommended
    Rgb32ui,

    /// Three-component 96-bit signed-integer format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32B32_SINT. OpenGL counterpart:
    /// GL_RGB32I.
    /// \warning This format has weak hardware support and is not recommended
    Rgb32i,

    /// Four-component 64-bit half-precision floating-point format with 16-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_FLOAT. OpenGL counterpart:
    /// GL_RGBA16F.
    Rgba16f,

    /// Four-component 64-bit unsigned-normalized-integer format with 16-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_UNORM. OpenGL counterpart:
    /// GL_RGBA16. \n
    /// [GL_EXT_texture_norm16]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
    /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
    Rgba16,

    /// Four-component 64-bit unsigned-integer format with 16-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_UINT. OpenGL counterpart:
    /// GL_RGBA16UI.
    Rgba16ui,

    /// [GL_EXT_texture_norm16]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
    /// Four-component 64-bit signed-normalized-integer format with 16-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_SNORM. OpenGL counterpart:
    /// GL_RGBA16_SNORM. \n
    /// [GL_EXT_texture_norm16]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
    /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
    Rgba16snorm,

    /// Four-component 64-bit signed-integer format with 16-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_SINT. OpenGL counterpart:
    /// GL_RGBA16I.
    Rgba16i,

    /// Two-component 64-bit floating-point format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32_FLOAT. OpenGL counterpart: GL_RG32F.
    Rg32f,

    /// Two-component 64-bit unsigned-integer format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32_UINT. OpenGL counterpart: GL_RG32UI.
    Rg32ui,

    /// Two-component 64-bit signed-integer format with 32-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R32G32_SINT. OpenGL counterpart: GL_RG32I.
    Rg32i,

    /// Two-component 64-bit format with 32-bit floating-point depth channel and
    /// 8-bit stencil channel. \n
    /// D3D counterpart: DXGI_FORMAT_D32_FLOAT_S8X24_UINT. OpenGL counterpart:
    /// GL_DEPTH32F_STENCIL8.
    D32f_S8,

    /// Four-component 32-bit unsigned-normalized-integer format with 10 bits
    /// for each color and 2 bits for alpha channel. \n
    /// D3D counterpart: DXGI_FORMAT_R10G10B10A2_UNORM. OpenGL counterpart:
    /// GL_RGB10_A2.
    Rgb10_A2,

    /// Four-component 32-bit unsigned-integer format with 10 bits for each
    /// color and 2 bits for alpha channel. \n
    /// D3D counterpart: DXGI_FORMAT_R10G10B10A2_UINT. OpenGL counterpart:
    /// GL_RGB10_A2UI.
    Rgb10_A2ui,

    /// Three-component 32-bit format encoding three partial precision channels
    /// using 11 bits for red and green and 10 bits for blue channel. \n
    /// D3D counterpart: DXGI_FORMAT_R11G11B10_FLOAT. OpenGL counterpart:
    /// GL_R11F_G11F_B10F.
    R11f_G11f_B10f,

    /// Four-component 32-bit unsigned-normalized-integer format with 8-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_UNORM. OpenGL counterpart:
    /// GL_RGBA8.
    Rgba8,

    /// Four-component 32-bit unsigned-normalized-integer sRGB format with 8-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_UNORM_SRGB. OpenGL counterpart:
    /// GL_SRGB8_ALPHA8.
    Srgb8_A8,

    /// Four-component 32-bit unsigned-integer format with 8-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_UINT. OpenGL counterpart:
    /// GL_RGBA8UI.
    Rgba8ui,

    /// Four-component 32-bit signed-normalized-integer format with 8-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_SNORM. OpenGL counterpart:
    /// GL_RGBA8_SNORM.
    Rgba8snorm,

    /// Four-component 32-bit signed-integer format with 8-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_SINT. OpenGL counterpart:
    /// GL_RGBA8I.
    Rgba8i,

    /// Two-component 32-bit half-precision floating-point format with 16-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16_FLOAT. OpenGL counterpart: GL_RG16F.
    Rg16f,

    /// Two-component 32-bit unsigned-normalized-integer format with 16-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16_UNORM. OpenGL counterpart: GL_RG16.
    /// \n
    /// [GL_EXT_texture_norm16]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
    /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
    Rg16,

    /// Two-component 32-bit unsigned-integer format with 16-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16_UINT. OpenGL counterpart: GL_RG16UI.
    Rg16ui,

    /// Two-component 32-bit signed-normalized-integer format with 16-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16_SNORM. OpenGL counterpart:
    /// GL_RG16_SNORM. \n
    /// [GL_EXT_texture_norm16]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
    /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
    Rg16snorm,

    /// Two-component 32-bit signed-integer format with 16-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R16G16_SINT. OpenGL counterpart: GL_RG16I.
    Rg16i,

    /// Single-component 32-bit floating-point depth format. \n
    /// D3D counterpart: DXGI_FORMAT_D32_FLOAT. OpenGL counterpart:
    /// GL_DEPTH_COMPONENT32F.
    D32f,

    /// Single-component 32-bit floating-point format. \n
    /// D3D counterpart: DXGI_FORMAT_R32_FLOAT. OpenGL counterpart: GL_R32F.
    R32f,

    /// Single-component 32-bit unsigned-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R32_UINT. OpenGL counterpart: GL_R32UI.
    R32ui,

    /// Single-component 32-bit signed-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R32_SINT. OpenGL counterpart: GL_R32I.
    R32i,

    /// Two-component 32-bit format with 24 bits for unsigned-normalized-integer
    /// depth and 8 bits for stencil. \n
    /// D3D counterpart: DXGI_FORMAT_D24_UNORM_S8_UINT. OpenGL counterpart:
    /// GL_DEPTH24_STENCIL8.
    D24_S8,

    /// Two-component 16-bit unsigned-normalized-integer format with 8-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8_UNORM. OpenGL counterpart: GL_RG8.
    Rg8,

    /// Two-component 16-bit unsigned-integer format with 8-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8_UINT. OpenGL counterpart: GL_RG8UI.
    Rg8ui,

    /// Two-component 16-bit signed-normalized-integer format with 8-bit
    /// channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8_SNORM. OpenGL counterpart:
    /// GL_RG8_SNORM.
    Rg8snorm,

    /// Two-component 16-bit signed-integer format with 8-bit channels. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8_SINT. OpenGL counterpart: GL_RG8I.
    Rg8i,

    /// Single-component 16-bit half-precision floating-point format. \n
    /// D3D counterpart: DXGI_FORMAT_R16_FLOAT. OpenGL counterpart: GL_R16F.
    R16f,

    /// Single-component 16-bit unsigned-normalized-integer depth format. \n
    /// D3D counterpart: DXGI_FORMAT_D16_UNORM. OpenGL counterpart:
    /// GL_DEPTH_COMPONENT16.
    D16,

    /// Single-component 16-bit unsigned-normalized-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R16_UNORM. OpenGL counterpart: GL_R16.
    /// [GL_EXT_texture_norm16]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
    /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
    R16,

    /// Single-component 16-bit unsigned-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R16_UINT. OpenGL counterpart: GL_R16UI.
    R16ui,

    /// Single-component 16-bit signed-normalized-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R16_SNORM. OpenGL counterpart:
    /// GL_R16_SNORM. \n
    /// [GL_EXT_texture_norm16]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
    /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
    R16snorm,

    /// Single-component 16-bit signed-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R16_SINT. OpenGL counterpart: GL_R16I.
    R16i,

    /// Single-component 8-bit unsigned-normalized-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R8_UNORM. OpenGL counterpart: GL_R8.
    R8,

    /// Single-component 8-bit unsigned-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R8_UINT. OpenGL counterpart: GL_R8UI.
    R8ui,

    /// Single-component 8-bit signed-normalized-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R8_SNORM. OpenGL counterpart: GL_R8_SNORM.
    R8snorm,

    /// Single-component 8-bit signed-integer format. \n
    /// D3D counterpart: DXGI_FORMAT_R8_SINT. OpenGL counterpart: GL_R8I.
    R8i,

    /// Single-component 8-bit unsigned-normalized-integer format for alpha
    /// only. \n
    /// D3D counterpart: DXGI_FORMAT_A8_UNORM
    /// \warning This format is not available in OpenGL
    A8,

    /// Single-component 1-bit format. \n
    /// D3D counterpart: DXGI_FORMAT_R1_UNORM
    /// \warning This format is not available in OpenGL
    R1,

    /// Three partial-precision floating pointer numbers sharing single exponent
    /// encoded into a 32-bit value. \n
    /// D3D counterpart: DXGI_FORMAT_R9G9B9E5_SHAREDEXP. OpenGL counterpart:
    /// GL_RGB9_E5.
    Rgb9_E5,

    /// Four-component unsigned-normalized integer format analogous to UYVY
    /// encoding. \n
    /// D3D counterpart: DXGI_FORMAT_R8G8_B8G8_UNORM
    /// \warning This format is not available in OpenGL
    Rg8_B8_G8,

    /// Four-component unsigned-normalized integer format analogous to YUY2
    /// encoding. \n
    /// D3D counterpart: DXGI_FORMAT_G8R8_G8B8_UNORM
    /// \warning This format is not available in OpenGL
    G8_R8_G8_B8,

    /// Four-component unsigned-normalized-integer block-compression format with
    /// 5 bits for R, 6 bits for G, 5 bits for B, and 0 or 1 bit for A channel.
    /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel)
    /// providing 1:8 compression ratio against RGBA8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC1_UNORM. OpenGL counterpart:
    /// GL_COMPRESSED_RGB_S3TC_DXT1_EXT.\n
    /// [GL_EXT_texture_compression_s3tc]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
    /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is
    /// required
    BC1,

    /// Four-component unsigned-normalized-integer block-compression sRGB format
    /// with 5 bits for R, 6 bits for G, 5 bits for B, and 0 or 1 bit for A
    /// channel. \n
    /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel)
    /// providing 1:8 compression ratio against RGBA8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC1_UNORM_SRGB. OpenGL counterpart:
    /// GL_COMPRESSED_SRGB_S3TC_DXT1_EXT.\n
    /// [GL_EXT_texture_compression_s3tc]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
    /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is
    /// required
    BC1_Srgb,

    /// Four-component unsigned-normalized-integer block-compression format with
    /// 5 bits for R, 6 bits for G, 5 bits for B, and 4 bits for low-coherent
    /// separate A channel.
    /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per
    /// pixel) providing 1:4 compression ratio against RGBA8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC2_UNORM. OpenGL counterpart:
    /// GL_COMPRESSED_RGBA_S3TC_DXT3_EXT. \n
    /// [GL_EXT_texture_compression_s3tc]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
    /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is
    /// required
    BC2,

    /// Four-component signed-normalized-integer block-compression sRGB format
    /// with 5 bits for R, 6 bits for G, 5 bits for B, and 4 bits for
    /// low-coherent separate A channel.
    /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per
    /// pixel) providing 1:4 compression ratio against RGBA8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC2_UNORM_SRGB. OpenGL counterpart:
    /// GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT. \n
    /// [GL_EXT_texture_compression_s3tc]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
    /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is
    /// required
    BC2_Srgb,

    /// Four-component unsigned-normalized-integer block-compression format with
    /// 5 bits for R, 6 bits for G, 5 bits for B, and 8 bits for highly-coherent
    /// A channel.
    /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per
    /// pixel) providing 1:4 compression ratio against RGBA8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC3_UNORM. OpenGL counterpart:
    /// GL_COMPRESSED_RGBA_S3TC_DXT5_EXT. \n
    /// [GL_EXT_texture_compression_s3tc]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
    /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is
    /// required
    BC3,

    /// Four-component unsigned-normalized-integer block-compression sRGB format
    /// with 5 bits for R, 6 bits for G, 5 bits for B, and 8 bits for
    /// highly-coherent A channel.
    /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per
    /// pixel) providing 1:4 compression ratio against RGBA8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC3_UNORM_SRGB. OpenGL counterpart:
    /// GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT. \n
    /// [GL_EXT_texture_compression_s3tc]:
    /// https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
    /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is
    /// required
    BC3_Srgb,

    /// One-component unsigned-normalized-integer block-compression format with
    /// 8 bits for R channel.
    /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel)
    /// providing 1:2 compression ratio against R8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC4_UNORM. OpenGL counterpart:
    /// GL_COMPRESSED_RED_RGTC1. \n
    /// [GL_ARB_texture_compression_rgtc]:
    /// https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
    /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is
    /// required
    BC4,

    /// One-component signed-normalized-integer block-compression format with 8
    /// bits for R channel.
    /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel)
    /// providing 1:2 compression ratio against R8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC4_SNORM. OpenGL counterpart:
    /// GL_COMPRESSED_SIGNED_RED_RGTC1. \n
    /// [GL_ARB_texture_compression_rgtc]:
    /// https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
    /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is
    /// required
    BC4snorm,

    /// Two-component unsigned-normalized-integer block-compression format with
    /// 8 bits for R and 8 bits for G channel.
    /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per
    /// pixel) providing 1:2 compression ratio against RG8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC5_UNORM. OpenGL counterpart:
    /// GL_COMPRESSED_RG_RGTC2. \n
    /// [GL_ARB_texture_compression_rgtc]:
    /// https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
    /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is
    /// required
    BC5,

    /// Two-component signed-normalized-integer block-compression format with 8
    /// bits for R and 8 bits for G channel.
    /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per
    /// pixel) providing 1:2 compression ratio against RG8 format. \n
    /// D3D counterpart: DXGI_FORMAT_BC5_SNORM. OpenGL counterpart:
    /// GL_COMPRESSED_SIGNED_RG_RGTC2. \n
    /// [GL_ARB_texture_compression_rgtc]:
    /// https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
    /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is
    /// required
    BC5snorm,

    /// Three-component 16-bit unsigned-normalized-integer format with 5 bits
    /// for blue, 6 bits for green, and 5 bits for red channel. \n
    /// D3D counterpart: DXGI_FORMAT_B5G6R5_UNORM
    /// \warning This format is not available until D3D11.1 and Windows 8. It is
    /// also not available in OpenGL
    B5_G6_R5,

    /// Four-component 16-bit unsigned-normalized-integer format with 5 bits for
    /// each color channel and 1-bit alpha. \n
    /// D3D counterpart: DXGI_FORMAT_B5G5R5A1_UNORM
    /// \warning This format is not available until D3D11.1 and Windows 8. It is
    /// also not available in OpenGL
    B5_G5_R5_A1,

    /// Four-component 32-bit unsigned-normalized-integer format with 8 bits for
    /// each channel. \n
    /// D3D counterpart: DXGI_FORMAT_B8G8R8A8_UNORM.
    /// \warning This format is not available in OpenGL
    Bgra8,

    /// Four-component 32-bit unsigned-normalized-integer format with 8 bits for
    /// each color channel and 8 bits unused. \n
    /// D3D counterpart: DXGI_FORMAT_B8G8R8X8_UNORM.
    /// \warning This format is not available in OpenGL
    Bgrx8,

    /// Four-component 32-bit 2.8-biased fixed-point format with 10 bits for
    /// each color channel and 2-bit alpha. \n
    /// D3D counterpart: DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM.
    /// \warning This format is not available in OpenGL
    R10_G10_B10_XR_BIAS_A2,

    /// Four-component 32-bit unsigned-normalized sRGB format with 8 bits for
    /// each channel. \n
    /// D3D counterpart: DXGI_FORMAT_B8G8R8A8_UNORM_SRGB.
    /// \warning This format is not available in OpenGL.
    Bgra8_Srgb,

    /// Four-component 32-bit unsigned-normalized sRGB format with 8 bits for
    /// each color channel, and 8 bits are unused. \n
    /// D3D counterpart: DXGI_FORMAT_B8G8R8X8_UNORM_SRGB.
    /// \warning This format is not available in OpenGL.
    Bgrx8_Srgb,

    /// Three-component unsigned half-precision floating-point format with 16
    /// bits for each channel. \n
    /// D3D counterpart: DXGI_FORMAT_BC6H_UF16. OpenGL counterpart:
    /// GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT. \n
    /// [GL_ARB_texture_compression_bptc]:
    /// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
    /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not
    /// supported in at least OpenGLES3.1
    BC6H_UF16,

    /// Three-channel signed half-precision floating-point format with 16 bits
    /// per each channel. \n
    /// D3D counterpart: DXGI_FORMAT_BC6H_SF16. OpenGL counterpart:
    /// GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT. \n
    /// [GL_ARB_texture_compression_bptc]:
    /// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
    /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not
    /// supported in at least OpenGLES3.1
    BC6H_SF16,

    /// Three-component block-compression unsigned-normalized-integer format
    /// with 4 to 7 bits per color channel and 0 to 8 bits of alpha. \n
    /// D3D counterpart: DXGI_FORMAT_BC7_UNORM. OpenGL counterpart:
    /// GL_COMPRESSED_RGBA_BPTC_UNORM. \n
    /// [GL_ARB_texture_compression_bptc]:
    /// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
    /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not
    /// supported in at least OpenGLES3.1
    BC7,

    /// Three-component block-compression unsigned-normalized-integer sRGB
    /// format with 4 to 7 bits per color channel and 0 to 8 bits of alpha. \n
    /// D3D counterpart: DXGI_FORMAT_BC7_UNORM_SRGB. OpenGL counterpart:
    /// GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM. \n
    /// [GL_ARB_texture_compression_bptc]:
    /// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
    /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not
    /// supported in at least OpenGLES3.1
    BC7_Srgb,

    /// Three-component block-compression unsigned-normalized-integer format. \n
    /// OpenGL counterpart: GL_COMPRESSED_RGB8_ETC2.
    ETC2_Rgb8,

    /// Three-component block-compression unsigned-normalized-integer sRGB
    /// format. \n
    /// OpenGL counterpart: GL_COMPRESSED_SRGB8_ETC2.
    ETC2_Rgb8_Srgb,

    /// Four-component block-compression unsigned-normalized-integer format. \n
    /// OpenGL counterpart: GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2.
    ETC2_Rgb8_A1,

    /// Four-component block-compression unsigned-normalized-integer sRGB
    /// format. \n
    /// OpenGL counterpart: GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2.
    ETC2_Rgb8_A1_Srgb,

    /// Four-component block-compression unsigned-normalized-integer format. \n
    /// OpenGL counterpart: GL_COMPRESSED_RGBA8_ETC2_EAC.
    ETC2_Rgba8,

    /// Four-component block-compression unsigned-normalized-integer sRGB
    /// format. \n
    /// OpenGL counterpart: GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC.
    ETC2_Rgba8_Srgb,

    /// Helper member containing the total number of texture formats in the
    /// enumeration
    NUM_FORMATS
};

enum class TextureType : uint8_t {
    Undefined = 0, ///< Texture type undefined
    Buffer,        ///< Buffer
    Tex1D,         ///< One-dimensional texture
    Tex1DArray,    ///< One-dimensional texture array
    Tex2D,         ///< Two-dimensional texture
    Tex2DArray,    ///< Two-dimensional texture array
    Tex3D,         ///< Three-dimensional texture
    TexCube,       ///< Cube-map texture
    TexCubeArray   ///< Cube-map array texture
};

///////////////////////////////////////////////////////////
/// \brief Primitive topologies
///
/// From Diligent Engine enum
///
///////////////////////////////////////////////////////////
enum class PrimitiveTopology : uint8_t {
    /// Undefined topology
    Undefined = 0,

    /// Interpret the vertex data as a list of triangles.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST. OpenGL
    /// counterpart: GL_TRIANGLES.
    TriangleList,

    /// Interpret the vertex data as a triangle strip.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP. OpenGL
    /// counterpart: GL_TRIANGLE_STRIP.
    TriangleStrip,

    /// Interpret the vertex data as a list of points.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_POINTLIST. OpenGL counterpart:
    /// GL_POINTS.
    PointList,

    /// Interpret the vertex data as a list of lines.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_LINELIST. OpenGL counterpart:
    /// GL_LINES.
    LineList,

    /// Interpret the vertex data as a line strip.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_LINESTRIP. OpenGL counterpart:
    /// GL_LINE_STRIP.
    LineStrip,

    /// Interpret the vertex data as a list of triangles with adjacency data.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ. OpenGL
    /// counterpart: GL_TRIANGLES_ADJACENCY.
    TriangleListAdj,

    /// Interpret the vertex data as a triangle strip with adjacency data.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ. OpenGL
    /// counterpart: GL_TRIANGLE_STRIP_ADJACENCY.
    TriangleStripAdj,

    /// Interpret the vertex data as a list of lines with adjacency data.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ. OpenGL
    /// counterpart: GL_LINES_ADJACENCY.
    LineListAdj,

    /// Interpret the vertex data as a line strip with adjacency data.\n
    /// D3D counterpart: D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ. OpenGL
    /// counterpart: GL_LINE_STRIP_ADJACENCY.
    LineStripAdj,

    /// Helper value that stores the total number of topologies in the
    /// enumeration
    NUM_TOPOLOGIES
};

///////////////////////////////////////////////////////////
/// \brief Cull mode
///
///////////////////////////////////////////////////////////
enum class CullMode : int8_t {
    /// Undefined cull mode.
    Undefined = 0,

    /// Draw all triangles. \n
    /// Direct3D counterpart: D3D11_CULL_NONE/D3D12_CULL_MODE_NONE. OpenGL
    /// counterpart: glDisable( GL_CULL_FACE ).
    None,

    /// Do not draw triangles that are front-facing. Front- and back-facing
    /// triangles are determined
    /// by the RasterizerStateDesc::FrontCounterClockwise member. \n
    /// Direct3D counterpart: D3D11_CULL_FRONT/D3D12_CULL_MODE_FRONT. OpenGL
    /// counterpart: GL_FRONT.
    Front,

    /// Do not draw triangles that are back-facing. Front- and back-facing
    /// triangles are determined
    /// by the RasterizerStateDesc::FrontCounterClockwise member. \n
    /// Direct3D counterpart: D3D11_CULL_BACK/D3D12_CULL_MODE_BACK. OpenGL
    /// counterpart: GL_BACK.
    Back,

    /// Helper value that stores the total number of cull modes in the
    /// enumeration.
    NUM_MODES
};

///////////////////////////////////////////////////////////
/// \brief Fill mode
///
///////////////////////////////////////////////////////////
enum class FillMode : int8_t {
    /// Undefined fill mode.
    Undefined = 0,

    /// Rasterize triangles using wireframe fill. \n
    /// Direct3D counterpart: D3D11_FILL_WIREFRAME/D3D12_FILL_MODE_WIREFRAME.
    /// OpenGL counterpart: GL_LINE.
    Wireframe,

    /// Rasterize triangles using solid fill. \n
    /// Direct3D counterpart: D3D11_FILL_SOLID/D3D12_FILL_MODE_SOLID. OpenGL
    /// counterpart: GL_FILL.
    Solid,

    /// Helper value that stores the total number of fill modes in the
    /// enumeration.
    NUM_MODES
};

enum class BlendFactor : int8_t {
    /// Undefined blend factor
    Undefined = 0,

    /// The blend factor is zero.\n
    /// Direct3D counterpart: D3D11_BLEND_ZERO/D3D12_BLEND_ZERO. OpenGL
    /// counterpart: GL_ZERO.
    Zero,

    /// The blend factor is one.\n
    /// Direct3D counterpart: D3D11_BLEND_ONE/D3D12_BLEND_ONE. OpenGL
    /// counterpart: GL_ONE.
    One,

    /// The blend factor is RGB data from a pixel shader.\n
    /// Direct3D counterpart: D3D11_BLEND_SRC_COLOR/D3D12_BLEND_SRC_COLOR.
    /// OpenGL counterpart: GL_SRC_COLOR.
    SrcColor,

    /// The blend factor is 1-RGB, where RGB is the data from a pixel shader.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_INV_SRC_COLOR/D3D12_BLEND_INV_SRC_COLOR. OpenGL counterpart:
    /// GL_ONE_MINUS_SRC_COLOR.
    InvSrcColor,

    /// The blend factor is alpha (A) data from a pixel shader.\n
    /// Direct3D counterpart: D3D11_BLEND_SRC_ALPHA/D3D12_BLEND_SRC_ALPHA.
    /// OpenGL counterpart: GL_SRC_ALPHA.
    SrcAlpha,

    /// The blend factor is 1-A, where A is alpha data from a pixel shader.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_INV_SRC_ALPHA/D3D12_BLEND_INV_SRC_ALPHA. OpenGL counterpart:
    /// GL_ONE_MINUS_SRC_ALPHA.
    InvSrcAlpha,

    /// The blend factor is alpha (A) data from a render target.\n
    /// Direct3D counterpart: D3D11_BLEND_DEST_ALPHA/D3D12_BLEND_DEST_ALPHA.
    /// OpenGL counterpart: GL_DST_ALPHA.
    DestAlpha,

    /// The blend factor is 1-A, where A is alpha data from a render target.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_INV_DEST_ALPHA/D3D12_BLEND_INV_DEST_ALPHA. OpenGL
    /// counterpart: GL_ONE_MINUS_DST_ALPHA.
    InvDestAlpha,

    /// The blend factor is RGB data from a render target.\n
    /// Direct3D counterpart: D3D11_BLEND_DEST_COLOR/D3D12_BLEND_DEST_COLOR.
    /// OpenGL counterpart: GL_DST_COLOR.
    DestColor,

    /// The blend factor is 1-RGB, where RGB is the data from a render target.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_INV_DEST_COLOR/D3D12_BLEND_INV_DEST_COLOR. OpenGL
    /// counterpart: GL_ONE_MINUS_DST_COLOR.
    InvDestColor,

    /// The blend factor is (f,f,f,1), where f = min(As, 1-Ad),
    /// As is alpha data from a pixel shader, and Ad is alpha data from a render
    /// target.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_SRC_ALPHA_SAT/D3D12_BLEND_SRC_ALPHA_SAT. OpenGL counterpart:
    /// GL_SRC_ALPHA_SATURATE.
    SrcAlphaSat,

    /// The blend factor is the constant blend factor set with
    /// IDeviceContext::SetBlendFactors().\n
    /// Direct3D counterpart: D3D11_BLEND_BLEND_FACTOR/D3D12_BLEND_BLEND_FACTOR.
    /// OpenGL counterpart: GL_CONSTANT_COLOR.
    BlendFactor,

    /// The blend factor is one minus constant blend factor set with
    /// IDeviceContext::SetBlendFactors().\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_INV_BLEND_FACTOR/D3D12_BLEND_INV_BLEND_FACTOR. OpenGL
    /// counterpart: GL_ONE_MINUS_CONSTANT_COLOR.
    InvBlendFactor,

    /// The blend factor is the second RGB data output from a pixel shader.\n
    /// Direct3D counterpart: D3D11_BLEND_SRC1_COLOR/D3D12_BLEND_SRC1_COLOR.
    /// OpenGL counterpart: GL_SRC1_COLOR.
    Src1Color,

    /// The blend factor is 1-RGB, where RGB is the second RGB data output from
    /// a pixel shader.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_INV_SRC1_COLOR/D3D12_BLEND_INV_SRC1_COLOR. OpenGL
    /// counterpart: GL_ONE_MINUS_SRC1_COLOR.
    InvSrc1Color,

    /// The blend factor is the second alpha (A) data output from a pixel
    /// shader.\n
    /// Direct3D counterpart: D3D11_BLEND_SRC1_ALPHA/D3D12_BLEND_SRC1_ALPHA.
    /// OpenGL counterpart: GL_SRC1_ALPHA.
    Src1Alpha,

    /// The blend factor is 1-A, where A is the second alpha data output from a
    /// pixel shader.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_INV_SRC1_ALPHA/D3D12_BLEND_INV_SRC1_ALPHA. OpenGL
    /// counterpart: GL_ONE_MINUS_SRC1_ALPHA.
    InvSrc1Alpha,

    /// Helper value that stores the total number of blend factors in the
    /// enumeration.
    NUM_FACTORS
};

enum class BlendOperation : int8_t {
    /// Undefined blend operation
    Undefined = 0,

    /// Add source and destination color components.\n
    /// Direct3D counterpart: D3D11_BLEND_OP_ADD/D3D12_BLEND_OP_ADD. OpenGL
    /// counterpart: GL_FUNC_ADD.
    Add,

    /// Subtract destination color components from source color components.\n
    /// Direct3D counterpart: D3D11_BLEND_OP_SUBTRACT/D3D12_BLEND_OP_SUBTRACT.
    /// OpenGL counterpart: GL_FUNC_SUBTRACT.
    Subtract,

    /// Subtract source color components from destination color components.\n
    /// Direct3D counterpart:
    /// D3D11_BLEND_OP_REV_SUBTRACT/D3D12_BLEND_OP_REV_SUBTRACT. OpenGL
    /// counterpart: GL_FUNC_REVERSE_SUBTRACT.
    ReverseSubtract,

    /// Compute the minimum of source and destination color components.\n
    /// Direct3D counterpart: D3D11_BLEND_OP_MIN/D3D12_BLEND_OP_MIN. OpenGL
    /// counterpart: GL_MIN.
    Min,

    /// Compute the maximum of source and destination color components.\n
    /// Direct3D counterpart: D3D11_BLEND_OP_MAX/D3D12_BLEND_OP_MAX. OpenGL
    /// counterpart: GL_MAX.
    Max,

    /// Helper value that stores the total number of blend operations in the
    /// enumeration.
    NUM_OPERATIONS
};

enum class Type : uint8_t {
    Undefined = 0, ///< Undefined type
    Int8,          ///< Signed 8-bit integer
    Int16,         ///< Signed 16-bit integer
    Int32,         ///< Signed 32-bit integer
    Uint8,         ///< Unsigned 8-bit integer
    Uint16,        ///< Unsigned 16-bit integer
    Uint32,        ///< Unsigned 32-bit integer
    Float16,       ///< Half-precision 16-bit floating point
    Float32,       ///< Full-precision 32-bit floating point
    Float64,       ///< Double-precision 64-bit floating point
    NUM_TYPES      ///< Helper value storing total number of types in the
                   ///< enumeration
};

/// \brief Defines how a resource can be used by the GPU and CPU
enum class ResourceUsage : uint8_t {
    /// A resource that can only be read by the GPU. It cannot be written by the
    /// GPU,
    /// and cannot be accessed at all by the CPU. This type of resource must be
    /// initialized
    /// when it is created, since it cannot be changed after creation. \n
    /// D3D11 Counterpart: D3D11_USAGE_IMMUTABLE. OpenGL counterpart:
    /// GL_STATIC_DRAW
    /// \remarks Static buffers do not allow CPU access and must use
    /// CPU_ACCESS_NONE flag.
    Immutable = 0,

    /// A resource that requires read and write access by the GPU and can also
    /// be occasionally
    /// written by the CPU.  \n
    /// D3D11 Counterpart: D3D11_USAGE_DEFAULT. OpenGL counterpart:
    /// GL_DYNAMIC_DRAW.
    /// \remarks Default buffers do not allow CPU access and must use
    /// CPU_ACCESS_NONE flag.
    Default,

    /// A resource that can be read by the GPU and written at least once per
    /// frame by the CPU.  \n
    /// D3D11 Counterpart: D3D11_USAGE_DYNAMIC. OpenGL counterpart:
    /// GL_STREAM_DRAW
    /// \remarks Dynamic buffers must use CPU_ACCESS_WRITE flag.
    Dynamic,

    /// A resource that facilitates transferring data between GPU and CPU. \n
    /// D3D11 Counterpart: D3D11_USAGE_STAGING. OpenGL counterpart:
    /// GL_STATIC_READ or
    /// GL_STATIC_COPY depending on the CPU access flags.
    /// \remarks Staging buffers must use exactly one of CPU_ACCESS_WRITE or
    /// CPU_ACCESS_READ flags.
    Staging,

    /// A resource residing in a unified memory (e.g. memory shared between CPU
    /// and GPU),
    /// that can be read and written by GPU and can also be directly accessed by
    /// CPU.
    ///
    /// \remarks An application should check if unified memory is available on
    /// the device by querying
    ///          the adapter info (see
    ///          Diligent::IRenderDevice::GetAdapterInfo().Memory and
    ///          Diligent::AdapterMemoryInfo).
    ///          If there is no unified memory, an application should choose
    ///          another usage type (typically, USAGE_DEFAULT).
    ///
    ///          Unified resources must use at least one of CPU_ACCESS_WRITE or
    ///          CPU_ACCESS_READ flags.
    ///          An application should check supported unified memory CPU access
    ///          types by querying the device caps.
    ///          (see Diligent::AdapterMemoryInfo::UnifiedMemoryCPUAccess).
    Unified,

    /// A resource that can be partially committed to physical memory.
    Sparse,

    /// Helper value indicating the total number of elements in the enum
    NUM_USAGES
};

/// \brief Defines which targest a gpu resource can be bound to
enum class ResourceBind : uint32_t {
    /// Undefined binding.
    None = 0,

    /// A buffer can be bound as a vertex buffer.
    VertexBuffer = 1u << 0u,

    /// A buffer can be bound as an index buffer.
    IndexBuffer = 1u << 1u,

    /// A buffer can be bound as a uniform buffer.
    ///
    /// \warning This flag may not be combined with any other bind flag.
    UniformBuffer = 1u << 2u,

    /// A buffer or a texture can be bound as a shader resource.
    ShaderResource = 1u << 3u,

    /// A buffer can be bound as a target for stream output stage.
    StreamOutput = 1u << 4u,

    /// A texture can be bound as a render target.
    RenderTarget = 1u << 5u,

    /// A texture can be bound as a depth-stencil target.
    DepthStencil = 1u << 6u,

    /// A buffer or a texture can be bound as an unordered access view.
    UnorderedAccess = 1u << 7u,

    /// A buffer can be bound as the source buffer for indirect draw commands.
    IndirectDrawArgs = 1u << 8u,

    /// A texture can be used as render pass input attachment.
    InputAttachment = 1u << 9u,

    /// A buffer can be used as a scratch buffer or as the source of primitive
    /// data
    /// for acceleration structure building.
    RayTracing = 1u << 10u,

    /// A texture can be used as shading rate texture.
    ShadingRate = 1u << 11u
};
BIT_OPERATOR(ResourceBind);

/// \brief Defines cpu access flags for gpu resources.
enum class ResourceAccess : uint8_t {
    None = 0u,        ///< No CPU access
    Read = 1u << 0u,  ///< A resource can be mapped for reading
    Write = 1u << 1u, ///< A resource can be mapped for writing
};
BIT_OPERATOR(ResourceAccess);

/// \brief Defines shader resource binding type
enum class ShaderResourceType : uint8_t {
    /// Shader resource bound to the variable is the same for all SRB
    /// instances. It must be set *once* directly through Pipeline State
    /// object.
    Static = 0,

    /// Shader resource bound to the variable is specific to the shader
    /// resource binding instance. It must be set *once* through
    /// ResourceBinding and cannot be change once bound.
    Mutable,

    /// Shader variable binding is dynamic. It can be set multiple times for
    /// every instance of shader resource binding.
    Dynamic,

    /// Total number of shader variable types
    NUM_TYPES
};

enum class ResourceState : uint32_t {
    /// The resource state is not known to the engine and is managed by the
    /// application
    Unknown = 0,

    /// The resource state is known to the engine, but is undefined. A resource
    /// is typically in an undefined state right after initialization.
    Undefined = 1u << 0,

    /// The resource is accessed as a vertex buffer
    /// \remarks Supported contexts: graphics.
    VertexBuffer = 1u << 1,

    /// The resource is accessed as a constant (uniform) buffer
    /// \remarks Supported contexts: graphics, compute.
    ConstantBuffer = 1u << 2,

    /// The resource is accessed as an index buffer
    /// \remarks Supported contexts: graphics.
    IndexBuffer = 1u << 3,

    /// The resource is accessed as a render target
    /// \remarks Supported contexts: graphics.
    RenderTarget = 1u << 4,

    /// The resource is used for unordered access
    /// \remarks Supported contexts: graphics, compute.
    UnorderedAccess = 1u << 5,

    /// The resource is used in a writable depth-stencil view or in clear
    /// operation
    /// \remarks Supported contexts: graphics.
    DepthWrite = 1u << 6,

    /// The resource is used in a read-only depth-stencil view
    /// \remarks Supported contexts: graphics.
    DepthRead = 1u << 7,

    /// The resource is accessed from a shader
    /// \remarks Supported contexts: graphics, compute.
    ShaderResource = 1u << 8,

    /// The resource is used as the destination for stream output
    StreamOut = 1u << 9,

    /// The resource is used as an indirect draw/dispatch arguments buffer
    /// \remarks Supported contexts: graphics, compute.
    IndirectArgument = 1u << 10,

    /// The resource is used as the destination in a copy operation
    /// \remarks Supported contexts: graphics, compute, transfer.
    CopyDest = 1u << 11,

    /// The resource is used as the source in a copy operation
    /// \remarks Supported contexts: graphics, compute, transfer.
    CopySource = 1u << 12,

    /// The resource is used as the destination in a resolve operation
    /// \remarks Supported contexts: graphics.
    ResolveDest = 1u << 13,

    /// The resource is used as the source in a resolve operation
    /// \remarks Supported contexts: graphics.
    ResolveSource = 1u << 14,

    /// The resource is used as an input attachment in a render pass subpass
    /// \remarks Supported contexts: graphics.
    InputAttachment = 1u << 15
};

enum class TextureFilter : uint8_t {
    Unknown = 0, ///< Unknown filter type
    Point,       ///< Point filtering
    Linear,      ///< Linear filtering
    Anisotropic, ///< Anisotropic filtering
};

enum class TextureAddress : uint8_t {
    /// Unknown mode
    Unknown = 0,

    /// Tile the texture at every integer junction. \n
    /// Direct3D Counterpart:
    /// D3D11_TEXTURE_ADDRESS_WRAP/D3D12_TEXTURE_ADDRESS_MODE_WRAP. OpenGL
    /// counterpart: GL_REPEAT
    Wrap = 1,

    /// Flip the texture at every integer junction. \n
    /// Direct3D Counterpart:
    /// D3D11_TEXTURE_ADDRESS_MIRROR/D3D12_TEXTURE_ADDRESS_MODE_MIRROR. OpenGL
    /// counterpart: GL_MIRRORED_REPEAT
    Mirror = 2,

    /// Texture coordinates outside the range [0.0, 1.0] are set to the
    /// texture color at 0.0 or 1.0, respectively. \n
    /// Direct3D Counterpart:
    /// D3D11_TEXTURE_ADDRESS_CLAMP/D3D12_TEXTURE_ADDRESS_MODE_CLAMP. OpenGL
    /// counterpart: GL_CLAMP_TO_EDGE
    Clamp = 3,

    /// Texture coordinates outside the range [0.0, 1.0] are set to the border
    /// color
    /// specified in SamplerDesc structure. \n
    /// Direct3D Counterpart:
    /// D3D11_TEXTURE_ADDRESS_BORDER/D3D12_TEXTURE_ADDRESS_MODE_BORDER. OpenGL
    /// counterpart: GL_CLAMP_TO_BORDER
    Border = 4,

    /// Similar to TEXTURE_ADDRESS_MIRROR and TEXTURE_ADDRESS_CLAMP. Takes the
    /// absolute
    /// value of the texture coordinate (thus, mirroring around 0), and then
    /// clamps to
    /// the maximum value. \n
    /// Direct3D Counterpart:
    /// D3D11_TEXTURE_ADDRESS_MIRROR_ONCE/D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE.
    /// OpenGL counterpart: GL_MIRROR_CLAMP_TO_EDGE
    /// \note GL_MIRROR_CLAMP_TO_EDGE is only available in OpenGL4.4+, and is
    /// not available until at least OpenGLES3.1
    MirrorOnce = 5
};

enum class MapMode : uint8_t { Read = 0x01, Write = 0x02, ReadWrite = 0x03 };

enum class MapFlag : uint8_t {
    None = 0x000,

    /// Specifies that map operation should not wait until previous command that
    /// using the same resource completes. Map returns null pointer if the
    /// resource is still in use.\n D3D11 counterpart:
    /// D3D11_MAP_FLAG_DO_NOT_WAIT
    /// \note: OpenGL does not have corresponding flag, so a buffer will always
    /// be mapped
    DoNotWait = 0x001,

    /// Previous contents of the resource will be undefined. This flag is only
    /// compatible with MAP_WRITE\n D3D11 counterpart: D3D11_MAP_WRITE_DISCARD.
    /// OpenGL counterpart: GL_MAP_INVALIDATE_BUFFER_BIT
    /// \note OpenGL implementation may orphan a buffer instead
    Discard = 0x002,

    /// The system will not synchronize pending operations before mapping the
    /// buffer. It is responsibility of the application to make sure that the
    /// buffer contents is not overwritten while it is in use by the GPU.\n
    /// D3D11 counterpart:  D3D11_MAP_WRITE_NO_OVERWRITE. OpenGL counterpart:
    /// GL_MAP_UNSYNCHRONIZED_BIT
    NoOverwrite = 0x004
};
BIT_OPERATOR(MapFlag);

} // namespace ply