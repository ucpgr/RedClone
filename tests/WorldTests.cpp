#include "world/World.h"

#include <iostream>

int runWorldTests()
{
    redclone::world::World world;
    auto& ecs = world.getEntityManager();

    redclone::ecs::EntityId selected = 0;
    redclone::ecs::EntityId unselected = 0;
    for (const auto entityId : ecs.getAliveEntities())
    {
        if (selected == 0)
        {
            selected = entityId;
        }
        else
        {
            unselected = entityId;
            break;
        }
    }

    auto* selectedTransform = ecs.getTransform(selected);
    if (selectedTransform == nullptr)
    {
        std::cerr << "World missing test units\n";
        return 1;
    }

    world.selectUnitAt(selectedTransform->position);
    world.issueMoveCommandToSelected({500.0F, 500.0F});
    if (!ecs.hasMoveTarget(selected))
    {
        std::cerr << "Selected units should receive move command\n";
        return 1;
    }
    if (unselected != 0 && ecs.hasMoveTarget(unselected))
    {
        std::cerr << "Non-selected units should not receive move command\n";
        return 1;
    }

    const auto start = selectedTransform->position;
    world.update(0.1F);
    const auto after = ecs.getTransform(selected)->position;
    if (after[0] <= start[0] && after[1] <= start[1])
    {
        std::cerr << "Unit did not move toward destination\n";
        return 1;
    }

    ecs.addMoveTarget(selected, {after + redclone::engine::math::Vec2f(1.0F, 0.0F)});
    world.update(1.0F);
    if (ecs.hasMoveTarget(selected))
    {
        std::cerr << "Unit should stop at destination without overshoot\n";
        return 1;
    }

    return 0;
}
