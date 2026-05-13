#include "engine/assets/TileAssetLoader.h"
#include "core/Logger.h"

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <ranges>
#include <sstream>
#include <unordered_set>
#include <vector>

extern "C"
{
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

namespace redclone::engine::assets
{
namespace
{
Direction parseDirection(const std::string& value)
{
    if (value == "north") return Direction::North;
    if (value == "east") return Direction::East;
    if (value == "south") return Direction::South;
    if (value == "west") return Direction::West;
    if (value == "northeast") return Direction::NorthEast;
    if (value == "southeast") return Direction::SouthEast;
    if (value == "southwest") return Direction::SouthWest;
    if (value == "northwest") return Direction::NorthWest;
    throw std::runtime_error("invalid direction: " + value);
}

TileKind parseKind(const std::string& value)
{
    if (value == "flat") return TileKind::Flat;
    if (value == "ramp") return TileKind::Ramp;
    if (value == "ramp_corner") return TileKind::RampCorner;
    if (value == "edge") return TileKind::Edge;
    if (value == "cliff") return TileKind::Cliff;
    throw std::runtime_error("invalid tile kind: " + value);
}

std::string reqString(lua_State* L, int idx, const char* key)
{
    lua_getfield(L, idx, key);
    if (!lua_isstring(L, -1)) throw std::runtime_error(std::string("missing/invalid field: ") + key);
    std::string v = lua_tostring(L, -1);
    lua_pop(L, 1);
    if (v.empty()) throw std::runtime_error(std::string("empty field: ") + key);
    return v;
}

int reqInt(lua_State* L, int idx, const char* key)
{
    lua_getfield(L, idx, key);
    if (!lua_isinteger(L, -1)) throw std::runtime_error(std::string("missing/invalid field: ") + key);
    const int v = static_cast<int>(lua_tointeger(L, -1));
    lua_pop(L, 1);
    return v;
}

void assignConnectors(TileDefinition& tile)
{
    if (tile.kind == TileKind::Flat)
    {
        tile.north = {tile.height, tile.terrain, true};
        tile.east = tile.south = tile.west = tile.north;
        return;
    }

    tile.north = {tile.lowHeight, tile.terrain, true};
    tile.east = tile.south = tile.west = tile.north;
    if (tile.kind == TileKind::Ramp)
    {
        if (tile.direction == Direction::North) tile.north.height = tile.highHeight;
        if (tile.direction == Direction::South) tile.south.height = tile.highHeight;
        if (tile.direction == Direction::East) tile.east.height = tile.highHeight;
        if (tile.direction == Direction::West) tile.west.height = tile.highHeight;
    }
}
} // namespace

bool TileAssetLoader::loadMetadataFile(const std::string& filePath, TileSheetDefinition& out, std::string& error)
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    try
    {
        if (luaL_dofile(L, filePath.c_str()) != LUA_OK) throw std::runtime_error(lua_tostring(L, -1));
        if (!lua_istable(L, -1)) throw std::runtime_error("metadata must return table");

        out = {};
        out.sourcePath = filePath;
        out.sheet = reqString(L, -1, "sheet");
        out.tileWidth = reqInt(L, -1, "tile_width");
        out.tileHeight = reqInt(L, -1, "tile_height");

        lua_getfield(L, -1, "tiles");
        if (!lua_istable(L, -1)) throw std::runtime_error("missing/invalid field: tiles");

        const int count = static_cast<int>(lua_rawlen(L, -1));
        if (count <= 0) throw std::runtime_error("tiles cannot be empty");

        std::unordered_set<std::string> names;
        for (int i = 1; i <= count; ++i)
        {
            lua_rawgeti(L, -1, i);
            if (!lua_istable(L, -1)) throw std::runtime_error("tile entry must be table");
            TileDefinition tile;
            tile.name = reqString(L, -1, "name");
            if (!names.insert(tile.name).second) throw std::runtime_error("duplicate tile name: " + tile.name);
            tile.terrain = reqString(L, -1, "terrain");
            tile.kind = parseKind(reqString(L, -1, "kind"));
            tile.x = reqInt(L, -1, "x");
            tile.y = reqInt(L, -1, "y");
            tile.w = reqInt(L, -1, "w");
            tile.h = reqInt(L, -1, "h");
            if (tile.x < 0 || tile.y < 0 || tile.w <= 0 || tile.h <= 0)
            {
                throw std::runtime_error("invalid source rectangle for tile: " + tile.name);
            }

            if (tile.kind == TileKind::Flat)
            {
                tile.height = reqInt(L, -1, "height");
                tile.lowHeight = tile.highHeight = tile.height;
            }
            else
            {
                tile.lowHeight = reqInt(L, -1, "low_height");
                tile.highHeight = reqInt(L, -1, "high_height");
                if (tile.kind == TileKind::Ramp) tile.direction = parseDirection(reqString(L, -1, "direction"));
            }
            assignConnectors(tile);
            out.tiles.push_back(std::move(tile));
            lua_pop(L, 1);
        }
        lua_close(L);
        return true;
    }
    catch (const std::exception& ex)
    {
        error = ex.what();
        lua_close(L);
        return false;
    }
}

bool TileAssetLoader::loadDirectory(const std::string& directoryPath, TileAssetRegistry& out, std::string& error)
{
    namespace fs = std::filesystem;
    REDCLONE_LOG_INFO(std::string("Tile asset directory: ") + directoryPath);
    if (!fs::exists(directoryPath))
    {
        REDCLONE_LOG_WARNING(std::string("Tile asset directory does not exist: ") + directoryPath);
        return true;
    }
    REDCLONE_LOG_INFO(std::string("Tile asset directory exists: ") + directoryPath);

    std::string combinedErrors;
    std::vector<fs::path> metadataFiles;
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".lua") continue;
        metadataFiles.push_back(entry.path());
    }
    std::ranges::sort(metadataFiles);
    REDCLONE_LOG_INFO(std::string("Tile metadata files found: ") + std::to_string(metadataFiles.size()));

    for (const auto& metadataPath : metadataFiles)
    {
        REDCLONE_LOG_DEBUG(std::string("Loading tile metadata: ") + metadataPath.string());

        TileSheetDefinition sheet;
        std::string localError;
        if (!loadMetadataFile(metadataPath.string(), sheet, localError))
        {
            combinedErrors += "metadata parse failed for '" + metadataPath.string() + "': " + localError + "\n";
            REDCLONE_LOG_ERROR(std::string("Metadata parse failed: ") + metadataPath.string() + " -> " + localError);
            continue;
        }
        REDCLONE_LOG_DEBUG(std::string("Metadata parse success: ") + metadataPath.string());

        const fs::path texturePath = metadataPath.parent_path() / sheet.sheet;
        REDCLONE_LOG_DEBUG(std::string("Resolved texture path: ") + texturePath.string());
        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(texturePath.string()))
        {
            combinedErrors +=
                "failed to load texture for metadata '" + metadataPath.string() + "': " + texturePath.string() + "\n";
            REDCLONE_LOG_ERROR(std::string("Texture load failed: ") + texturePath.string());
            continue;
        }
        REDCLONE_LOG_DEBUG(std::string("Texture load success: ") + texturePath.string());

        out.registerSheet(std::move(sheet), std::move(texture));
    }

    REDCLONE_LOG_INFO(std::string("Tile sheets loaded: ") + std::to_string(out.sheetCount()));
    REDCLONE_LOG_INFO(std::string("Tiles registered: ") + std::to_string(out.tileCount()));
    REDCLONE_LOG_INFO(std::string("Contains grass_raised_067: ") + (out.containsTile("grass_raised_067") ? "yes" : "no"));

    if (!combinedErrors.empty())
    {
        error = combinedErrors;
        return false;
    }
    return true;
}
} // namespace redclone::engine::assets
