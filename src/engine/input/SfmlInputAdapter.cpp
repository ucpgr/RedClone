#include "engine/input/SfmlInputAdapter.h"

#include "engine/input/InputEvent.h"
#include "engine/input/InputSystem.h"
#include "engine/window/Window.h"

#include <SFML/Window/Event.hpp>

namespace redclone::engine::input
{
static Key mapKey(const sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Key::W:
        return Key::W;
    case sf::Keyboard::Key::A:
        return Key::A;
    case sf::Keyboard::Key::S:
        return Key::S;
    case sf::Keyboard::Key::D:
        return Key::D;
    case sf::Keyboard::Key::Up:
        return Key::Up;
    case sf::Keyboard::Key::Down:
        return Key::Down;
    case sf::Keyboard::Key::Left:
        return Key::Left;
    case sf::Keyboard::Key::Right:
        return Key::Right;
    case sf::Keyboard::Key::Escape:
        return Key::Escape;
    default:
        return Key::Unknown;
    }
}

static MouseButton mapButton(const sf::Mouse::Button button)
{
    switch (button)
    {
    case sf::Mouse::Button::Left:
        return MouseButton::Left;
    case sf::Mouse::Button::Right:
        return MouseButton::Right;
    case sf::Mouse::Button::Middle:
        return MouseButton::Middle;
    default:
        return MouseButton::Unknown;
    }
}

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
        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            m_InputSystem.notify({.type = InputEventType::WindowResized,
                                  .windowSize = {static_cast<int>(resized->size.x),
                                                 static_cast<int>(resized->size.y)}});
            continue;
        }
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            m_InputSystem.notify({.type = InputEventType::KeyPressed, .key = mapKey(keyPressed->code)});
            continue;
        }
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            m_InputSystem.notify({.type = InputEventType::KeyReleased, .key = mapKey(keyReleased->code)});
            continue;
        }
        if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            m_InputSystem.notify(
                {.type = InputEventType::MouseWheelScrolled, .mouseWheelDelta = wheel->delta});
            continue;
        }
        if (const auto* moved = event->getIf<sf::Event::MouseMoved>())
        {
            m_InputSystem.notify({.type = InputEventType::MouseMoved,
                                  .mousePosition = {moved->position.x, moved->position.y}});
            continue;
        }
        if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            m_InputSystem.notify({.type = InputEventType::MouseButtonPressed,
                                  .mouseButton = mapButton(pressed->button),
                                  .mousePosition = {pressed->position.x, pressed->position.y}});
            continue;
        }
        if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>())
        {
            m_InputSystem.notify({.type = InputEventType::MouseButtonReleased,
                                  .mouseButton = mapButton(released->button),
                                  .mousePosition = {released->position.x, released->position.y}});
        }
    }
}
} // namespace redclone::engine::input
