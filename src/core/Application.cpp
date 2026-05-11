#include "core/Application.h"

#include <SFML/Window/Keyboard.hpp>

namespace redclone::core
{
Application::Application() : renderer_(window_), inputAdapter_(window_, inputSystem_)
{
    inputSystem_.addObserver(*this);
}

Application::~Application()
{
    inputSystem_.removeObserver(*this);
}

int Application::run()
{
    while (window_.isOpen())
    {
        inputAdapter_.pollEvents();
        renderer_.beginFrame();
        renderer_.endFrame();
    }

    return 0;
}

void Application::onInputEvent(const engine::input::InputEvent& event)
{
    if (event.type == engine::input::InputEventType::Closed)
    {
        window_.close();
        return;
    }

    if (event.type == engine::input::InputEventType::KeyPressed &&
        event.key == static_cast<int>(sf::Keyboard::Key::Escape))
    {
        window_.close();
    }
}
} // namespace redclone::core
