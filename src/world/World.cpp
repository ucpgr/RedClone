#include "world/World.h"

#include <algorithm>
#include <cmath>

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
        m_EntityManager.addTransform(id, {{220.0F + static_cast<float>(i * 40), 200.0F + static_cast<float>(i * 25)}});
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
    constexpr engine::math::Vec2f unitSize{18.0F, 18.0F};
    for (const auto entityId : m_EntityManager.getAliveEntities())
    {
        const auto* transform = m_EntityManager.getTransform(entityId);
        const auto* unit = m_EntityManager.getUnit(entityId);
        if (transform == nullptr || unit == nullptr)
        {
            continue;
        }

        const engine::rendering::Color fillColor = unit->ownerId == 0 ? engine::rendering::Color{90, 160, 255, 255}
                                                                       : engine::rendering::Color{255, 110, 110, 255};
        const bool selected = m_EntityManager.hasSelection(entityId);
        const auto topLeft = transform->position - (unitSize * 0.5F);
        renderer.drawOutlinedRect(topLeft, unitSize, fillColor,
                                  selected ? engine::rendering::Color{255, 255, 0, 255}
                                           : engine::rendering::Color{20, 20, 20, 255},
                                  selected ? -3.0F : -1.0F);
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

ecs::EntityManager& World::getEntityManager() { return m_EntityManager; }
const ecs::EntityManager& World::getEntityManager() const { return m_EntityManager; }
} // namespace redclone::world
