#pragma once
#include "engine/assets/TileAsset.h"
#include "engine/rendering/IRenderer.h"
#include "gameplay/SelectionController.h"
#include "world/TileMap.h"

namespace redclone::world
{
class TileMapRenderer
{
  public:
    void setTileAssets(const engine::assets::TileAssetRegistry* tileAssets);
    void render(engine::rendering::IRenderer& renderer, const TileMap& tileMap,
                const gameplay::SelectionController& selectionController) const;

  private:
    [[nodiscard]] static engine::math::Vec2f projectTerrainVertex(const engine::math::Vec3f& position);
    [[nodiscard]] static engine::rendering::Color terrainColor(TileType type, float shade);

    mutable bool m_LoggedMeshPath = false;
};
} // namespace redclone::world
