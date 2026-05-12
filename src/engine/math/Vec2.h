#pragma once

namespace redclone::engine::math
{
struct Vec2f
{
    float x{};
    float y{};

    constexpr Vec2f() = default;
    constexpr Vec2f(const float xValue, const float yValue) : x(xValue), y(yValue) {}

    constexpr Vec2f& operator+=(const Vec2f& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vec2f& operator-=(const Vec2f& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vec2f& operator*=(const float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
};

struct Vec2i
{
    int x{};
    int y{};

    constexpr Vec2i() = default;
    constexpr Vec2i(const int xValue, const int yValue) : x(xValue), y(yValue) {}

    constexpr bool operator==(const Vec2i& other) const = default;
};

[[nodiscard]] constexpr Vec2f operator+(Vec2f lhs, const Vec2f& rhs)
{
    lhs += rhs;
    return lhs;
}

[[nodiscard]] constexpr Vec2f operator-(Vec2f lhs, const Vec2f& rhs)
{
    lhs -= rhs;
    return lhs;
}

[[nodiscard]] constexpr Vec2f operator*(Vec2f lhs, const float scalar)
{
    lhs *= scalar;
    return lhs;
}
} // namespace redclone::engine::math
