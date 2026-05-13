#pragma once

#include "engine/math/MathTypes.h"

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace redclone::ecs
{
using EntityId = std::uint32_t;

struct TransformComponent
{
    engine::math::Vec2f position{0.0F, 0.0F};
};

struct UnitComponent
{
    float movementSpeed = 80.0F;
    float health = 100.0F;
    int ownerId = 0;
};

struct SelectionComponent
{
};

struct MoveTargetComponent
{
    engine::math::Vec2f destination{0.0F, 0.0F};
};

class EntityManager
{
  public:
    [[nodiscard]] EntityId createEntity();
    void destroyEntity(EntityId entityId);

    [[nodiscard]] bool isAlive(EntityId entityId) const;

    void addTransform(EntityId entityId, const TransformComponent& component);
    [[nodiscard]] bool hasTransform(EntityId entityId) const;
    [[nodiscard]] TransformComponent* getTransform(EntityId entityId);
    [[nodiscard]] const TransformComponent* getTransform(EntityId entityId) const;
    void removeTransform(EntityId entityId);

    void addUnit(EntityId entityId, const UnitComponent& component);
    [[nodiscard]] bool hasUnit(EntityId entityId) const;
    [[nodiscard]] const UnitComponent* getUnit(EntityId entityId) const;
    void removeUnit(EntityId entityId);

    void addSelection(EntityId entityId, const SelectionComponent& component = {});
    [[nodiscard]] bool hasSelection(EntityId entityId) const;
    void removeSelection(EntityId entityId);

    void addMoveTarget(EntityId entityId, const MoveTargetComponent& component);
    [[nodiscard]] bool hasMoveTarget(EntityId entityId) const;
    [[nodiscard]] MoveTargetComponent* getMoveTarget(EntityId entityId);
    void removeMoveTarget(EntityId entityId);

    [[nodiscard]] const std::unordered_set<EntityId>& getAliveEntities() const;

  private:
    EntityId m_NextEntityId = 1;
    std::unordered_set<EntityId> m_AliveEntities;
    std::unordered_map<EntityId, TransformComponent> m_Transforms;
    std::unordered_map<EntityId, UnitComponent> m_Units;
    std::unordered_map<EntityId, SelectionComponent> m_Selections;
    std::unordered_map<EntityId, MoveTargetComponent> m_MoveTargets;
};
} // namespace redclone::ecs
