#include "world/TileMapRenderer.h"
#include "core/Logger.h"

#include "engine/math/Isometric.h"

#include <array>

namespace redclone::world
{
void TileMapRenderer::setTileAssets(const engine::assets::TileAssetRegistry* tileAssets)
{
    m_TileAssets = tileAssets;
}

void TileMapRenderer::render(engine::rendering::IRenderer& renderer, const TileMap& tileMap,
                             const gameplay::SelectionController& selectionController) const
{
    const auto defaultLookup = m_TileAssets ? m_TileAssets->findTileWithTexture(c_DefaultTileName) : engine::assets::TileLookup{};
    if (defaultLookup.tile == nullptr && !m_LoggedMissingDefault)
    {
        REDCLONE_LOG_WARNING("TileMapRenderer missing default tile grass_raised_067.");
        m_LoggedMissingDefault = true;
    }
    if (defaultLookup.tile != nullptr && defaultLookup.texture != nullptr && !m_LoggedTexturedPath)
    {
        REDCLONE_LOG_INFO("TileMapRenderer using textured tile rendering for grass_raised_067.");
        m_LoggedTexturedPath = true;
    }
    if ((defaultLookup.tile == nullptr || defaultLookup.texture == nullptr) && !m_LoggedPrimitiveFallback)
    {
        REDCLONE_LOG_WARNING("TileMapRenderer falling back to primitive diamond rendering.");
        m_LoggedPrimitiveFallback = true;
    }

    for (int y = 0; y < TileMap::c_Height; ++y)
    {
        for (int x = 0; x < TileMap::c_Width; ++x)
        {
            const auto type = tileMap.getTileTypeAt({x, y});
            if (!type.has_value())
            {
                continue;
            }

            const auto center = engine::math::isometric::tileToIso({x, y});
            if (defaultLookup.tile != nullptr && defaultLookup.texture != nullptr)
            {
                const auto& t = *defaultLookup.tile;
                const engine::math::IntRect src{t.x, t.y, t.w, t.h};
                renderer.drawTexturedSprite(*defaultLookup.texture, src, {center[0] - (t.w * 0.5F), center[1] - (t.h * 0.5F)});
                continue;
            }

            engine::rendering::Color tileColor{70, 135, 70, 255};
            if (*type == TileType::Dirt) tileColor = {130, 100, 60, 255};
            if (*type == TileType::Blocked) tileColor = {80, 80, 80, 255};

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
