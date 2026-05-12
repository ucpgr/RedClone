#include "world/TileMapRenderer.h"

namespace redclone::world
{
void TileMapRenderer::render(engine::rendering::IRenderer& renderer, const TileMap& tileMap,
                             const gameplay::SelectionController& selectionController) const
{
    const engine::math::Vec2f tileSize(static_cast<float>(TileMap::c_TileSize),
                                       static_cast<float>(TileMap::c_TileSize));

    for (int y = 0; y < TileMap::c_Height; ++y)
    {
        for (int x = 0; x < TileMap::c_Width; ++x)
        {
            const auto type = tileMap.getTileTypeAt({x, y});
            if (!type.has_value())
            {
                continue;
            }

            engine::rendering::Color tileColor{70, 135, 70, 255};
            if (*type == TileType::Dirt)
            {
                tileColor = {130, 100, 60, 255};
            }
            if (*type == TileType::Blocked)
            {
                tileColor = {80, 80, 80, 255};
            }

            renderer.drawOutlinedRect(
                {static_cast<float>(x * TileMap::c_TileSize), static_cast<float>(y * TileMap::c_TileSize)},
                tileSize, tileColor, {30, 30, 30, 255}, -1.0F);
        }
    }

    if (const auto selected = selectionController.getSelectedTile())
    {
        renderer.drawOutlinedRect({static_cast<float>((*selected)[0] * TileMap::c_TileSize),
                                   static_cast<float>((*selected)[1] * TileMap::c_TileSize)},
                                  tileSize, {0, 0, 0, 0}, {255, 255, 0, 255}, -3.0F);
    }
}
} // namespace redclone::world
