#include "engine/input/SfmlInputAdapter.h"

#include "engine/input/InputEvent.h"
#include "engine/input/InputSystem.h"
#include "engine/window/Window.h"

#include <SFML/Window/Event.hpp>

namespace redclone::engine::input
{
SfmlInputAdapter::SfmlInputAdapter(window::Window& window, InputSystem& inputSystem)
    : window_(window), inputSystem_(inputSystem)
{
}

void SfmlInputAdapter::pollEvents()
{
    while (const auto event = window_.nativeWindow().pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            inputSystem_.notify({.type = InputEventType::Closed});
            continue;
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            inputSystem_.notify({.type = InputEventType::KeyPressed, .key = static_cast<int>(keyPressed->code)});
            continue;
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            inputSystem_.notify({.type = InputEventType::KeyReleased, .key = static_cast<int>(keyReleased->code)});
            continue;
        }

        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            inputSystem_.notify(
                {.type = InputEventType::MouseButtonPressed,
                 .mouseButton = static_cast<int>(mouseButtonPressed->button),
                 .mouseX = mouseButtonPressed->position.x,
                 .mouseY = mouseButtonPressed->position.y});
            continue;
        }

        if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            inputSystem_.notify(
                {.type = InputEventType::MouseButtonReleased,
                 .mouseButton = static_cast<int>(mouseButtonReleased->button),
                 .mouseX = mouseButtonReleased->position.x,
                 .mouseY = mouseButtonReleased->position.y});
            continue;
        }

        if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            inputSystem_.notify(
                {.type = InputEventType::MouseMoved, .mouseX = mouseMoved->position.x, .mouseY = mouseMoved->position.y});
        }
    }
}
} // namespace redclone::engine::input
