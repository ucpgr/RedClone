#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace redclone::engine::assets
{
enum class Direction
{
    North,
    East,
    South,
    West,
    NorthEast,
    SouthEast,
    SouthWest,
    NorthWest
};

enum class TileKind
{
    Flat,
    Ramp,
    RampCorner,
    Edge,
    Cliff
};

struct TileEdgeConnector
{
    int height = 0;
    std::string terrain;
    bool walkable = true;
};

struct TileDefinition
{
    std::string name;
    std::string terrain;
    TileKind kind = TileKind::Flat;

    int height = 0;
    int lowHeight = 0;
    int highHeight = 0;

    Direction direction = Direction::North;
    Direction corner = Direction::NorthEast;

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    TileEdgeConnector north;
    TileEdgeConnector east;
    TileEdgeConnector south;
    TileEdgeConnector west;
};

struct TileSheetDefinition
{
    std::string sourcePath;
    std::string sheet;
    int tileWidth = 0;
    int tileHeight = 0;
    std::vector<TileDefinition> tiles;
};

class TileAssetRegistry
{
  public:
    void registerSheet(TileSheetDefinition sheet);
    const TileDefinition* findTile(std::string_view name) const;

  private:
    std::vector<TileSheetDefinition> m_Sheets;
    std::unordered_map<std::string, const TileDefinition*> m_ByName;
};
} // namespace redclone::engine::assets
