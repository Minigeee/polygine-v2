#include <ply/core/HandleArray.h>
#include <ply/core/Types.h>

#include <iostream>

struct Position {
    float x, y, z;
};

#include <ply/ecs/World.h>
#include <ply/ecs/World.h>

void test(int x, float y) {}
using Test = typename std::conditional_t<
    false,
    rest_param_types<std::decay_t<decltype(test)>>::type,
    param_types<std::decay_t<decltype(test)>>::type>;

int main() {
    ply::World world;
    world.observer(ply::World::OnCreate)
        .match<Position>()
        .each([](ply::QueryIterator it, Position& pos) {
            pos.x = 1.5f;
            printf("%f %f %f %d\n", pos.x, pos.y, pos.z, (uint32_t)it.id);
        });
    world.observer(ply::World::OnRemove)
        .match<Position>()
        .each([](ply::QueryIterator it, Position& pos) {
            pos.x += 2.0f;
            printf("%f %f %f %d\n", pos.x, pos.y, pos.z, (uint32_t)it.id);
        });

    auto entity = world.entity().add(Position{0.0f, 1.0f, 0.0f}).create();

    auto query = world.query().match<Position>().compile();
    query.each([](const Position& pos) {
        printf("queried entity\n");
    });

    world.remove(entity[0]);

    std::cout << "Hello, World!\n";

    world.tick();

    return 0;
}
