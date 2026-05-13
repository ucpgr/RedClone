#include "world/TileMap.h"

#include <iostream>

int runTileMapTests()
{
    redclone::world::TileMap tileMap;
    const auto tile = tileMap.worldToTile({2.2F, 3.7F});
    if (!tile || (*tile)[0] != 2 || (*tile)[1] != 3)
    {
        std::cerr << "TileMap::worldToTile failed\n";
        return 1;
    }

    const redclone::engine::math::Vec2i invalidCoords[] = {
        {-1, 0},
        {redclone::world::TileMap::c_Width, 0},
        {0, redclone::world::TileMap::c_Height},
        {redclone::world::TileMap::c_Width, redclone::world::TileMap::c_Height},
        {9999, 9999},
    };

    for (const auto& coord : invalidCoords)
    {
        if (tileMap.getTileTypeAt(coord).has_value())
        {
            std::cerr << "TileMap safe lookup failed\n";
            return 1;
        }
    }

    return 0;
}
