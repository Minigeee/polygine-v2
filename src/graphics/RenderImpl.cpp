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
            return TEX_FORMAT_B5G6R5_UNORM; // Not available in OpenGL
        case TextureFormat::B5_G5_R5_A1:
            return TEX_FORMAT_B5G5R5A1_UNORM; // Not available in OpenGL
        case TextureFormat::Bgra8:
            return TEX_FORMAT_BGRA8_UNORM; // Not available in OpenGL
        case TextureFormat::Bgrx8:
            return TEX_FORMAT_BGRX8_UNORM; // Not available in OpenGL
        case TextureFormat::R10_G10_B10_XR_BIAS_A2:
            return TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM; // Not available in
                                                          // OpenGL
        case TextureFormat::Bgra8_Srgb:
            return TEX_FORMAT_BGRA8_UNORM_SRGB; // Not available in OpenGL
        case TextureFormat::Bgrx8_Srgb:
            return TEX_FORMAT_BGRX8_UNORM_SRGB; // Not available in OpenGL
        case TextureFormat::BC6H_UF16:
            return TEX_FORMAT_BC6H_UF16; // Not supported in OpenGLES3.1
        case TextureFormat::BC6H_SF16:
            return TEX_FORMAT_BC6H_SF16; // Not supported in OpenGLES3.1
        case TextureFormat::BC7:
            return TEX_FORMAT_BC7_UNORM; // Not supported in OpenGLES3.1
        case TextureFormat::BC7_Srgb:
            return TEX_FORMAT_BC7_UNORM_SRGB; // Not supported in OpenGLES3.1
        case TextureFormat::ETC2_Rgb8:
            return TEX_FORMAT_ETC2_RGB8_UNORM; // ETC2 formats are OpenGL
                                               // specific
        case TextureFormat::ETC2_Rgb8_Srgb:
            return TEX_FORMAT_ETC2_RGB8_UNORM_SRGB; // ETC2 formats are OpenGL
                                                    // specific
        case TextureFormat::ETC2_Rgb8_A1:
            return TEX_FORMAT_ETC2_RGB8A1_UNORM; // ETC2 formats are OpenGL
                                                 // specific
        case TextureFormat::ETC2_Rgb8_A1_Srgb:
            return TEX_FORMAT_ETC2_RGB8A1_UNORM_SRGB; // ETC2 formats are OpenGL
                                                      // specific
        case TextureFormat::ETC2_Rgba8:
            return TEX_FORMAT_ETC2_RGBA8_UNORM; // ETC2 formats are OpenGL
                                                // specific
        case TextureFormat::ETC2_Rgba8_Srgb:
            return TEX_FORMAT_ETC2_RGBA8_UNORM_SRGB; // ETC2 formats are OpenGL
                                                     // specific
        case TextureFormat::NUM_FORMATS:
            return TEX_FORMAT_UNKNOWN;
        default:
            return TEX_FORMAT_UNKNOWN;
        }
    }

    TextureFormat convertTextureFormat(TEXTURE_FORMAT format) {
        switch (format) {
        case TEX_FORMAT_UNKNOWN:
            return TextureFormat::Unknown;
        case TEX_FORMAT_RGBA32_FLOAT:
            return TextureFormat::Rgba32f;
        case TEX_FORMAT_RGBA32_UINT:
            return TextureFormat::Rgba32ui;
        case TEX_FORMAT_RGBA32_SINT:
            return TextureFormat::Rgba32i;
        case TEX_FORMAT_RGB32_FLOAT:
            return TextureFormat::Rgb32f;
        case TEX_FORMAT_RGB32_UINT:
            return TextureFormat::Rgb32ui;
        case TEX_FORMAT_RGB32_SINT:
            return TextureFormat::Rgb32i;
        case TEX_FORMAT_RGBA16_FLOAT:
            return TextureFormat::Rgba16f;
        case TEX_FORMAT_RGBA16_UNORM:
            return TextureFormat::Rgba16;
        case TEX_FORMAT_RGBA16_UINT:
            return TextureFormat::Rgba16ui;
        case TEX_FORMAT_RGBA16_SNORM:
            return TextureFormat::Rgba16snorm;
        case TEX_FORMAT_RGBA16_SINT:
            return TextureFormat::Rgba16i;
        case TEX_FORMAT_RG32_FLOAT:
            return TextureFormat::Rg32f;
        case TEX_FORMAT_RG32_UINT:
            return TextureFormat::Rg32ui;
        case TEX_FORMAT_RG32_SINT:
            return TextureFormat::Rg32i;
        case TEX_FORMAT_D32_FLOAT_S8X24_UINT:
            return TextureFormat::D32f_S8;
        case TEX_FORMAT_RGB10A2_UNORM:
            return TextureFormat::Rgb10_A2;
        case TEX_FORMAT_RGB10A2_UINT:
            return TextureFormat::Rgb10_A2ui;
        case TEX_FORMAT_R11G11B10_FLOAT:
            return TextureFormat::R11f_G11f_B10f;
        case TEX_FORMAT_RGBA8_UNORM:
            return TextureFormat::Rgba8;
        case TEX_FORMAT_RGBA8_UNORM_SRGB:
            return TextureFormat::Srgb8_A8;
        case TEX_FORMAT_RGBA8_UINT:
            return TextureFormat::Rgba8ui;
        case TEX_FORMAT_RGBA8_SNORM:
            return TextureFormat::Rgba8snorm;
        case TEX_FORMAT_RGBA8_SINT:
            return TextureFormat::Rgba8i;
        case TEX_FORMAT_RG16_FLOAT:
            return TextureFormat::Rg16f;
        case TEX_FORMAT_RG16_UNORM:
            return TextureFormat::Rg16;
        case TEX_FORMAT_RG16_UINT:
            return TextureFormat::Rg16ui;
        case TEX_FORMAT_RG16_SNORM:
            return TextureFormat::Rg16snorm;
        case TEX_FORMAT_RG16_SINT:
            return TextureFormat::Rg16i;
        case TEX_FORMAT_D32_FLOAT:
            return TextureFormat::D32f;
        case TEX_FORMAT_R32_FLOAT:
            return TextureFormat::R32f;
        case TEX_FORMAT_R32_UINT:
            return TextureFormat::R32ui;
        case TEX_FORMAT_R32_SINT:
            return TextureFormat::R32i;
        case TEX_FORMAT_D24_UNORM_S8_UINT:
            return TextureFormat::D24_S8;
        case TEX_FORMAT_RG8_UNORM:
            return TextureFormat::Rg8;
        case TEX_FORMAT_RG8_UINT:
            return TextureFormat::Rg8ui;
        case TEX_FORMAT_RG8_SNORM:
            return TextureFormat::Rg8snorm;
        case TEX_FORMAT_RG8_SINT:
            return TextureFormat::Rg8i;
        case TEX_FORMAT_R16_FLOAT:
            return TextureFormat::R16f;
        case TEX_FORMAT_D16_UNORM:
            return TextureFormat::D16;
        case TEX_FORMAT_R16_UNORM:
            return TextureFormat::R16;
        case TEX_FORMAT_R16_UINT:
            return TextureFormat::R16ui;
        case TEX_FORMAT_R16_SNORM:
            return TextureFormat::R16snorm;
        case TEX_FORMAT_R16_SINT:
            return TextureFormat::R16i;
        case TEX_FORMAT_R8_UNORM:
            return TextureFormat::R8;
        case TEX_FORMAT_R8_UINT:
            return TextureFormat::R8ui;
        case TEX_FORMAT_R8_SNORM:
            return TextureFormat::R8snorm;
        case TEX_FORMAT_R8_SINT:
            return TextureFormat::R8i;
        case TEX_FORMAT_A8_UNORM:
            return TextureFormat::A8;
        case TEX_FORMAT_R1_UNORM:
            return TextureFormat::R1;
        case TEX_FORMAT_RGB9E5_SHAREDEXP:
            return TextureFormat::Rgb9_E5;
        case TEX_FORMAT_RG8_B8G8_UNORM:
            return TextureFormat::Rg8_B8_G8;
        case TEX_FORMAT_G8R8_G8B8_UNORM:
            return TextureFormat::G8_R8_G8_B8;
        case TEX_FORMAT_BC1_UNORM:
            return TextureFormat::BC1;
        case TEX_FORMAT_BC1_UNORM_SRGB:
            return TextureFormat::BC1_Srgb;
        case TEX_FORMAT_BC2_UNORM:
            return TextureFormat::BC2;
        case TEX_FORMAT_BC2_UNORM_SRGB:
            return TextureFormat::BC2_Srgb;
        case TEX_FORMAT_BC3_UNORM:
            return TextureFormat::BC3;
        case TEX_FORMAT_BC3_UNORM_SRGB:
            return TextureFormat::BC3_Srgb;
        case TEX_FORMAT_BC4_UNORM:
            return TextureFormat::BC4;
        case TEX_FORMAT_BC4_SNORM:
            return TextureFormat::BC4snorm;
        case TEX_FORMAT_BC5_UNORM:
            return TextureFormat::BC5;
        case TEX_FORMAT_BC5_SNORM:
            return TextureFormat::BC5snorm;
        case TEX_FORMAT_B5G6R5_UNORM:
            return TextureFormat::B5_G6_R5;
        case TEX_FORMAT_B5G5R5A1_UNORM:
            return TextureFormat::B5_G5_R5_A1;
        case TEX_FORMAT_BGRA8_UNORM:
            return TextureFormat::Bgra8;
        case TEX_FORMAT_BGRX8_UNORM:
            return TextureFormat::Bgrx8;
        case TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
            return TextureFormat::R10_G10_B10_XR_BIAS_A2;
        case TEX_FORMAT_BGRA8_UNORM_SRGB:
            return TextureFormat::Bgra8_Srgb;
        case TEX_FORMAT_BGRX8_UNORM_SRGB:
            return TextureFormat::Bgrx8_Srgb;
        case TEX_FORMAT_BC6H_UF16:
            return TextureFormat::BC6H_UF16;
        case TEX_FORMAT_BC6H_SF16:
            return TextureFormat::BC6H_SF16;
        case TEX_FORMAT_BC7_UNORM:
            return TextureFormat::BC7;
        case TEX_FORMAT_BC7_UNORM_SRGB:
            return TextureFormat::BC7_Srgb;
        case TEX_FORMAT_ETC2_RGB8_UNORM:
            return TextureFormat::ETC2_Rgb8;
        case TEX_FORMAT_ETC2_RGB8_UNORM_SRGB:
            return TextureFormat::ETC2_Rgb8_Srgb;
        case TEX_FORMAT_ETC2_RGB8A1_UNORM:
            return TextureFormat::ETC2_Rgb8_A1;
        case TEX_FORMAT_ETC2_RGB8A1_UNORM_SRGB:
            return TextureFormat::ETC2_Rgb8_A1_Srgb;
        case TEX_FORMAT_ETC2_RGBA8_UNORM:
            return TextureFormat::ETC2_Rgba8;
        case TEX_FORMAT_ETC2_RGBA8_UNORM_SRGB:
            return TextureFormat::ETC2_Rgba8_Srgb;
        default:
            return TextureFormat::Unknown;
        }
    }

} // namespace priv

} // namespace ply