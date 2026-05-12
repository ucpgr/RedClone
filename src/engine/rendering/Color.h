#pragma once

#include <cstdint>

namespace redclone::engine::rendering
{
struct Color
{
    std::uint8_t r{};
    std::uint8_t g{};
    std::uint8_t b{};
    std::uint8_t a{255};
};
} // namespace redclone::engine::rendering
