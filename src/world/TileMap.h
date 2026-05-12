#pragma once

#include "engine/math/Vec2.h"
#include "world/Tile.h"

#include <optional>
#include <span>
#include <vector>

namespace redclone::world
{
class TileMap
{
public:
    static constexpr int c_Width = 64;
    static constexpr int c_Height = 64;
    static constexpr int c_TileSize = 32;

    TileMap();

    [[nodiscard]] std::span<const Tile> getTiles() const;
    [[nodiscard]] std::optional<engine::math::Vec2i> worldToTile(const engine::math::Vec2f& worldPosition) const;
    [[nodiscard]] TileType getTileTypeAt(const engine::math::Vec2i& coord) const;

private:
    std::vector<Tile> m_Tiles;
};
} // namespace redclone::world
