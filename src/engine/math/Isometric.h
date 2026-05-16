#pragma once

#include "engine/math/MathTypes.h"

namespace redclone::engine::math::isometric
{
inline constexpr float c_TileWidth = 64.0F;
inline constexpr float c_TileHeight = 32.0F;
inline constexpr float c_TerrainHeightStep = 16.0F;

[[nodiscard]] Vec2f worldToIso(const Vec2f& world);
[[nodiscard]] Vec2f worldToIso(const Vec3f& world);
[[nodiscard]] Vec2f isoToWorld(const Vec2f& iso);
[[nodiscard]] Vec2f tileToIso(const Vec2i& tileCoord);
[[nodiscard]] Vec2i isoToTile(const Vec2f& iso);
} // namespace redclone::engine::math::isometric
