#include "engine/assets/TileAsset.h"
#include "engine/assets/TileAssetLoader.h"

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
}

int runTileAssetMetadataTests()
{
    using namespace redclone::engine::assets;
    const auto tempDir = std::filesystem::path("build/tmp");
    std::filesystem::create_directories(tempDir);

    std::string error;
    TileSheetDefinition sheet;

    const std::string validPath = (tempDir / "tile_valid.lua").string();
    writeFile(validPath,
              "return { sheet='temperate.jpg', tile_width=64, tile_height=32, tiles={{name='grass_raised_067',terrain='grass',kind='flat',height=0,x=0,y=0,w=64,h=32}} }");
    if (!TileAssetLoader::loadMetadataFile(validPath, sheet, error) || sheet.tiles.size() != 1)
    {
        std::cerr << "valid metadata parse failed: " << error << "\n";
        return 1;
    }

    const std::string duplicatePath = (tempDir / "tile_dup.lua").string();
    writeFile(duplicatePath,
              "return { sheet='temperate.jpg', tile_width=64, tile_height=32, tiles={{name='dup',terrain='grass',kind='flat',height=0,x=0,y=0,w=64,h=32},{name='dup',terrain='grass',kind='flat',height=0,x=64,y=0,w=64,h=32}} }");
    if (TileAssetLoader::loadMetadataFile(duplicatePath, sheet, error))
    {
        std::cerr << "duplicate tile names should fail\n";
        return 1;
    }

    const std::string badPath = (tempDir / "tile_bad.lua").string();
    writeFile(badPath, "return { sheet='', tile_width=64, tile_height=32, tiles={} }");
    if (TileAssetLoader::loadMetadataFile(badPath, sheet, error) || error.empty())
    {
        std::cerr << "bad metadata should fail with readable error\n";
        return 1;
    }

    const std::string unsafePath = (tempDir / "tile_unsafe.lua").string();
    writeFile(unsafePath,
              "return { sheet=os.date(), tile_width=64, tile_height=32, tiles={{name='grass',terrain='grass',kind='flat',height=0,x=0,y=0,w=64,h=32}} }");
    if (TileAssetLoader::loadMetadataFile(unsafePath, sheet, error) || error.empty())
    {
        std::cerr << "metadata loader should not expose Lua standard libraries\n";
        return 1;
    }

    return 0;
}
