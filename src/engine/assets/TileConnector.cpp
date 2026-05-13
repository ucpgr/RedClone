#include "engine/assets/TileConnector.h"

namespace redclone::engine::assets
{
namespace
{
const TileEdgeConnector& edge(const TileDefinition& tile, const Direction direction)
{
    switch (direction)
    {
        case Direction::North: return tile.north;
        case Direction::East: return tile.east;
        case Direction::South: return tile.south;
        case Direction::West: return tile.west;
        default: return tile.north;
    }
}

Direction opposite(const Direction direction)
{
    switch (direction)
    {
        case Direction::North: return Direction::South;
        case Direction::East: return Direction::West;
        case Direction::South: return Direction::North;
        case Direction::West: return Direction::East;
        default: return Direction::South;
    }
}
} // namespace

bool canConnect(const TileDefinition& a, const Direction edgeFromA, const TileDefinition& b)
{
    const auto& edgeA = edge(a, edgeFromA);
    const auto& edgeB = edge(b, opposite(edgeFromA));
    return edgeA.height == edgeB.height && edgeA.terrain == edgeB.terrain;
}
} // namespace redclone::engine::assets
