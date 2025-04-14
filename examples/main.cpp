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

#include <ply/graphics/Pipeline.h>
#include <ply/graphics/RenderDevice.h>

#include <Common/interface/AdvancedMath.hpp>
#include <iostream>

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
    };
    constexpr Vertex CubeVerts[] = {
        {ply::Vector3f{-1, -1, -1}, ply::Vector2f{0, 1}},
        {ply::Vector3f{-1, +1, -1}, ply::Vector2f{0, 0}},
        {ply::Vector3f{+1, +1, -1}, ply::Vector2f{1, 0}},
        {ply::Vector3f{+1, -1, -1}, ply::Vector2f{1, 1}},

        {ply::Vector3f{-1, -1, -1}, ply::Vector2f{0, 1}},
        {ply::Vector3f{-1, -1, +1}, ply::Vector2f{0, 0}},
        {ply::Vector3f{+1, -1, +1}, ply::Vector2f{1, 0}},
        {ply::Vector3f{+1, -1, -1}, ply::Vector2f{1, 1}},

        {ply::Vector3f{+1, -1, -1}, ply::Vector2f{0, 1}},
        {ply::Vector3f{+1, -1, +1}, ply::Vector2f{1, 1}},
        {ply::Vector3f{+1, +1, +1}, ply::Vector2f{1, 0}},
        {ply::Vector3f{+1, +1, -1}, ply::Vector2f{0, 0}},

        {ply::Vector3f{+1, +1, -1}, ply::Vector2f{0, 1}},
        {ply::Vector3f{+1, +1, +1}, ply::Vector2f{0, 0}},
        {ply::Vector3f{-1, +1, +1}, ply::Vector2f{1, 0}},
        {ply::Vector3f{-1, +1, -1}, ply::Vector2f{1, 1}},

        {ply::Vector3f{-1, +1, -1}, ply::Vector2f{1, 0}},
        {ply::Vector3f{-1, +1, +1}, ply::Vector2f{0, 0}},
        {ply::Vector3f{-1, -1, +1}, ply::Vector2f{0, 1}},
        {ply::Vector3f{-1, -1, -1}, ply::Vector2f{1, 1}},

        {ply::Vector3f{-1, -1, +1}, ply::Vector2f{1, 1}},
        {ply::Vector3f{+1, -1, +1}, ply::Vector2f{0, 1}},
        {ply::Vector3f{+1, +1, +1}, ply::Vector2f{0, 0}},
        {ply::Vector3f{-1, +1, +1}, ply::Vector2f{1, 0}},
    };

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

ply::Matrix4f createTransformMatrix(float time) {
    ply::Matrix4f T = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(time * 45.0f),
        ply::Vector3f{0, 1, 0}
    );

    ply::Matrix4f V = glm::lookAt(
        ply::Vector3f{5, 0, 5},
        ply::Vector3f{0, 0, 0},
        ply::Vector3f{0, 1, 0}
    );

    ply::Matrix4f P =
        ply::toPerspectiveMatrix(45.0f, 8.0f / 6.0f, 0.1f, 100.0f);

    return P * V * T;
}

int main(int argc, char* argv[]) {
    // WIP :
    // - [X] Pipeline resource layout
    // - [X] Pipeline static variable setter
    // - [X] Pipeline resource binding api
    // - [X] Buffer mapping util
    // - [X] Render context methods: set buffers, clear, set pipeline, commit
    // resources
    //    - Try to figure out a way to more cleanly expose these functionalities
    // - [X] Indexed draw
    // - [X] Finish setting up cube demo: create shaders, apply transform
    // matrices

    // ecsTest();
    // schedulerTest();

    ply::Window window;
    window.create(800, 600, "Window");

    // Enable gamepad support
    ply::Gamepad::enable();

    // Initialize renderer
    ply::RenderDevice device;
    device.initialize(&window);

    auto vs = device.shader().type(ply::Shader::Vertex).file("cube.vsh").load();
    auto ps = device.shader().type(ply::Shader::Pixel).file("cube.psh").load();

    auto pipeline =
        device.pipeline()
            .shader(&vs)
            .shader(&ps)
            .addInputLayout(0, 0, 3, ply::Type::Float32) // Position
            .addInputLayout(1, 0, 2, ply::Type::Float32) // UV
            .addVariable(
                "g_Texture",
                ply::Shader::Pixel,
                ply::ShaderResourceType::Mutable
            ) // Texture
            .addSampler("g_Texture", ply::Shader::Pixel)
            .cull(ply::CullMode::None)
            .create();

    // Create constants buffer
    auto buffer =
        device.buffer()
            .bind(ply::ResourceBind::UniformBuffer)
            .usage(ply::ResourceUsage::Dynamic)
            .access(ply::ResourceAccess::Write)
            .size(sizeof(ply::Matrix4f))
            .create();
    pipeline.setStaticVariable(ply::Shader::Vertex, "Constants", buffer);

    // Create default resource binding
    auto binding = pipeline.createResourceBinding();

    auto vertexBuffer = createVertexBuffer(device);
    auto indexBuffer = createIndexBuffer(device);

    // Load image texture
    ply::Image image("examples/assets/DGLogo.png");
    auto texture = device.texture(image);
    binding.set(ply::Shader::Pixel, "g_Texture", texture);

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

        // Update transform matrix
        float time = clock.getElapsedTime().seconds();
        auto transform = createTransformMatrix(time);
        ply::Matrix4f* mapped = (ply::Matrix4f*)buffer.map(
            ply::MapMode::Write,
            ply::MapFlag::Discard
        );
        mapped[0] = glm::transpose(transform);
        buffer.unmap();

        device.context.bindBackBuffer();
        device.context.clear(ply::ClearFlag::Color | ply::ClearFlag::Depth);

        device.context.setVertexBuffers({&vertexBuffer});
        device.context.setIndexBuffer(indexBuffer);
        device.context.drawIndexed(pipeline, 36, &binding);
        device.context.present();
    }

    return 0;
}
