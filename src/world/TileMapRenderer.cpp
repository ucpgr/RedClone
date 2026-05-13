#include "world/TileMapRenderer.h"

#include "engine/math/Isometric.h"

#include <array>

namespace redclone::world
{
void TileMapRenderer::render(engine::rendering::IRenderer& renderer, const TileMap& tileMap,
                             const gameplay::SelectionController& selectionController) const
{
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

            const auto center = engine::math::isometric::tileToIso({x, y});
            const float halfWidth = engine::math::isometric::c_TileWidth * 0.5F;
            const float halfHeight = engine::math::isometric::c_TileHeight * 0.5F;
            const std::array points = {
                engine::math::Vec2f{center[0], center[1] - halfHeight},
                engine::math::Vec2f{center[0] + halfWidth, center[1]},
                engine::math::Vec2f{center[0], center[1] + halfHeight},
                engine::math::Vec2f{center[0] - halfWidth, center[1]},
            };
            renderer.drawConvexPolygon(points, tileColor, {30, 30, 30, 255}, -1.0F);
        }
    }

    if (const auto selected = selectionController.getSelectedTile())
    {
        const auto center = engine::math::isometric::tileToIso(*selected);
        const float halfWidth = engine::math::isometric::c_TileWidth * 0.5F;
        const float halfHeight = engine::math::isometric::c_TileHeight * 0.5F;
        const std::array points = {
            engine::math::Vec2f{center[0], center[1] - halfHeight},
            engine::math::Vec2f{center[0] + halfWidth, center[1]},
            engine::math::Vec2f{center[0], center[1] + halfHeight},
            engine::math::Vec2f{center[0] - halfWidth, center[1]},
        };
        renderer.drawConvexPolygon(points, {0, 0, 0, 0}, {255, 255, 0, 255}, -3.0F);
    }
}
} // namespace redclone::world
