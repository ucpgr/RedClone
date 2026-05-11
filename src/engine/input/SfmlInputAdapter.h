#pragma once

namespace redclone::engine::input
{
class InputSystem;
}

namespace redclone::engine::window
{
class Window;
}

namespace redclone::engine::input
{
class SfmlInputAdapter
{
public:
    SfmlInputAdapter(window::Window& window, InputSystem& inputSystem);

    void pollEvents();

private:
    window::Window& window_;
    InputSystem& inputSystem_;
};
} // namespace redclone::engine::input
