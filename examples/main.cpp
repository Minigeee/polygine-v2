#include <ply/core/HandleArray.h>
#include <ply/core/Types.h>
#include <ply/engine/Events.h>

#include <iostream>

struct Position {
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

    auto entities =
        world.entity().add(Position{0.0f, 1.0f, 0.0f}).create([](Position& pos) { pos.z = 2.5f; });

    auto query = world.query().match<Position>().compile();
    query.each([](const Position& pos) { printf("queried entity\n"); });

    auto entity = world.getEntity(entities[0]);
    auto pos = entity.get<Position>();
    entity.release();

    world.remove(entities[0]);

    events.addListener<Position>([](const Position& pos) {
        std::cout << "received pos event\n";
    });

    std::cout << "Hello, World!\n";

    world.removeQueuedEntities();

    events.poll();

    return 0;
}
