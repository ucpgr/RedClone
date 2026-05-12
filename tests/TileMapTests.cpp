#include "world/TileMap.h"

#include <iostream>

int runTileMapTests()
{
    redclone::world::TileMap tileMap;
    const auto tile = tileMap.worldToTile({64.0F, 96.0F});
    if (!tile || (*tile)[0] != 2 || (*tile)[1] != 3)
    {
        std::cerr << "TileMap::worldToTile failed\n";
        return 1;
    }

    if (tileMap.getTileTypeAt({-1, 0}).has_value())
    {
        std::cerr << "TileMap safe lookup failed\n";
        return 1;
    }

    return 0;
}
