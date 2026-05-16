#include "world/World.h"

#include "engine/math/Isometric.h"

#include <cmath>
#include <iostream>

namespace
{
float distance(const redclone::engine::math::Vec2f& a, const redclone::engine::math::Vec2f& b)
{
    const auto delta = b - a;
    return std::sqrt((delta[0] * delta[0]) + (delta[1] * delta[1]));
}
}

int runWorldTests()
{
    redclone::world::World world;
    redclone::world::TileMap tileMap;
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

    const float selectedHeight = tileMap.sampleHeightAt(selectedTransform->position);
    const auto selectedIso =
        redclone::engine::math::isometric::worldToIso({selectedTransform->position[0], selectedTransform->position[1],
                                                       selectedHeight});
    world.selectUnitAt(selectedIso, tileMap);
    if (!world.hasSelectedUnits())
    {
        std::cerr << "Selecting a unit should mark selection state in world\n";
        return 1;
    }

    world.clearUnitSelection();
    world.selectUnitAt(selectedIso + redclone::engine::math::Vec2f{80.0F, 0.0F}, tileMap);
    if (world.hasSelectedUnits())
    {
        std::cerr << "Selecting away from rendered unit position should not mark selection state\n";
        return 1;
    }

    world.selectUnitAt(selectedIso + redclone::engine::math::Vec2f{8.0F, 0.0F}, tileMap);
    if (!world.hasSelectedUnits())
    {
        std::cerr << "Selecting near rendered unit position should mark selection state\n";
        return 1;
    }

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
    const redclone::engine::math::Vec2f farDestination{500.0F, 500.0F};
    const float startDistanceToFarDestination = distance(start, farDestination);
    world.update(0.1F);
    const auto after = ecs.getTransform(selected)->position;
    if (distance(start, after) <= 0.0F)
    {
        std::cerr << "Unit position should change after world update\n";
        return 1;
    }
    if (distance(after, farDestination) >= startDistanceToFarDestination)
    {
        std::cerr << "Unit should move closer to destination after update\n";
        return 1;
    }

    const auto nearDestination = after + redclone::engine::math::Vec2f(1.0F, 0.0F);
    ecs.addMoveTarget(selected, {nearDestination});
    world.update(1.0F);
    const auto afterNearMove = ecs.getTransform(selected)->position;
    if (distance(afterNearMove, nearDestination) > 0.001F)
    {
        std::cerr << "Unit should not overshoot nearby destination\n";
        return 1;
    }
    if (ecs.hasMoveTarget(selected))
    {
        std::cerr << "Unit should stop at destination without overshoot\n";
        return 1;
    }

    world.clearUnitSelection();
    if (world.hasSelectedUnits())
    {
        std::cerr << "Clearing selection should remove world selection state\n";
        return 1;
    }

    world.issueMoveCommandToSelected({10.0F, 10.0F});
    if (ecs.hasMoveTarget(selected) || (unselected != 0 && ecs.hasMoveTarget(unselected)))
    {
        std::cerr << "Move command without selection should not assign move targets\n";
        return 1;
    }

    return 0;
}
