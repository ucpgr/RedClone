#include "world/TileMapGenerator.h"
#include "core/Logger.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>

namespace
{
constexpr float c_NoiseScale = 0.12F;
constexpr int c_MaxRelaxationPasses = 8;

[[nodiscard]] std::uint32_t hash2D(const int x, const int y, const std::uint32_t seed)
{
    std::uint32_t h = seed;
    h ^= static_cast<std::uint32_t>(x) * 374761393U;
    h = (h << 13U) ^ h;
    h ^= static_cast<std::uint32_t>(y) * 668265263U;
    h = (h << 13U) ^ h;
    return h * 1274126177U;
}

[[nodiscard]] float random01(const int x, const int y, const std::uint32_t seed)
{
    return static_cast<float>(hash2D(x, y, seed) & 0x00FFFFFFU) / static_cast<float>(0x01000000U);
}

[[nodiscard]] float smoothStep(const float t)
{
    return t * t * (3.0F - (2.0F * t));
}

[[nodiscard]] float lerp(const float a, const float b, const float t)
{
    return a + ((b - a) * t);
}

[[nodiscard]] float valueNoise(const float x, const float y, const std::uint32_t seed)
{
    const int x0 = static_cast<int>(std::floor(x));
    const int y0 = static_cast<int>(std::floor(y));
    const int x1 = x0 + 1;
    const int y1 = y0 + 1;
    const float tx = smoothStep(x - static_cast<float>(x0));
    const float ty = smoothStep(y - static_cast<float>(y0));

    const float v00 = random01(x0, y0, seed);
    const float v10 = random01(x1, y0, seed);
    const float v01 = random01(x0, y1, seed);
    const float v11 = random01(x1, y1, seed);

    return lerp(lerp(v00, v10, tx), lerp(v01, v11, tx), ty);
}
} // namespace

namespace redclone::world
{
void TileMapGenerator::generateTestMap(std::vector<Tile>& tiles, const int width, const int height)
{
    assert(width > 0);
    assert(height > 0);
    const auto requiredTileCount = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    assert(tiles.size() >= requiredTileCount);

    REDCLONE_LOG_INFO("TileMapGenerator height generation started.");
    REDCLONE_LOG_INFO("TileMapGenerator height seed = " + std::to_string(c_FixedSeed) + ".");

    int minHeight = c_MaxHeight;
    int maxHeight = c_MinHeight;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            auto& tile = tiles[static_cast<std::size_t>((y * width) + x)];

            if ((x + y) % 13 == 0)
            {
                tile.type = TileType::Blocked;
            }
            else if ((x * 3 + y * 7) % 5 == 0)
            {
                tile.type = TileType::Dirt;
            }
            else
            {
                tile.type = TileType::Grass;
            }

            const float sample = valueNoise(static_cast<float>(x) * c_NoiseScale, static_cast<float>(y) * c_NoiseScale, c_FixedSeed);
            tile.height = c_MinHeight + static_cast<int>(sample * static_cast<float>((c_MaxHeight - c_MinHeight) + 1));
            if (tile.height > c_MaxHeight)
            {
                tile.height = c_MaxHeight;
            }
            if (tile.height < minHeight) minHeight = tile.height;
            if (tile.height > maxHeight) maxHeight = tile.height;
        }
    }

    for (int pass = 0; pass < c_MaxRelaxationPasses; ++pass)
    {
        bool changed = false;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                auto& tile = tiles[static_cast<std::size_t>((y * width) + x)];
                const int offsets[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
                for (const auto& offset : offsets)
                {
                    const int nx = x + offset[0];
                    const int ny = y + offset[1];
                    if (nx < 0 || ny < 0 || nx >= width || ny >= height)
                    {
                        continue;
                    }
                    const auto neighborIndex = static_cast<std::size_t>((ny * width) + nx);
                    auto& neighbor = tiles[neighborIndex];
                    const int diff = tile.height - neighbor.height;
                    if (std::abs(diff) > 1)
                    {
                        if (diff > 0)
                        {
                            tile.height = neighbor.height + 1;
                        }
                        else
                        {
                            neighbor.height = tile.height + 1;
                        }
                        changed = true;
                    }
                }
            }
        }
        if (!changed)
        {
            break;
        }
    }

    minHeight = c_MaxHeight;
    maxHeight = c_MinHeight;
    for (const auto& tile : tiles)
    {
        if (tile.height < minHeight) minHeight = tile.height;
        if (tile.height > maxHeight) maxHeight = tile.height;
    }
    REDCLONE_LOG_INFO("TileMapGenerator generated height range: min=" + std::to_string(minHeight) + ", max=" + std::to_string(maxHeight) + ".");
    REDCLONE_LOG_INFO(std::string("TileMapGenerator height transition validation: ") +
                      (hasValidHeightTransitions(tiles, width, height) ? "passed." : "failed."));
}

bool TileMapGenerator::hasValidHeightTransitions(const std::vector<Tile>& tiles, const int width, const int height)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const auto index = static_cast<std::size_t>((y * width) + x);
            const int currentHeight = tiles[index].height;
            const int offsets[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
            for (const auto& offset : offsets)
            {
                const int nx = x + offset[0];
                const int ny = y + offset[1];
                if (nx < 0 || ny < 0 || nx >= width || ny >= height)
                {
                    continue;
                }
                const auto neighborIndex = static_cast<std::size_t>((ny * width) + nx);
                if (std::abs(currentHeight - tiles[neighborIndex].height) > 1)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
} // namespace redclone::world
