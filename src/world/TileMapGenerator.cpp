#include "world/TileMapGenerator.h"

#include <cassert>
#include <cstddef>

namespace redclone::world
{
void TileMapGenerator::generateTestMap(std::vector<Tile>& tiles, const int width, const int height)
{
    assert(width > 0);
    assert(height > 0);
    const auto requiredTileCount = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    assert(tiles.size() >= requiredTileCount);

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
        }
    }
}
} // namespace redclone::world
