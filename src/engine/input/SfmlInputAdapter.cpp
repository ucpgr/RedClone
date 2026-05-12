#include "engine/input/SfmlInputAdapter.h"

#include "engine/input/InputEvent.h"
#include "engine/input/InputSystem.h"
#include "engine/window/Window.h"

#include <SFML/Window/Event.hpp>

namespace redclone::engine::input
{
SfmlInputAdapter::SfmlInputAdapter(window::Window& window, InputSystem& inputSystem)
    : m_Window(window), m_InputSystem(inputSystem)
{
}

void SfmlInputAdapter::pollEvents()
{
    while (const auto event = m_Window.nativeWindow().pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_InputSystem.notify({.type = InputEventType::Closed});
            continue;
        }
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            m_InputSystem.notify({.type = InputEventType::KeyPressed, .key = static_cast<int>(keyPressed->code)});
            continue;
        }
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            m_InputSystem.notify({.type = InputEventType::KeyReleased, .key = static_cast<int>(keyReleased->code)});
            continue;
        }
        if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            m_InputSystem.notify({.type = InputEventType::MouseWheelScrolled, .mouseWheelDelta = mouseWheelScrolled->delta});
            continue;
        }
        if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            m_InputSystem.notify({.type = InputEventType::MouseMoved, .mouseX = mouseMoved->position.x, .mouseY = mouseMoved->position.y});
            continue;
        }
        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            m_InputSystem.notify({.type = InputEventType::MouseButtonPressed, .mouseButton = static_cast<int>(mouseButtonPressed->button), .mouseX = mouseButtonPressed->position.x, .mouseY = mouseButtonPressed->position.y});
            continue;
        }
        if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            m_InputSystem.notify({.type = InputEventType::MouseButtonReleased, .mouseButton = static_cast<int>(mouseButtonReleased->button), .mouseX = mouseButtonReleased->position.x, .mouseY = mouseButtonReleased->position.y});
        }
    }
}
} // namespace redclone::engine::input
