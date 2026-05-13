#include "ecs/EntityManager.h"

namespace redclone::ecs
{
EntityId EntityManager::createEntity()
{
    const EntityId entityId = m_NextEntityId++;
    m_AliveEntities.insert(entityId);
    return entityId;
}

void EntityManager::destroyEntity(const EntityId entityId)
{
    m_AliveEntities.erase(entityId);
    m_Transforms.erase(entityId);
    m_Units.erase(entityId);
    m_Selections.erase(entityId);
    m_MoveTargets.erase(entityId);
}

bool EntityManager::isAlive(const EntityId entityId) const { return m_AliveEntities.contains(entityId); }

void EntityManager::addTransform(const EntityId entityId, const TransformComponent& component)
{
    m_Transforms[entityId] = component;
}
bool EntityManager::hasTransform(const EntityId entityId) const { return m_Transforms.contains(entityId); }
TransformComponent* EntityManager::getTransform(const EntityId entityId)
{
    if (const auto it = m_Transforms.find(entityId); it != m_Transforms.end())
    {
        return &it->second;
    }
    return nullptr;
}
const TransformComponent* EntityManager::getTransform(const EntityId entityId) const
{
    if (const auto it = m_Transforms.find(entityId); it != m_Transforms.end())
    {
        return &it->second;
    }
    return nullptr;
}
void EntityManager::removeTransform(const EntityId entityId) { m_Transforms.erase(entityId); }

void EntityManager::addUnit(const EntityId entityId, const UnitComponent& component) { m_Units[entityId] = component; }
bool EntityManager::hasUnit(const EntityId entityId) const { return m_Units.contains(entityId); }
UnitComponent* EntityManager::getUnit(const EntityId entityId)
{
    if (const auto it = m_Units.find(entityId); it != m_Units.end())
    {
        return &it->second;
    }
    return nullptr;
}
void EntityManager::removeUnit(const EntityId entityId) { m_Units.erase(entityId); }

void EntityManager::addSelection(const EntityId entityId, const SelectionComponent& component)
{
    m_Selections[entityId] = component;
}
bool EntityManager::hasSelection(const EntityId entityId) const { return m_Selections.contains(entityId); }
void EntityManager::removeSelection(const EntityId entityId) { m_Selections.erase(entityId); }

void EntityManager::addMoveTarget(const EntityId entityId, const MoveTargetComponent& component)
{
    m_MoveTargets[entityId] = component;
}
bool EntityManager::hasMoveTarget(const EntityId entityId) const { return m_MoveTargets.contains(entityId); }
MoveTargetComponent* EntityManager::getMoveTarget(const EntityId entityId)
{
    if (const auto it = m_MoveTargets.find(entityId); it != m_MoveTargets.end())
    {
        return &it->second;
    }
    return nullptr;
}
void EntityManager::removeMoveTarget(const EntityId entityId) { m_MoveTargets.erase(entityId); }

const std::unordered_set<EntityId>& EntityManager::getAliveEntities() const { return m_AliveEntities; }
} // namespace redclone::ecs
