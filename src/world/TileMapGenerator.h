#pragma once

#include "world/Tile.h"

#include <vector>

namespace redclone::world
{
class TileMapGenerator
{
  public:
    static constexpr int c_MinHeight = 0;
    static constexpr int c_MaxHeight = 2;
    static constexpr unsigned int c_FixedSeed = 1337U;

    static void generateTestMap(std::vector<Tile>& tiles, int width, int height);
    [[nodiscard]] static bool hasValidHeightTransitions(const std::vector<Tile>& tiles, int width, int height);
};
} // namespace redclone::world
