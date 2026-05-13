#include "engine/math/Isometric.h"

#include <cmath>

namespace redclone::engine::math::isometric
{
Vec2f worldToIso(const Vec2f& world)
{
    const float halfWidth = c_TileWidth * 0.5F;
    const float halfHeight = c_TileHeight * 0.5F;
    return {(world[0] - world[1]) * halfWidth, (world[0] + world[1]) * halfHeight};
}

Vec2f isoToWorld(const Vec2f& iso)
{
    const float halfWidth = c_TileWidth * 0.5F;
    const float halfHeight = c_TileHeight * 0.5F;
    const float normalizedX = iso[0] / halfWidth;
    const float normalizedY = iso[1] / halfHeight;
    return {(normalizedX + normalizedY) * 0.5F, (normalizedY - normalizedX) * 0.5F};
}

Vec2f tileToIso(const Vec2i& tileCoord)
{
    return worldToIso({static_cast<float>(tileCoord[0]), static_cast<float>(tileCoord[1])});
}

Vec2i isoToTile(const Vec2f& iso)
{
    const auto world = isoToWorld(iso);
    return {static_cast<int>(std::floor(world[0])), static_cast<int>(std::floor(world[1]))};
}
} // namespace redclone::engine::math::isometric
