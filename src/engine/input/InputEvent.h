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
    MouseMoved
};

struct InputEvent
{
    InputEventType type{};
    int key{};
    int mouseButton{};
    int mouseX{};
    int mouseY{};
};
} // namespace redclone::engine::input
