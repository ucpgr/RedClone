#include "world/TerrainMesh.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>

namespace redclone::world
{
namespace
{
constexpr float c_AmbientLight = 0.80F;
constexpr float c_NorthWestPointLightStrength = 0.28F;
constexpr float c_NorthWestPointLightHeight = 96.0F;
constexpr float c_NorthWestPointLightOffset = 64.0F;
constexpr float c_MinTerrainShade = 0.72F;
constexpr float c_MaxTerrainShade = 1.08F;

[[nodiscard]] std::size_t vertexIndex(const int x, const int y, const int width)
{
    return static_cast<std::size_t>((y * (width + 1)) + x);
}

[[nodiscard]] std::size_t tileIndex(const int x, const int y, const int width)
{
    return static_cast<std::size_t>((y * width) + x);
}

[[nodiscard]] engine::math::Vec3f cross(const engine::math::Vec3f& lhs, const engine::math::Vec3f& rhs)
{
    return {(lhs[1] * rhs[2]) - (lhs[2] * rhs[1]), (lhs[2] * rhs[0]) - (lhs[0] * rhs[2]),
            (lhs[0] * rhs[1]) - (lhs[1] * rhs[0])};
}

[[nodiscard]] float dot(const engine::math::Vec3f& lhs, const engine::math::Vec3f& rhs)
{
    return (lhs[0] * rhs[0]) + (lhs[1] * rhs[1]) + (lhs[2] * rhs[2]);
}

[[nodiscard]] engine::math::Vec3f normalized(const engine::math::Vec3f& value)
{
    const float length = std::sqrt(dot(value, value));
    if (length <= 0.0001F)
    {
        return {0.0F, 0.0F, 1.0F};
    }
    return value / length;
}

[[nodiscard]] float northWestPointLightShade(const engine::math::Vec3f& northWest,
                                             const engine::math::Vec3f& northEast,
                                             const engine::math::Vec3f& southEast,
                                             const engine::math::Vec3f& southWest)
{
    const auto normal = normalized(cross(northEast - northWest, southWest - northWest));
    const auto center = (northWest + northEast + southEast + southWest) * 0.25F;
    const engine::math::Vec3f lightPosition{-c_NorthWestPointLightOffset, -c_NorthWestPointLightOffset,
                                            c_NorthWestPointLightHeight};
    const auto lightDirection = normalized(lightPosition - center);
    const float diffuse = std::max(0.0F, dot(normal, lightDirection));
    return std::clamp(c_AmbientLight + (diffuse * c_NorthWestPointLightStrength), c_MinTerrainShade,
                      c_MaxTerrainShade);
}
} // namespace

void TerrainMesh::rebuild(std::span<const Tile> tiles, const int width, const int height)
{
    assert(width > 0);
    assert(height > 0);
    assert(tiles.size() >= static_cast<std::size_t>(width * height));

    m_Vertices.clear();
    m_Faces.clear();
    m_Vertices.reserve(static_cast<std::size_t>((width + 1) * (height + 1)));
    m_Faces.reserve(static_cast<std::size_t>(width * height));

    for (int y = 0; y <= height; ++y)
    {
        for (int x = 0; x <= width; ++x)
        {
            m_Vertices.push_back({{static_cast<float>(x), static_cast<float>(y),
                                   sampleCornerHeight(tiles, width, height, x, y)}});
        }
    }

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const auto& tile = tiles[tileIndex(x, y, width)];
            (void)tile;
            m_Faces.push_back({});
            updateFace(tiles, width, x, y);
        }
    }
}

void TerrainMesh::updateTile(std::span<const Tile> tiles, const int width, const int height,
                             const engine::math::Vec2i& tileCoord)
{
    assert(width > 0);
    assert(height > 0);
    assert(tiles.size() >= static_cast<std::size_t>(width * height));

    for (int y = tileCoord[1]; y <= tileCoord[1] + 1; ++y)
    {
        for (int x = tileCoord[0]; x <= tileCoord[0] + 1; ++x)
        {
            m_Vertices[vertexIndex(x, y, width)].position[2] = sampleCornerHeight(tiles, width, height, x, y);
        }
    }

    const int minX = std::max(0, tileCoord[0] - 1);
    const int maxX = std::min(width - 1, tileCoord[0] + 1);
    const int minY = std::max(0, tileCoord[1] - 1);
    const int maxY = std::min(height - 1, tileCoord[1] + 1);
    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            updateFace(tiles, width, x, y);
        }
    }
}

std::span<const TerrainVertex> TerrainMesh::getVertices() const
{
    return m_Vertices;
}

std::span<const TerrainFace> TerrainMesh::getFaces() const
{
    return m_Faces;
}

float TerrainMesh::sampleCornerHeight(std::span<const Tile> tiles, const int width, const int height,
                                      const int cornerX, const int cornerY)
{
    float heightTotal = 0.0F;
    float samples = 0.0F;
    for (int y = cornerY - 1; y <= cornerY; ++y)
    {
        for (int x = cornerX - 1; x <= cornerX; ++x)
        {
            if (x < 0 || y < 0 || x >= width || y >= height)
            {
                continue;
            }
            heightTotal += static_cast<float>(tiles[tileIndex(x, y, width)].height);
            samples += 1.0F;
        }
    }

    return samples > 0.0F ? heightTotal / samples : 0.0F;
}

void TerrainMesh::updateFace(std::span<const Tile> tiles, const int width, const int x, const int y)
{
    const auto& tile = tiles[tileIndex(x, y, width)];
    const auto northWest = vertexIndex(x, y, width);
    const auto northEast = vertexIndex(x + 1, y, width);
    const auto southEast = vertexIndex(x + 1, y + 1, width);
    const auto southWest = vertexIndex(x, y + 1, width);

    const float shade = northWestPointLightShade(m_Vertices[northWest].position, m_Vertices[northEast].position,
                                                 m_Vertices[southEast].position, m_Vertices[southWest].position);

    const float averageHeight = (m_Vertices[northWest].position[2] + m_Vertices[northEast].position[2] +
                                 m_Vertices[southEast].position[2] + m_Vertices[southWest].position[2]) *
                                0.25F;
    m_Faces[tileIndex(x, y, width)] = TerrainFace{{northWest, northEast, southEast, southWest},
                                                  {x, y},
                                                  tile.type,
                                                  shade,
                                                  static_cast<float>(x + y) + (averageHeight * 0.01F)};
}
} // namespace redclone::world
