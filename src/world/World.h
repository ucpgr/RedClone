#pragma once

#include "ecs/EntityManager.h"
#include "engine/math/MathTypes.h"
#include "engine/rendering/IRenderer.h"

namespace redclone::world
{
class World
{
  public:
    World();

    void update(float deltaSeconds);
    void render(engine::rendering::IRenderer& renderer) const;

    void selectUnitAt(const engine::math::Vec2f& worldPosition);
    void clearUnitSelection();
    void issueMoveCommandToSelected(const engine::math::Vec2f& destination);
    [[nodiscard]] bool hasSelectedUnits() const;

    [[nodiscard]] ecs::EntityManager& getEntityManager();
    [[nodiscard]] const ecs::EntityManager& getEntityManager() const;

  private:
    static constexpr float c_SelectionRadius = 12.0F;
    static constexpr float c_ArrivalDistance = 2.0F;

    void spawnTestUnits();

    ecs::EntityManager m_EntityManager;
};
} // namespace redclone::world
