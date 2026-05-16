#include "world/TileMap.h"

#include "world/TileMapGenerator.h"

#include "engine/math/Isometric.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <optional>

namespace redclone::world
{
namespace
{
constexpr int c_PickSearchRadius = 4;

[[nodiscard]] float cross2D(const engine::math::Vec2f& a, const engine::math::Vec2f& b,
                            const engine::math::Vec2f& c)
{
    const auto ab = b - a;
    const auto ac = c - a;
    return (ab[0] * ac[1]) - (ab[1] * ac[0]);
}

[[nodiscard]] std::optional<std::array<float, 3>> triangleBarycentric(const engine::math::Vec2f& point,
                                                                      const engine::math::Vec2f& a,
                                                                      const engine::math::Vec2f& b,
                                                                      const engine::math::Vec2f& c)
{
    constexpr float c_Epsilon = 0.001F;
    const float area = cross2D(a, b, c);
    if (std::abs(area) <= c_Epsilon)
    {
        return std::nullopt;
    }

    const float w0 = cross2D(point, b, c) / area;
    const float w1 = cross2D(point, c, a) / area;
    const float w2 = cross2D(point, a, b) / area;
    if (w0 < -c_Epsilon || w1 < -c_Epsilon || w2 < -c_Epsilon)
    {
        return std::nullopt;
    }

    return std::array{w0, w1, w2};
}

[[nodiscard]] engine::math::Vec3f interpolate(const std::array<float, 3>& weights,
                                              const engine::math::Vec3f& a,
                                              const engine::math::Vec3f& b,
                                              const engine::math::Vec3f& c)
{
    return (a * weights[0]) + (b * weights[1]) + (c * weights[2]);
}

[[nodiscard]] std::size_t tileIndex(const int x, const int y)
{
    return static_cast<std::size_t>((y * TileMap::c_Width) + x);
}
} // namespace

TileMap::TileMap() : m_Tiles(c_Width * c_Height)
{
    TileMapGenerator::generateTestMap(m_Tiles, c_Width, c_Height);
    m_TerrainMesh.rebuild(m_Tiles, c_Width, c_Height);
}

std::span<const Tile> TileMap::getTiles() const
{
    return m_Tiles;
}

const TerrainMesh& TileMap::getTerrainMesh() const
{
    return m_TerrainMesh;
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

std::optional<engine::math::Vec2i> TileMap::isoToTerrainTile(const engine::math::Vec2f& isoPosition) const
{
    const auto hit = isoToTerrainHit(isoPosition);
    if (!hit.has_value())
    {
        return std::nullopt;
    }
    return hit->tileCoord;
}

std::optional<TerrainHit> TileMap::isoToTerrainHit(const engine::math::Vec2f& isoPosition) const
{
    const auto vertices = m_TerrainMesh.getVertices();

    float bestSortKey = -1.0F;
    std::optional<TerrainHit> bestHit;
    const auto faces = m_TerrainMesh.getFaces();
    const auto flatTile = engine::math::isometric::isoToTile(isoPosition);
    const int minX = std::max(0, flatTile[0] - c_PickSearchRadius);
    const int maxX = std::min(c_Width - 1, flatTile[0] + c_PickSearchRadius);
    const int minY = std::max(0, flatTile[1] - c_PickSearchRadius);
    const int maxY = std::min(c_Height - 1, flatTile[1] + c_PickSearchRadius);

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            const auto& face = faces[tileIndex(x, y)];
            const std::array positions = {
                vertices[face.vertexIndices[0]].position,
                vertices[face.vertexIndices[1]].position,
                vertices[face.vertexIndices[2]].position,
                vertices[face.vertexIndices[3]].position,
            };
            const std::array projected = {
                engine::math::isometric::worldToIso(positions[0]),
                engine::math::isometric::worldToIso(positions[1]),
                engine::math::isometric::worldToIso(positions[2]),
                engine::math::isometric::worldToIso(positions[3]),
            };

            std::optional<engine::math::Vec3f> terrainPosition;
            if (const auto weights = triangleBarycentric(isoPosition, projected[0], projected[1], projected[2]))
            {
                terrainPosition = interpolate(*weights, positions[0], positions[1], positions[2]);
            }
            else if (const auto weights = triangleBarycentric(isoPosition, projected[0], projected[2], projected[3]))
            {
                terrainPosition = interpolate(*weights, positions[0], positions[2], positions[3]);
            }

            if (!terrainPosition.has_value() || face.sortKey < bestSortKey)
            {
                continue;
            }

            bestSortKey = face.sortKey;
            bestHit = TerrainHit{{x, y}, {(*terrainPosition)[0], (*terrainPosition)[1]}, (*terrainPosition)[2],
                                 face.sortKey};
        }
    }

    return bestHit;
}

float TileMap::sampleHeightAt(const engine::math::Vec2f& worldPosition) const
{
    const auto isoPosition = engine::math::isometric::worldToIso(worldPosition);
    if (const auto hit = isoToTerrainHit(isoPosition))
    {
        return hit->height;
    }

    if (const auto tileCoord = worldToTile(worldPosition); tileCoord.has_value())
    {
        return static_cast<float>(getTileHeightAt(*tileCoord).value_or(0));
    }
    return 0.0F;
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

std::optional<int> TileMap::getTileHeightAt(const engine::math::Vec2i& coord) const
{
    if (coord[0] < 0 || coord[1] < 0 || coord[0] >= c_Width || coord[1] >= c_Height)
    {
        return std::nullopt;
    }

    const auto index = static_cast<std::size_t>((coord[1] * c_Width) + coord[0]);
    return m_Tiles[index].height;
}

bool TileMap::setTileHeightAt(const engine::math::Vec2i& coord, const int height)
{
    if (coord[0] < 0 || coord[1] < 0 || coord[0] >= c_Width || coord[1] >= c_Height)
    {
        return false;
    }

    const auto index = static_cast<std::size_t>((coord[1] * c_Width) + coord[0]);
    m_Tiles[index].height = height;
    m_TerrainMesh.updateTile(m_Tiles, c_Width, c_Height, coord);
    return true;
}
} // namespace redclone::world
