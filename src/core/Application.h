#pragma once

#include "engine/input/IInputObserver.h"
#include "engine/input/InputSystem.h"
#include "engine/input/SfmlInputAdapter.h"
#include "engine/rendering/SfmlRenderer.h"
#include "engine/window/Window.h"

namespace redclone::core
{
class Application final : public engine::input::IInputObserver
{
public:
    Application();
    ~Application() override;

    int run();
    void onInputEvent(const engine::input::InputEvent& event) override;

private:
    engine::window::Window window_;
    engine::rendering::SfmlRenderer renderer_;
    engine::input::InputSystem inputSystem_;
    engine::input::SfmlInputAdapter inputAdapter_;
};
} // namespace redclone::core
