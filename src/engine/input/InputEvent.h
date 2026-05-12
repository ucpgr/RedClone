#pragma once

#include "engine/input/Key.h"
#include "engine/input/MouseButton.h"
#include "engine/math/MathTypes.h"

namespace redclone::engine::input
{
enum class InputEventType
{
    Closed,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseWheelScrolled
};

struct InputEvent
{
    InputEventType type{};
    Key key{Key::Unknown};
    MouseButton mouseButton{MouseButton::Unknown};
    math::Vec2i mousePosition{};
    float mouseWheelDelta{};
};
} // namespace redclone::engine::input
