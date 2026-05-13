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

    ecs.destroyEntity(entity);
    if (ecs.isAlive(entity) || ecs.hasTransform(entity) || ecs.hasUnit(entity) || ecs.hasSelection(entity) ||
        ecs.hasMoveTarget(entity))
    {
        std::cerr << "Destroy entity should remove all components\n";
        return 1;
    }

    ecs.addTransform(entity, {{1.0F, 1.0F}});
    ecs.addUnit(entity, {20.0F, 20.0F, 0});
    ecs.addSelection(entity, {});
    ecs.addMoveTarget(entity, {{2.0F, 2.0F}});
    if (ecs.hasTransform(entity) || ecs.hasUnit(entity) || ecs.hasSelection(entity) || ecs.hasMoveTarget(entity))
    {
        std::cerr << "Components should not be added to destroyed entities\n";
        return 1;
    }

    constexpr redclone::ecs::EntityId nonexistentEntityId = 999999U;
    ecs.addTransform(nonexistentEntityId, {{1.0F, 1.0F}});
    ecs.addUnit(nonexistentEntityId, {20.0F, 20.0F, 0});
    ecs.addSelection(nonexistentEntityId, {});
    ecs.addMoveTarget(nonexistentEntityId, {{2.0F, 2.0F}});
    if (ecs.hasTransform(nonexistentEntityId) || ecs.hasUnit(nonexistentEntityId) || ecs.hasSelection(nonexistentEntityId) ||
        ecs.hasMoveTarget(nonexistentEntityId))
    {
        std::cerr << "Components should not be added to non-existent entities\n";
        return 1;
    }

    return 0;
}
