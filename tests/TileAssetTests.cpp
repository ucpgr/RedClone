#include "engine/assets/TileAsset.h"
#include "engine/assets/TileAssetLoader.h"
#include "engine/assets/TileConnector.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace
{
void writeFile(const std::string& path, const std::string& body)
{
    std::ofstream out(path);
    out << body;
}

redclone::engine::assets::TileDefinition flat(const std::string& terrain, int h)
{
    using namespace redclone::engine::assets;
    TileDefinition t;
    t.terrain = terrain;
    t.north = {h, terrain, true};
    t.east = t.south = t.west = t.north;
    return t;
}
}

int runTileAssetTests()
{
    using namespace redclone::engine::assets;

    const auto tempDir = std::filesystem::path("build/tmp");
    std::filesystem::create_directories(tempDir);

    const std::string validPath = (tempDir / "tile_valid.lua").string();
    writeFile(validPath,
              "return { sheet='temperate.jpg', tile_width=64, tile_height=32, tiles={"
              "{name='grass_flat',terrain='grass',kind='flat',height=0,x=0,y=0,w=64,h=32},"
              "{name='grass_ramp_north',terrain='grass',kind='ramp',direction='north',low_height=1,high_height=2,x=64,y=0,w=64,h=32}"
              "} }");

    TileSheetDefinition sheet;
    std::string error;
    if (!TileAssetLoader::loadMetadataFile(validPath, sheet, error) || sheet.tiles.size() != 2)
    {
        std::cerr << "valid metadata parse failed: " << error << "\n";
        return 1;
    }

    const std::string invalidPath = (tempDir / "tile_invalid.lua").string();
    writeFile(invalidPath, "return { sheet='bad.jpg', tile_width=64, tile_height=32, tiles={{name='x',terrain='grass',kind='ramp',direction='up',low_height=0,high_height=1,x=0,y=0,w=64,h=32}} }");
    TileSheetDefinition invalidSheet;
    if (TileAssetLoader::loadMetadataFile(invalidPath, invalidSheet, error))
    {
        std::cerr << "invalid direction should fail\n";
        return 1;
    }


    const std::string missingXYPath = (tempDir / "tile_missing_xy.lua").string();
    writeFile(missingXYPath,
              "return { sheet='bad.jpg', tile_width=64, tile_height=32, tiles={{name='x',terrain='grass',kind='flat',height=0,w=64,h=32}} }");
    TileSheetDefinition missingXySheet;
    if (TileAssetLoader::loadMetadataFile(missingXYPath, missingXySheet, error))
    {
        std::cerr << "missing x/y should fail\n";
        return 1;
    }

    TileAssetRegistry reg;
    reg.registerSheet(std::move(sheet));
    if (reg.findTile("grass_flat") == nullptr)
    {
        std::cerr << "tile lookup failed\n";
        return 1;
    }

    auto flat0 = flat("grass", 0);
    auto flat1 = flat("grass", 1);
    auto flat2 = flat("grass", 2);
    if (!canConnect(flat0, Direction::East, flat0))
    {
        std::cerr << "flat0-flat0 should connect\n";
        return 1;
    }
    if (canConnect(flat1, Direction::East, flat2))
    {
        std::cerr << "flat1-flat2 should not connect\n";
        return 1;
    }

    TileDefinition rampNorth;
    rampNorth.terrain = "grass";
    rampNorth.north = {2, "grass", true};
    rampNorth.south = {1, "grass", true};
    rampNorth.east = {1, "grass", true};
    rampNorth.west = {1, "grass", true};

    if (!canConnect(flat1, Direction::North, rampNorth))
    {
        std::cerr << "ramp low side should connect to low flat\n";
        return 1;
    }
    if (!canConnect(flat2, Direction::South, rampNorth))
    {
        std::cerr << "ramp high side should connect to high flat\n";
        return 1;
    }

    if (!canConnect(flat0, Direction::East, flat0) || !canConnect(flat0, Direction::West, flat0) ||
        !canConnect(flat0, Direction::North, flat0) || !canConnect(flat0, Direction::South, flat0))
    {
        std::cerr << "opposite edge compatibility failed\n";
        return 1;
    }

    return 0;
}
