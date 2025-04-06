#include <ply/core/HandleArray.h>
#include <ply/core/Types.h>
#include <ply/engine/Events.h>

#include <iostream>

struct Position {
    float x, y, z;
};

struct Velocity {
    float x, y, z;
};

#include <ply/ecs/World.h>

void test(int x, float y) {}
using Test = typename std::conditional_t<
    false,
    rest_param_types<std::decay_t<decltype(test)>>::type,
    param_types<std::decay_t<decltype(test)>>::type>;

int main() {
    ply::EventSystem events;
    ply::World world;

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

    auto entities =
        world.entity().add(Position{0.0f, 1.0f, 0.0f}).create([](Position& pos) { pos.z = 2.5f; });

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

    events.addListener<Position>([](const Position& pos) { std::cout << "received pos event\n"; });

    std::cout << "Hello, World!\n";

    world.tick();

    events.poll();

    // WIP :
    // - [X] Implement enter/exit dispatcher
    // - [ ] Implement deferred component change processor
    // - [ ] Implement entity create defer option
    // - [ ] Implement deferred entity create processor

    return 0;
}
