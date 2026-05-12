#include "world/TileMap.h"

#include "world/TileMapGenerator.h"

#include <cstddef>

namespace redclone::world
{
TileMap::TileMap() : m_Tiles(c_Width * c_Height)
{
    TileMapGenerator::generateTestMap(m_Tiles, c_Width, c_Height);
}

std::span<const Tile> TileMap::getTiles() const
{
    return m_Tiles;
}

std::optional<engine::math::Vec2i> TileMap::worldToTile(const engine::math::Vec2f& worldPosition) const
{
    const int tileX = static_cast<int>(worldPosition[0]) / c_TileSize;
    const int tileY = static_cast<int>(worldPosition[1]) / c_TileSize;

    if (tileX < 0 || tileY < 0 || tileX >= c_Width || tileY >= c_Height)
    {
        return std::nullopt;
    }

    return engine::math::Vec2i(tileX, tileY);
}

std::optional<TileType> TileMap::getTileTypeAt(const engine::math::Vec2i& coord) const
{
    if (coord[0] < 0 || coord[1] < 0 || coord[0] >= c_Width || coord[1] >= c_Height)
    {
        return std::nullopt;
    }

    const auto index = static_cast<std::size_t>((coord[1] * c_Width) + coord[0]);
    return m_Tiles[index].type;
}
} // namespace redclone::world
