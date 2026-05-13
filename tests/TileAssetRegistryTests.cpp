#include "engine/assets/TileAsset.h"

#include <iostream>

int runTileAssetRegistryTests()
{
    using namespace redclone::engine::assets;

    TileSheetDefinition sheet;
    sheet.tiles.push_back(TileDefinition{.name = "grass_flat"});
    TileAssetRegistry reg;
    reg.registerSheet(sheet);

    if (reg.findTile("grass_flat") == nullptr || reg.findTileWithTexture("grass_flat").tile == nullptr)
    {
        std::cerr << "tile lookup failed\n";
        return 1;
    }

    TileSheetDefinition sheet2;
    sheet2.tiles.push_back(TileDefinition{.name = "other"});
    reg.registerSheet(std::move(sheet2));

    if (reg.findTile("grass_flat") == nullptr)
    {
        std::cerr << "registry stability failed\n";
        return 1;
    }

    return 0;
}
