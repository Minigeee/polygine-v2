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

#include <ply/graphics/Pipeline.h>
#include <ply/graphics/RenderDevice.h>

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

static const char* VSSource = R"(
    struct PSInput 
    { 
        float4 Pos   : SV_POSITION; 
        float3 Color : COLOR; 
    };
    
    void main(in  uint    VertId : SV_VertexID,
              out PSInput PSIn) 
    {
        float4 Pos[3];
        Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
        Pos[1] = float4( 0.0, +0.5, 0.0, 1.0);
        Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);
    
        float3 Col[3];
        Col[0] = float3(1.0, 0.0, 0.0); // red
        Col[1] = float3(0.0, 1.0, 0.0); // green
        Col[2] = float3(0.0, 0.0, 1.0); // blue
    
        PSIn.Pos   = Pos[VertId];
        PSIn.Color = Col[VertId];
    }
    )";

// Pixel shader simply outputs interpolated vertex color
static const char* PSSource = R"(
    struct PSInput 
    { 
        float4 Pos   : SV_POSITION; 
        float3 Color : COLOR; 
    };
    
    struct PSOutput
    { 
        float4 Color : SV_TARGET; 
    };
    
    void main(in  PSInput  PSIn,
              out PSOutput PSOut)
    {
        PSOut.Color = float4(PSIn.Color.rgb, 1.0);
    }
    )";

int main(int argc, char* argv[]) {
    // WIP :

    // ecsTest();
    // schedulerTest();

    ply::Window window;
    window.create(800, 600, "Window");

    // Enable gamepad support
    ply::Gamepad::enable();

    // Initialize renderer
    ply::RenderDevice device;
    device.initialize(&window);

    auto vs =
        device.shader().type(ply::Shader::Vertex).fromSource(VSSource).load();
    auto ps =
        device.shader().type(ply::Shader::Pixel).fromSource(PSSource).load();

    auto pipeline =
        device.pipeline()
            .addShader(&vs)
            .addShader(&ps)
            .cull(ply::CullMode::None)
            .depth(false)
            .create();

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

        device.bindBackBuffer();

        device.draw(&pipeline, 3);
        device.present();

        ply::sleep(1.0f / 60.0f);
    }

    return 0;
}
