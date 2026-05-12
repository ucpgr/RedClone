#pragma once
#include "engine/rendering/IRenderer.h"
#include "gameplay/SelectionController.h"
#include "world/TileMap.h"

namespace redclone::world
{
class TileMapRenderer
{
  public:
    void render(engine::rendering::IRenderer& renderer, const TileMap& tileMap,
                const gameplay::SelectionController& selectionController) const;
};
} // namespace redclone::world
