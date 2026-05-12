#pragma once

#include "world/Tile.h"

#include <vector>

namespace redclone::world
{
class TileMapGenerator
{
  public:
    static void generateTestMap(std::vector<Tile>& tiles, int width, int height);
};
} // namespace redclone::world
