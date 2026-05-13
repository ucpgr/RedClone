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
    int height{0};
};
} // namespace redclone::world
