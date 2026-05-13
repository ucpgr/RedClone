#include "world/World.h"

#include "engine/math/Isometric.h"

#include <algorithm>
#include <array>
#include <cmath>
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

void World::render(engine::rendering::IRenderer& renderer) const
{
    struct DrawUnit
    {
        ecs::EntityId entityId{};
        engine::math::Vec2f worldPos{};
        int ownerId{};
        bool selected{};
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

        drawUnits.push_back({entityId, transform->position, unit->ownerId, m_EntityManager.hasSelection(entityId),
                             transform->position[0] + transform->position[1]});
    }

    std::ranges::sort(drawUnits, [](const DrawUnit& lhs, const DrawUnit& rhs) { return lhs.sortKey < rhs.sortKey; });

    for (const auto& drawUnit : drawUnits)
    {
        const auto isoPosition = engine::math::isometric::worldToIso(drawUnit.worldPos);
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

void World::selectUnitAt(const engine::math::Vec2f& worldPosition)
{
    clearUnitSelection();
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        const auto* transform = m_EntityManager.getTransform(entityId);
        const auto* unit = m_EntityManager.getUnit(entityId);
        if (transform == nullptr || unit == nullptr)
        {
            continue;
        }
        (void)unit;

        const auto delta = transform->position - worldPosition;
        if (length(delta) <= c_SelectionRadius)
        {
            m_EntityManager.addSelection(entityId, {});
            return;
        }
    }
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
