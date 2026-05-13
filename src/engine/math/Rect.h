#pragma once

#include "engine/math/MathTypes.h"

namespace redclone::engine::math
{
struct Rectf
{
    Vec2f position{};
    Vec2f size{};
};

struct IntRect
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};
} // namespace redclone::engine::math
