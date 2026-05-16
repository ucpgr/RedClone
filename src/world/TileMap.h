#pragma once

#include "engine/math/MathTypes.h"
#include "world/TerrainMesh.h"
#include "world/Tile.h"

#include <optional>
#include <span>
#include <vector>

namespace redclone::world
{
struct TerrainHit
{
    engine::math::Vec2i tileCoord{0, 0};
    engine::math::Vec2f worldPosition{0.0F, 0.0F};
    float height{0.0F};
    float sortKey{0.0F};
};

class TileMap
{
  public:
    static constexpr int c_Width = 64;
    static constexpr int c_Height = 64;

    TileMap();

    [[nodiscard]] std::span<const Tile> getTiles() const;
    [[nodiscard]] const TerrainMesh& getTerrainMesh() const;
    [[nodiscard]] std::optional<engine::math::Vec2i>
    worldToTile(const engine::math::Vec2f& worldPosition) const;
    [[nodiscard]] std::optional<engine::math::Vec2i>
    isoToTerrainTile(const engine::math::Vec2f& isoPosition) const;
    [[nodiscard]] std::optional<TerrainHit> isoToTerrainHit(const engine::math::Vec2f& isoPosition) const;
    [[nodiscard]] float sampleHeightAt(const engine::math::Vec2f& worldPosition) const;
    [[nodiscard]] std::optional<TileType> getTileTypeAt(const engine::math::Vec2i& coord) const;
    [[nodiscard]] std::optional<int> getTileHeightAt(const engine::math::Vec2i& coord) const;
    bool setTileHeightAt(const engine::math::Vec2i& coord, int height);

  private:
    std::vector<Tile> m_Tiles;
    TerrainMesh m_TerrainMesh;
};
} // namespace redclone::world
