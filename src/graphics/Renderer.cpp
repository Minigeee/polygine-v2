#include <ply/graphics/Renderer.h>

#include <ply/graphics/RenderDevice.h>

///////////////////////////////////////////////////////////
#define LIGHT_UNIFORM_BUFFER_SIZE 10
#define CAMERA_UNIFORM_BUFFER_SIZE 10
#define ANIM_UNIFORM_BUFFER_SIZE 20

///////////////////////////////////////////////////////////
#define MAX_NUM_DIR_LIGHTS 2
#define MAX_NUM_POINT_LIGHTS 100
#define MAX_NUM_SHADOW_CASCADES 3

///////////////////////////////////////////////////////////
#define MAX_NUM_SKELETAL_BONES 50

namespace ply {

///////////////////////////////////////////////////////////
struct CB_Camera {
    Matrix4f m_projView;
    Vector4f m_cameraPos;
};

///////////////////////////////////////////////////////////
struct CS_DirLight {
    Vector4f m_diffuse;
    Vector4f m_specular;
    Vector3f m_direction;
    int m_hasShadows;
};

///////////////////////////////////////////////////////////
struct CB_Lights {
    Vector4f m_ambient;
    CS_DirLight m_dirLights[MAX_NUM_DIR_LIGHTS];
    // UniformStruct_PointLight		m_pointLights[MAX_NUM_POINT_LIGHTS];
    int m_numDirLights;
};

///////////////////////////////////////////////////////////
struct CB_Skeleton {
    Matrix4f m_bones[MAX_NUM_SKELETAL_BONES];
};

///////////////////////////////////////////////////////////
Renderer::Renderer() :
    m_ambient(0.1f) {}

///////////////////////////////////////////////////////////
Renderer::~Renderer() {
    // Free all g-buffers
    for (auto it = m_gBuffers.begin(); it != m_gBuffers.end(); ++it) {
        Framebuffer* buffer = it.value();

        // Free textures
        for (size_t i = 0; i < buffer->getNumColorTextures(); ++i)
            Pool<Texture>::free(buffer->getColorTexture(i));

        delete buffer;
    }

    m_gBuffers.clear();
}

///////////////////////////////////////////////////////////
void Renderer::initialize(RenderDevice* device) {
    uint32_t align = device->getConstantBufferAlignment();

    // Allocate camera buffer
    uint32_t size = (sizeof(CB_Camera) + align - 1) / align * align;
    m_cameraBuffer =
        device->buffer()
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * CAMERA_UNIFORM_BUFFER_SIZE)
            .create();

    // Allocate lights buffer
    size = (sizeof(CB_Lights) + align - 1) / align * align;
    m_lightsBuffer =
        device->buffer()
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * LIGHT_UNIFORM_BUFFER_SIZE)
            .create();

    // Allocate animation buffer
    size = (sizeof(CB_Skeleton) + align - 1) / align * align;
    m_animationBuffer =
        device->buffer()
            .access(ResourceAccess::Write)
            .bind(ResourceBind::UniformBuffer)
            .usage(ResourceUsage::Dynamic)
            .size(size * ANIM_UNIFORM_BUFFER_SIZE)
            .create();
}

} // namespace ply