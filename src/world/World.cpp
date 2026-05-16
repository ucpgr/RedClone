#include "world/World.h"
#include "core/Logger.h"

#include "engine/math/Isometric.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

namespace redclone::world
{
namespace
{
float length(const engine::math::Vec2f& v)
{
    return std::sqrt((v[0] * v[0]) + (v[1] * v[1]));
}
}

World::World() { spawnTestUnits(); }

void World::spawnTestUnits()
{
    for (int i = 0; i < 4; ++i)
    {
        const auto id = m_EntityManager.createEntity();
        m_EntityManager.addTransform(id, {{8.0F + static_cast<float>(i), 8.0F + static_cast<float>(i)}});
        m_EntityManager.addUnit(id, {80.0F, 100.0F, i % 2});
        REDCLONE_LOG_DEBUG(std::string("Spawned test unit entity=") + std::to_string(id));
    }
}

void World::update(const float deltaSeconds)
{
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        auto* transform = m_EntityManager.getTransform(entityId);
        auto* unit = m_EntityManager.getUnit(entityId);
        auto* moveTarget = m_EntityManager.getMoveTarget(entityId);

        if (transform == nullptr || unit == nullptr || moveTarget == nullptr)
        {
            continue;
        }

        const auto delta = moveTarget->destination - transform->position;
        const float distance = length(delta);
        if (distance <= c_ArrivalDistance)
        {
            transform->position = moveTarget->destination;
            m_EntityManager.removeMoveTarget(entityId);
            continue;
        }

        const float maxStep = unit->movementSpeed * deltaSeconds;
        const float step = std::min(maxStep, distance);
        const auto direction = delta / distance;
        transform->position += direction * step;

        if (step >= distance)
        {
            transform->position = moveTarget->destination;
            m_EntityManager.removeMoveTarget(entityId);
        }
    }
}

void World::render(engine::rendering::IRenderer& renderer, const TileMap& tileMap) const
{
    struct DrawUnit
    {
        ecs::EntityId entityId{};
        engine::math::Vec2f worldPos{};
        int ownerId{};
        bool selected{};
        float terrainHeight{};
        float sortKey{};
    };

    std::vector<DrawUnit> drawUnits;
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        const auto* transform = m_EntityManager.getTransform(entityId);
        const auto* unit = m_EntityManager.getUnit(entityId);
        if (transform == nullptr || unit == nullptr)
        {
            continue;
        }

        const float terrainHeight = tileMap.sampleHeightAt(transform->position);
        drawUnits.push_back({entityId, transform->position, unit->ownerId, m_EntityManager.hasSelection(entityId),
                             terrainHeight, transform->position[0] + transform->position[1] + (terrainHeight * 0.01F)});
    }

    std::ranges::sort(drawUnits, [](const DrawUnit& lhs, const DrawUnit& rhs) { return lhs.sortKey < rhs.sortKey; });

    for (const auto& drawUnit : drawUnits)
    {
        const auto isoPosition =
            engine::math::isometric::worldToIso({drawUnit.worldPos[0], drawUnit.worldPos[1], drawUnit.terrainHeight});
        const engine::rendering::Color fillColor = drawUnit.ownerId == 0 ? engine::rendering::Color{90, 160, 255, 255}
                                                                          : engine::rendering::Color{255, 110, 110, 255};
        const std::array points = {
            engine::math::Vec2f{isoPosition[0], isoPosition[1] - 9.0F},
            engine::math::Vec2f{isoPosition[0] + 9.0F, isoPosition[1]},
            engine::math::Vec2f{isoPosition[0], isoPosition[1] + 9.0F},
            engine::math::Vec2f{isoPosition[0] - 9.0F, isoPosition[1]},
        };
        renderer.drawConvexPolygon(points, fillColor,
                                   drawUnit.selected ? engine::rendering::Color{255, 255, 0, 255}
                                                     : engine::rendering::Color{20, 20, 20, 255},
                                   drawUnit.selected ? -3.0F : -1.0F);
    }
}

void World::selectUnitAt(const engine::math::Vec2f& isoPosition, const TileMap& tileMap)
{
    clearUnitSelection();

    ecs::EntityId bestEntity = 0;
    float bestDistance = c_SelectionRadius;
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        const auto* transform = m_EntityManager.getTransform(entityId);
        const auto* unit = m_EntityManager.getUnit(entityId);
        if (transform == nullptr || unit == nullptr)
        {
            continue;
        }
        (void)unit;

        const float terrainHeight = tileMap.sampleHeightAt(transform->position);
        const auto unitIsoPosition =
            engine::math::isometric::worldToIso({transform->position[0], transform->position[1], terrainHeight});
        const auto delta = unitIsoPosition - isoPosition;
        const float distance = length(delta);
        if (distance <= bestDistance)
        {
            bestDistance = distance;
            bestEntity = entityId;
        }
    }

    if (bestEntity != 0)
    {
        m_EntityManager.addSelection(bestEntity, {});
        REDCLONE_LOG_DEBUG(std::string("Unit selected entity=") + std::to_string(bestEntity));
        return;
    }

    REDCLONE_LOG_DEBUG("Unit selection failed.");
}

void World::clearUnitSelection()
{
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        m_EntityManager.removeSelection(entityId);
    }
}

void World::issueMoveCommandToSelected(const engine::math::Vec2f& destination)
{
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        if (!m_EntityManager.hasSelection(entityId))
        {
            continue;
        }
        if (m_EntityManager.hasTransform(entityId) && m_EntityManager.hasUnit(entityId))
        {
            m_EntityManager.addMoveTarget(entityId, {destination});
            REDCLONE_LOG_DEBUG(std::string("Move command entity=") + std::to_string(entityId) + " destination=(" +
                               std::to_string(destination[0]) + "," + std::to_string(destination[1]) + ")");
        }
    }
}

bool World::hasSelectedUnits() const
{
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        if (m_EntityManager.hasSelection(entityId))
        {
            return true;
        }
    }
    return false;
}

ecs::EntityManager& World::getEntityManager() { return m_EntityManager; }
const ecs::EntityManager& World::getEntityManager() const { return m_EntityManager; }
} // namespace redclone::world
