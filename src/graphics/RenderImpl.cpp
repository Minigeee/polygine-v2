#include "RenderImpl.h"

namespace ply {

namespace priv {

    TEXTURE_FORMAT convertTextureFormat(TextureFormat f) {
        switch (f) {
        case TextureFormat::Unknown:
            return TEX_FORMAT_UNKNOWN;
        case TextureFormat::Rgba32f:
            return TEX_FORMAT_RGBA32_FLOAT;
        case TextureFormat::Rgba32ui:
            return TEX_FORMAT_RGBA32_UINT;
        case TextureFormat::Rgba32i:
            return TEX_FORMAT_RGBA32_SINT;
        case TextureFormat::Rgb32f:
            return TEX_FORMAT_RGB32_FLOAT;
        case TextureFormat::Rgb32ui:
            return TEX_FORMAT_RGB32_UINT;
        case TextureFormat::Rgb32i:
            return TEX_FORMAT_RGB32_SINT;
        case TextureFormat::Rgba16f:
            return TEX_FORMAT_RGBA16_FLOAT;
        case TextureFormat::Rgba16:
            return TEX_FORMAT_RGBA16_UNORM;
        case TextureFormat::Rgba16ui:
            return TEX_FORMAT_RGBA16_UINT;
        case TextureFormat::Rgba16snorm:
            return TEX_FORMAT_RGBA16_SNORM;
        case TextureFormat::Rgba16i:
            return TEX_FORMAT_RGBA16_SINT;
        case TextureFormat::Rg32f:
            return TEX_FORMAT_RG32_FLOAT;
        case TextureFormat::Rg32ui:
            return TEX_FORMAT_RG32_UINT;
        case TextureFormat::Rg32i:
            return TEX_FORMAT_RG32_SINT;
        case TextureFormat::D32f_S8:
            return TEX_FORMAT_D32_FLOAT_S8X24_UINT;
        case TextureFormat::Rgb10_A2:
            return TEX_FORMAT_RGB10A2_UNORM;
        case TextureFormat::Rgb10_A2ui:
            return TEX_FORMAT_RGB10A2_UINT;
        case TextureFormat::R11f_G11f_B10f:
            return TEX_FORMAT_R11G11B10_FLOAT;
        case TextureFormat::Rgba8:
            return TEX_FORMAT_RGBA8_UNORM;
        case TextureFormat::Srgb8_A8:
            return TEX_FORMAT_RGBA8_UNORM_SRGB;
        case TextureFormat::Rgba8ui:
            return TEX_FORMAT_RGBA8_UINT;
        case TextureFormat::Rgba8snorm:
            return TEX_FORMAT_RGBA8_SNORM;
        case TextureFormat::Rgba8i:
            return TEX_FORMAT_RGBA8_SINT;
        case TextureFormat::Rg16f:
            return TEX_FORMAT_RG16_FLOAT;
        case TextureFormat::Rg16:
            return TEX_FORMAT_RG16_UNORM;
        case TextureFormat::Rg16ui:
            return TEX_FORMAT_RG16_UINT;
        case TextureFormat::Rg16snorm:
            return TEX_FORMAT_RG16_SNORM;
        case TextureFormat::Rg16i:
            return TEX_FORMAT_RG16_SINT;
        case TextureFormat::D32f:
            return TEX_FORMAT_D32_FLOAT;
        case TextureFormat::R32f:
            return TEX_FORMAT_R32_FLOAT;
        case TextureFormat::R32ui:
            return TEX_FORMAT_R32_UINT;
        case TextureFormat::R32i:
            return TEX_FORMAT_R32_SINT;
        case TextureFormat::D24_S8:
            return TEX_FORMAT_D24_UNORM_S8_UINT;
        case TextureFormat::Rg8:
            return TEX_FORMAT_RG8_UNORM;
        case TextureFormat::Rg8ui:
            return TEX_FORMAT_RG8_UINT;
        case TextureFormat::Rg8snorm:
            return TEX_FORMAT_RG8_SNORM;
        case TextureFormat::Rg8i:
            return TEX_FORMAT_RG8_SINT;
        case TextureFormat::R16f:
            return TEX_FORMAT_R16_FLOAT;
        case TextureFormat::D16:
            return TEX_FORMAT_D16_UNORM;
        case TextureFormat::R16:
            return TEX_FORMAT_R16_UNORM;
        case TextureFormat::R16ui:
            return TEX_FORMAT_R16_UINT;
        case TextureFormat::R16snorm:
            return TEX_FORMAT_R16_SNORM;
        case TextureFormat::R16i:
            return TEX_FORMAT_R16_SINT;
        case TextureFormat::R8:
            return TEX_FORMAT_R8_UNORM;
        case TextureFormat::R8ui:
            return TEX_FORMAT_R8_UINT;
        case TextureFormat::R8snorm:
            return TEX_FORMAT_R8_SNORM;
        case TextureFormat::R8i:
            return TEX_FORMAT_R8_SINT;
        case TextureFormat::A8:
            return TEX_FORMAT_A8_UNORM;
        case TextureFormat::R1:
            return TEX_FORMAT_R1_UNORM;
        case TextureFormat::Rgb9_E5:
            return TEX_FORMAT_RGB9E5_SHAREDEXP;
        case TextureFormat::Rg8_B8_G8:
            return TEX_FORMAT_RG8_B8G8_UNORM;
        case TextureFormat::G8_R8_G8_B8:
            return TEX_FORMAT_G8R8_G8B8_UNORM;
        case TextureFormat::BC1:
            return TEX_FORMAT_BC1_UNORM;
        case TextureFormat::BC1_Srgb:
            return TEX_FORMAT_BC1_UNORM_SRGB;
        case TextureFormat::BC2:
            return TEX_FORMAT_BC2_UNORM;
        case TextureFormat::BC2_Srgb:
            return TEX_FORMAT_BC2_UNORM_SRGB;
        case TextureFormat::BC3:
            return TEX_FORMAT_BC3_UNORM;
        case TextureFormat::BC3_Srgb:
            return TEX_FORMAT_BC3_UNORM_SRGB;
        case TextureFormat::BC4:
            return TEX_FORMAT_BC4_UNORM;
        case TextureFormat::BC4snorm:
            return TEX_FORMAT_BC4_SNORM;
        case TextureFormat::BC5:
            return TEX_FORMAT_BC5_UNORM;
        case TextureFormat::BC5snorm:
            return TEX_FORMAT_BC5_SNORM;
        case TextureFormat::B5_G6_R5:
            return TEX_FORMAT_UNKNOWN; // Not available in OpenGL
        case TextureFormat::B5_G5_R5_A1:
            return TEX_FORMAT_UNKNOWN; // Not available in OpenGL
        case TextureFormat::Bgra8:
            return TEX_FORMAT_UNKNOWN; // Not available in OpenGL
        case TextureFormat::Bgrx8:
            return TEX_FORMAT_UNKNOWN; // Not available in OpenGL
        case TextureFormat::R10_G10_B10_XR_BIAS_A2:
            return TEX_FORMAT_UNKNOWN; // Not available in OpenGL
        case TextureFormat::Bgra8_Srgb:
            return TEX_FORMAT_UNKNOWN; // Not available in OpenGL
        case TextureFormat::Bgrx8_Srgb:
            return TEX_FORMAT_UNKNOWN; // Not available in OpenGL
        case TextureFormat::BC6H_UF16:
            return TEX_FORMAT_UNKNOWN; // Not supported in OpenGLES3.1
        case TextureFormat::BC6H_SF16:
            return TEX_FORMAT_UNKNOWN; // Not supported in OpenGLES3.1
        case TextureFormat::BC7:
            return TEX_FORMAT_UNKNOWN; // Not supported in OpenGLES3.1
        case TextureFormat::BC7_Srgb:
            return TEX_FORMAT_UNKNOWN; // Not supported in OpenGLES3.1
        case TextureFormat::ETC2_Rgb8:
            return TEX_FORMAT_UNKNOWN; // ETC2 formats are OpenGL specific
        case TextureFormat::ETC2_Rgb8_Srgb:
            return TEX_FORMAT_UNKNOWN; // ETC2 formats are OpenGL specific
        case TextureFormat::ETC2_Rgb8_A1:
            return TEX_FORMAT_UNKNOWN; // ETC2 formats are OpenGL specific
        case TextureFormat::ETC2_Rgb8_A1_Srgb:
            return TEX_FORMAT_UNKNOWN; // ETC2 formats are OpenGL specific
        case TextureFormat::ETC2_Rgba8:
            return TEX_FORMAT_UNKNOWN; // ETC2 formats are OpenGL specific
        case TextureFormat::ETC2_Rgba8_Srgb:
            return TEX_FORMAT_UNKNOWN; // ETC2 formats are OpenGL specific
        case TextureFormat::NUM_FORMATS:
            return TEX_FORMAT_UNKNOWN;
        default:
            return TEX_FORMAT_UNKNOWN;
        }
    }

} // namespace priv

} // namespace ply