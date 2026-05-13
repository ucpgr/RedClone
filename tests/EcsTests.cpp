#include "ecs/EntityManager.h"

#include <iostream>

int runEcsTests()
{
    redclone::ecs::EntityManager ecs;
    const auto entity = ecs.createEntity();
    if (!ecs.isAlive(entity))
    {
        std::cerr << "Entity create failed\n";
        return 1;
    }

    ecs.addTransform(entity, {{10.0F, 20.0F}});
    ecs.addUnit(entity, {60.0F, 50.0F, 1});
    ecs.addMoveTarget(entity, {{30.0F, 20.0F}});

    if (!ecs.hasTransform(entity) || !ecs.hasUnit(entity) || !ecs.hasMoveTarget(entity))
    {
        std::cerr << "Entity component add/has failed\n";
        return 1;
    }

    ecs.removeMoveTarget(entity);
    if (ecs.hasMoveTarget(entity))
    {
        std::cerr << "Entity component remove failed\n";
        return 1;
    }

    return 0;
}
