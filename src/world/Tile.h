#pragma once

namespace redclone::world
{
enum class TileType
{
    Grass,
    Dirt,
    Blocked
};

struct Tile
{
    TileType type{TileType::Grass};
};
} // namespace redclone::world
