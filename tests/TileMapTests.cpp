#include "world/TileMap.h"
#include "world/TileMapGenerator.h"

#include <iostream>
#include <vector>

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
        if (tileMap.getTileHeightAt(coord).has_value())
        {
            std::cerr << "TileMap safe height lookup failed\n";
            return 1;
        }
    }

    for (int y = 0; y < redclone::world::TileMap::c_Height; ++y)
    {
        for (int x = 0; x < redclone::world::TileMap::c_Width; ++x)
        {
            const auto height = tileMap.getTileHeightAt({x, y});
            if (!height || *height < redclone::world::TileMapGenerator::c_MinHeight ||
                *height > redclone::world::TileMapGenerator::c_MaxHeight)
            {
                std::cerr << "Tile height should always be in range [0,2]\n";
                return 1;
            }
        }
    }

    const auto tileSpan = tileMap.getTiles();
    std::vector<redclone::world::Tile> tileCopy(tileSpan.begin(), tileSpan.end());
    if (!redclone::world::TileMapGenerator::hasValidHeightTransitions(tileCopy, redclone::world::TileMap::c_Width,
                                                                       redclone::world::TileMap::c_Height))
    {
        std::cerr << "Tile height transitions should be valid\n";
        return 1;
    }

    std::vector<redclone::world::Tile> generatedA(redclone::world::TileMap::c_Width * redclone::world::TileMap::c_Height);
    std::vector<redclone::world::Tile> generatedB(redclone::world::TileMap::c_Width * redclone::world::TileMap::c_Height);
    redclone::world::TileMapGenerator::generateTestMap(generatedA, redclone::world::TileMap::c_Width,
                                                        redclone::world::TileMap::c_Height);
    redclone::world::TileMapGenerator::generateTestMap(generatedB, redclone::world::TileMap::c_Width,
                                                        redclone::world::TileMap::c_Height);

    if (generatedA.size() != generatedB.size())
    {
        std::cerr << "Deterministic generation size mismatch\n";
        return 1;
    }
    for (std::size_t i = 0; i < generatedA.size(); ++i)
    {
        if (generatedA[i].height != generatedB[i].height)
        {
            std::cerr << "Deterministic generation produced different heights\n";
            return 1;
        }
    }

    return 0;
}
