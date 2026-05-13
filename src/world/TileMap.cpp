#include "world/TileMap.h"

#include "world/TileMapGenerator.h"

#include "engine/math/Isometric.h"

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
    const auto tileCoord = engine::math::isometric::isoToTile(engine::math::isometric::worldToIso(worldPosition));

    if (tileCoord[0] < 0 || tileCoord[1] < 0 || tileCoord[0] >= c_Width || tileCoord[1] >= c_Height)
    {
        return std::nullopt;
    }

    return tileCoord;
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
