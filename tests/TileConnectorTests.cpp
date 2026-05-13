#include "engine/assets/TileAsset.h"
#include "engine/assets/TileConnector.h"

#include <iostream>

namespace
{
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

int runTileConnectorTests()
{
    using namespace redclone::engine::assets;
    auto flat0 = flat("grass", 0);
    auto flat1 = flat("grass", 1);
    auto flat2 = flat("grass", 2);

    if (!canConnect(flat0, Direction::East, flat0) || !canConnect(flat0, Direction::West, flat0) ||
        !canConnect(flat0, Direction::North, flat0) || !canConnect(flat0, Direction::South, flat0))
    {
        std::cerr << "directional flat connections failed\n";
        return 1;
    }
    if (canConnect(flat1, Direction::East, flat2))
    {
        std::cerr << "flat mismatch should fail\n";
        return 1;
    }

    TileDefinition rampNorth;
    rampNorth.terrain = "grass";
    rampNorth.north = {2, "grass", true};
    rampNorth.south = {1, "grass", true};
    rampNorth.east = {1, "grass", true};
    rampNorth.west = {1, "grass", true};

    if (!canConnect(flat1, Direction::North, rampNorth) || !canConnect(flat2, Direction::South, rampNorth))
    {
        std::cerr << "ramp low/high connections failed\n";
        return 1;
    }

    return 0;
}
