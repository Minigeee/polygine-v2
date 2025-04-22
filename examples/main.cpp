#include <ply/EntryPoint.h>

#include <ply/core/HandleArray.h>
#include <ply/core/Scheduler.h>
#include <ply/core/Sleep.h>
#include <ply/core/Types.h>
#include <ply/ecs/World.h>
#include <ply/engine/Events.h>
#include <ply/engine/Gamepad.h>
#include <ply/engine/Input.h>
#include <ply/engine/Window.h>
#include <ply/math/Transform.h>

#include <ply/graphics/Camera.h>
#include <ply/graphics/Pipeline.h>
#include <ply/graphics/RenderDevice.h>
#include <ply/graphics/RenderSystem.h>
#include <ply/graphics/Renderer.h>

#include <ply/components/Lights.h>
#include <ply/components/Spatial.h>

#include <Common/interface/AdvancedMath.hpp>
#include <iostream>
#include <ply/math/Functions.h>
#include <random>

struct Position {
    float x, y, z;
};

struct Velocity {
    float x, y, z;
};

void ecsTest() {
    ply::Scheduler scheduler;
    scheduler.setNumWorkers(4);

    ply::EventSystem events;
    ply::World world;
    world.setScheduler(&scheduler);

    world.observer(ply::World::OnCreate)
        .match<Position>()
        .each([](ply::QueryIterator it, Position& pos) {
            pos.x = 1.5f;
            printf("%f %f %f %d\n", pos.x, pos.y, pos.z, (uint32_t)it.id);
        });
    world.observer(ply::World::OnRemove)
        .match<Position>()
        .each([&events](ply::QueryIterator it, Position& pos) {
            pos.x += 2.0f;
            printf("%f %f %f %d\n", pos.x, pos.y, pos.z, (uint32_t)it.id);
            events.sendEvent(pos);
        });
    world.observer(ply::World::OnEnter)
        .match<Velocity>()
        .each([&world](ply::QueryIterator it, const Velocity& vel) {
            printf("enter velocity query\n");
            world.entity().add(Velocity{1.0f, 0.0f, 0.0f}).create();
        });

    ply::System* a = world.system().match<Position>().each(
        [](ply::QueryIterator it, Position& pos) {
            printf("a\n");
        }
    );
    // clang-format off
    ply::System* b = world.system()
        .before(a)
        .match<Position>()
        .each([](ply::QueryIterator it, Position& pos) {
            printf("b\n");
        });
    // clang-format on
    world.system().after(b).each([](ply::QueryIterator it) {
        printf("c\n");
    });

    auto entities =
        world.entity()
            .add(Position{0.0f, 1.0f, 0.0f})
            .create([](Position& pos) {
                pos.z = 2.5f;
            });

    auto query = world.query().match<Position>().compile();

    world.addComponent(entities[0], Velocity{1.0f, 0.0f, 0.0f});
    query.each([](ply::QueryIterator it, const Position& pos) {
        bool hasVel = it.has<Velocity>();
        printf("queried entity\n");
    });

    world.removeComponent<Position>(entities[0]);
    query.each([](ply::QueryIterator it, const Position& pos) {
        bool hasVel = it.has<Velocity>();
        printf("queried entity 2\n");
    });

    auto entity = world.getEntity(entities[0]);
    auto vel = entity.get<Velocity>();
    entity.add(Position{0.0f, 0.0f, 0.0f});

    world.remove(entities[0]);
    entity.release();

    events.addListener<Position>([](const Position& pos) {
        std::cout << "received pos event\n";
    });

    world.tick();

    events.poll();
}

void schedulerTest() {
    ply::Scheduler scheduler;
    scheduler.setNumWorkers(4);

    auto barrier = scheduler.barrier();
    auto taskA = barrier.add([]() {
        ply::sleep(1.0f);
        std::cout << "a\n";
    });
    barrier.add(
        []() {
            std::cout << "b\n";
        },
        {taskA.getHandle()}
    );
    barrier.wait();
}

///////////////////////////////////////////////////////////

ply::Buffer createVertexBuffer(ply::RenderDevice& device) {
    struct Vertex {
        ply::Vector3f pos;
        ply::Vector2f uv;
        ply::Vector3f normal;
    };

    // clang-format off
    constexpr Vertex CubeVerts[] = {
        {ply::Vector3f{-1, -1, -1}, ply::Vector2f{0, 1}, ply::Vector3f{0, 0, -1}},
        {ply::Vector3f{-1, +1, -1}, ply::Vector2f{0, 0}, ply::Vector3f{0, 0, -1}},
        {ply::Vector3f{+1, +1, -1}, ply::Vector2f{1, 0}, ply::Vector3f{0, 0, -1}},
        {ply::Vector3f{+1, -1, -1}, ply::Vector2f{1, 1}, ply::Vector3f{0, 0, -1}},

        {ply::Vector3f{-1, -1, -1}, ply::Vector2f{0, 1}, ply::Vector3f{0, -1, 0}},
        {ply::Vector3f{-1, -1, +1}, ply::Vector2f{0, 0}, ply::Vector3f{0, -1, 0}},
        {ply::Vector3f{+1, -1, +1}, ply::Vector2f{1, 0}, ply::Vector3f{0, -1, 0}},
        {ply::Vector3f{+1, -1, -1}, ply::Vector2f{1, 1}, ply::Vector3f{0, -1, 0}},

        {ply::Vector3f{+1, -1, -1}, ply::Vector2f{0, 1}, ply::Vector3f{1, 0, 0}},
        {ply::Vector3f{+1, -1, +1}, ply::Vector2f{1, 1}, ply::Vector3f{1, 0, 0}},
        {ply::Vector3f{+1, +1, +1}, ply::Vector2f{1, 0}, ply::Vector3f{1, 0, 0}},
        {ply::Vector3f{+1, +1, -1}, ply::Vector2f{0, 0}, ply::Vector3f{1, 0, 0}},

        {ply::Vector3f{+1, +1, -1}, ply::Vector2f{0, 1}, ply::Vector3f{0, 1, 0}},
        {ply::Vector3f{+1, +1, +1}, ply::Vector2f{0, 0}, ply::Vector3f{0, 1, 0}},
        {ply::Vector3f{-1, +1, +1}, ply::Vector2f{1, 0}, ply::Vector3f{0, 1, 0}},
        {ply::Vector3f{-1, +1, -1}, ply::Vector2f{1, 1}, ply::Vector3f{0, 1, 0}},

        {ply::Vector3f{-1, +1, -1}, ply::Vector2f{1, 0}, ply::Vector3f{-1, 0, 0}},
        {ply::Vector3f{-1, +1, +1}, ply::Vector2f{0, 0}, ply::Vector3f{-1, 0, 0}},
        {ply::Vector3f{-1, -1, +1}, ply::Vector2f{0, 1}, ply::Vector3f{-1, 0, 0}},
        {ply::Vector3f{-1, -1, -1}, ply::Vector2f{1, 1}, ply::Vector3f{-1, 0, 0}},

        {ply::Vector3f{-1, -1, +1}, ply::Vector2f{1, 1}, ply::Vector3f{0, 0, 1}},
        {ply::Vector3f{+1, -1, +1}, ply::Vector2f{0, 1}, ply::Vector3f{0, 0, 1}},
        {ply::Vector3f{+1, +1, +1}, ply::Vector2f{0, 0}, ply::Vector3f{0, 0, 1}},
        {ply::Vector3f{-1, +1, +1}, ply::Vector2f{1, 0}, ply::Vector3f{0, 0, 1}},
    };
    // clang-format on

    return device.buffer()
        .bind(ply::ResourceBind::VertexBuffer)
        .usage(ply::ResourceUsage::Immutable)
        .data(CubeVerts, sizeof(CubeVerts))
        .create();
}

ply::Buffer createIndexBuffer(ply::RenderDevice& device) {
    // clang-format off
    constexpr uint32_t Indices[] = {
        2,0,1,    2,3,0,
        4,6,5,    4,7,6,
        8,10,9,   8,11,10,
        12,14,13, 12,15,14,
        16,18,17, 16,19,18,
        20,21,22, 20,22,23
    };
    // clang-format on

    return device.buffer()
        .bind(ply::ResourceBind::IndexBuffer)
        .usage(ply::ResourceUsage::Immutable)
        .data(Indices, sizeof(Indices))
        .create();
}

struct InstanceData {
    ply::Matrix4f Matrix;
    float TextureInd = 0;
};

void populateInstanceBuffer(ply::Buffer& buffer, size_t gridSize) {
    // Populate instance data buffer
    const auto zGridSize = static_cast<size_t>(gridSize);
    std::vector<InstanceData> instanceData(zGridSize * zGridSize * zGridSize);

    float fGridSize = static_cast<float>(gridSize);

    std::mt19937 gen; // Standard mersenne_twister_engine. Use default seed
                      // to generate consistent distribution.

    constexpr uint32_t NUM_TEXTURE = 3;
    std::uniform_real_distribution<float> scale_distr(0.3f, 1.0f);
    std::uniform_real_distribution<float> offset_distr(-0.15f, +0.15f);
    std::uniform_real_distribution<float> rot_distr(-ply::PI, +ply::PI);
    std::uniform_int_distribution<int32_t> tex_distr(0, NUM_TEXTURE - 1);

    float BaseScale = 0.6f / fGridSize;
    int instId = 0;
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            for (int z = 0; z < gridSize; ++z) {
                // Add random offset from central position in the grid
                float xOffset =
                    2.f * (x + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                float yOffset =
                    2.f * (y + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                float zOffset =
                    2.f * (z + 0.5f + offset_distr(gen)) / fGridSize - 1.f;

                // Random scale
                float scale = BaseScale * scale_distr(gen);

                // Random rotation
                ply::Matrix4f matrix = ply::toTransformMatrix(
                    ply::Vector3f{xOffset, yOffset, zOffset},
                    ply::Vector3f(
                        rot_distr(gen),
                        rot_distr(gen),
                        rot_distr(gen)
                    ),
                    ply::Vector3f(scale)
                );

                InstanceData instance;
                instance.Matrix = matrix;
                instance.TextureInd = static_cast<float>(tex_distr(gen));

                instanceData[instId++] = instance;
            }
        }
    }

    buffer.update(instanceData);
}

constexpr size_t GRID_SIZE = 5;
constexpr size_t MAX_INSTANCES = GRID_SIZE * GRID_SIZE * GRID_SIZE;

class TestSystem : public ply::RenderSystem {
public:
    void initialize(const ply::RenderSystem::Init& context) override {
        m_vertexShader =
            m_device->shader()
                .type(ply::Shader::Vertex)
                .file("cube.vsh")
                .load();
        m_pixelShader =
            m_device->shader().type(ply::Shader::Pixel).file("cube.psh").load();

        m_pipeline =
            m_device->pipeline()
                .renderPass(context.renderPass)
                .shader(&m_vertexShader)
                .shader(&m_pixelShader)
                .addInputLayout(0, 0, 3, ply::Type::Float32)       // Position
                .addInputLayout(1, 0, 2, ply::Type::Float32)       // UV
                .addInputLayout(2, 0, 3, ply::Type::Float32)       // Normal
                .addInputLayout(3, 1, 4, ply::Type::Float32, true) // Instance
                                                                   // matrix
                .addInputLayout(4, 1, 4, ply::Type::Float32, true)
                .addInputLayout(5, 1, 4, ply::Type::Float32, true)
                .addInputLayout(6, 1, 4, ply::Type::Float32, true)
                .addInputLayout(7, 1, 1, ply::Type::Float32, true) // Tex index
                .addVariable(
                    "g_Texture",
                    ply::Shader::Pixel,
                    ply::ShaderResourceType::Mutable
                ) // Texture
                .addVariable(
                    "Camera",
                    ply::Shader::Vertex,
                    ply::ShaderResourceType::Mutable
                ) // Camera
                .addSampler("g_Texture", ply::Shader::Pixel)
                .cull(ply::CullMode::Back)
                .create();

        // Create constants buffer
        m_constantsBuffer =
            m_device->buffer()
                .bind(ply::ResourceBind::UniformBuffer)
                .usage(ply::ResourceUsage::Dynamic)
                .access(ply::ResourceAccess::Write)
                .size(2 * sizeof(ply::Matrix4f))
                .create();

        // Static variables
        m_pipeline.setStaticVariable(
            ply::Shader::Vertex,
            "Constants",
            m_constantsBuffer
        );

        // Vertex buffer
        m_vertexBuffer = createVertexBuffer(*m_device);
        // Index buffer
        m_indexBuffer = createIndexBuffer(*m_device);

        // Instance buffer
        m_instanceBuffer =
            m_device->buffer()
                .bind(ply::ResourceBind::VertexBuffer)
                .usage(ply::ResourceUsage::Default)
                .size(MAX_INSTANCES * sizeof(InstanceData))
                .create();
        populateInstanceBuffer(m_instanceBuffer, GRID_SIZE);

        // Create default resource binding
        m_binding = m_pipeline.createResourceBinding();
        m_binding.set(
            ply::Shader::Vertex,
            "Camera",
            context.buffers.camera
        );

        // Load image texture
        ply::Image image1("examples/assets/DGLogo1.png");
        ply::Image image2("examples/assets/DGLogo2.png");
        ply::Image image3("examples/assets/DGLogo3.png");
        // clang-format off
        m_texture = m_device->texture()
            .from(image1)
            .from(image2)
            .from(image3)
            .mips(5)
            .create();
        // clang-format on
        m_binding.set(ply::Shader::Pixel, "g_Texture", m_texture);

        // Register resources
        addResource(m_constantsBuffer, ply::ResourceState::ConstantBuffer);
        addResource(m_instanceBuffer, ply::ResourceState::VertexBuffer);
        addResource(m_vertexBuffer, ply::ResourceState::VertexBuffer);
        addResource(m_indexBuffer, ply::ResourceState::IndexBuffer);
        addResource(m_texture, ply::ResourceState::ShaderResource);
    }

    void update(float dt) override {
        ply::Matrix4f* mapped = (ply::Matrix4f*)m_constantsBuffer.map(
            ply::MapMode::Write,
            ply::MapFlag::Discard
        );

        mapped[0] = glm::rotate(
            glm::mat4(1.0f),
            glm::radians((float)m_clock.getElapsedTime().seconds() * 45.0f),
            ply::Vector3f{0, 1, 0}
        );

        m_constantsBuffer.unmap();
    }

    void render(ply::RenderPassContext& context) override {
        m_binding.setOffset(
            ply::Shader::Vertex,
            "Camera",
            context.offsets.camera
        );

        m_device->context.setPipeline(m_pipeline);
        m_device->context.setVertexBuffers({&m_vertexBuffer, &m_instanceBuffer}
        );
        m_device->context.setIndexBuffer(m_indexBuffer);
        m_device->context.setResourceBinding(m_binding);
        m_device->context.drawIndexed(36, MAX_INSTANCES);
    }

private:
    ply::Clock m_clock;
    ply::Pipeline m_pipeline;
    ply::ResourceBinding m_binding;
    ply::Shader m_vertexShader;
    ply::Shader m_pixelShader;
    ply::Buffer m_constantsBuffer;
    ply::Buffer m_instanceBuffer;
    ply::Buffer m_vertexBuffer;
    ply::Buffer m_indexBuffer;
    ply::Texture m_texture;
};

int main(int argc, char* argv[]) {
    // WIP :
    // - [X] Implement ambient + directional lighting
    // - [X] Implement point light volumes
    // - [ ] Clean up:
    //   - [X] Implement constant buffer offsets (create constant buffer for easy data pushing, create streaming buffer for easy instance data streaming)
    //   - [X] Handle resizes
    //   - [X] Add name function to gpu resource builders (and use them in the renderer class)
    //   - [X] Handle when camera is inside point light volume

    // Logger
    loguru::add_file(
        "game.log",
        loguru::FileMode::Truncate,
        loguru::Verbosity_1
    );

    // ecsTest();
    // schedulerTest();
    {
        ply::Window window;
        window.create(800, 600, "Window");

        // Enable gamepad support
        ply::Gamepad::enable();

        // Initialize renderer
        ply::RenderDevice device;
        device.initialize(&window);

        // Renderer
        ply::Renderer renderer;
        renderer.initialize(&device);

        TestSystem testSystem;
        renderer.add(&testSystem);

        // World
        ply::World world;
        renderer.setWorld(&world);

        // Camera
        ply::Camera camera;
        camera.setPosition({0.0f, 0.0f, -1.0f});
        camera.setDirection({0.0f, 0.0f, 1.0f});
        camera.setPerspective(90.0f, 8.0f / 6.0f, 0.1f, 100.0f);

        // Lights
        world.entity()
            .add(ply::Transform())
            .add(ply::PointLight())
            .create([](ply::Transform& t, ply::PointLight& light) {
                t.position = {0.0f, 0.0f, 0.0f};
                light.coefficients.y *= 10.0f; // Reduce light intensity
                light.coefficients.z *= 10.0f;
            });

        // Move it around
        ply::Clock persistentClock;
        world.system().match<ply::Transform, ply::PointLight>().each(
            [&persistentClock](
                ply::QueryIterator it,
                ply::Transform& t,
                ply::PointLight& light
            ) {
                t.position.y =
                    0.5f *
                    std::sin(persistentClock.getElapsedTime().seconds() * 2.0f);
            }
        );

        window.addListener<ply::Event::MouseButton>(
            [](const ply::Event::MouseButton& event) {
                std::cout << "Mouse button " << (int)event.button << " "
                          << (int)event.action << "\n";
            }
        );

        window.addListener<ply::Event::Key>([](const ply::Event::Key& event) {
            std::cout << "Key " << (int)event.key << " " << (int)event.action
                      << "\n";
        });

        ply::Gamepad::getHandler().addListener<ply::Event::GamepadConnection>(
            [](const ply::Event::GamepadConnection& event) {
                std::cout << "Gamepad connection " << (int)event.id << " "
                          << (int)event.connected << "\n";
            }
        );

        auto gamepads = ply::Gamepad::getDevices();

        ply::Clock clock;

        while (!window.shouldClose()) {
            ply::Input::poll();

            if (gamepads.size() > 0) {
                if (ply::Gamepad::isButtonPressed(
                        gamepads[0],
                        ply::Gamepad::Button::South
                    ))
                    std::cout << "Gamepad button pressed B\n";
                if (ply::Gamepad::isButtonPressed(
                        gamepads[0],
                        ply::Gamepad::Button::East
                    ))
                    std::cout << "Gamepad button pressed A\n";
                if (ply::Gamepad::isButtonPressed(
                        gamepads[0],
                        ply::Gamepad::Button::North
                    ))
                    std::cout << "Gamepad button pressed X\n";
                if (ply::Gamepad::isButtonPressed(
                        gamepads[0],
                        ply::Gamepad::Button::West
                    ))
                    std::cout << "Gamepad button pressed Y\n";
            }

            // Updates
            float dt = clock.restart().seconds();
            world.tick();
            renderer.update(dt);

            // Render
            renderer.render(camera);

            device.context.present();
        }
    }

    return 0;
}
