#pragma once

#include <cmath>

inline bool approx(const float a, const float b, const float epsilon = 0.0001F)
{
    return std::fabs(a - b) <= epsilon;
}
