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
    static constexpr const char* c_DefaultTileName = "grass_raised_067";
    const engine::assets::TileAssetRegistry* m_TileAssets = nullptr;
};
} // namespace redclone::world
