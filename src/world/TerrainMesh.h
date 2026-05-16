#pragma once

#include "engine/math/MathTypes.h"
#include "world/Tile.h"

#include <array>
#include <cstddef>
#include <span>
#include <vector>

namespace redclone::world
{
struct TerrainVertex
{
    engine::math::Vec3f position{0.0F, 0.0F, 0.0F};
};

struct TerrainFace
{
    std::array<std::size_t, 4> vertexIndices{};
    engine::math::Vec2i tileCoord{0, 0};
    TileType type{TileType::Grass};
    float shade{1.0F};
    float sortKey{0.0F};
};

class TerrainMesh
{
  public:
    void rebuild(std::span<const Tile> tiles, int width, int height);
    void updateTile(std::span<const Tile> tiles, int width, int height, const engine::math::Vec2i& tileCoord);

    [[nodiscard]] std::span<const TerrainVertex> getVertices() const;
    [[nodiscard]] std::span<const TerrainFace> getFaces() const;

  private:
    [[nodiscard]] static float sampleCornerHeight(std::span<const Tile> tiles, int width, int height, int cornerX,
                                                  int cornerY);
    void updateFace(std::span<const Tile> tiles, int width, int x, int y);

    std::vector<TerrainVertex> m_Vertices;
    std::vector<TerrainFace> m_Faces;
};
} // namespace redclone::world
