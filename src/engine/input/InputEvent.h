#pragma once

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
    int key{};
    int mouseButton{};
    int mouseX{};
    int mouseY{};
    float mouseWheelDelta{};
};
} // namespace redclone::engine::input
