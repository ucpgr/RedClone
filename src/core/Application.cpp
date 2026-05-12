#include "core/Application.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace redclone::core
{
Application::Application() : m_Renderer(m_Window), m_DebugRenderer(m_Renderer), m_InputAdapter(m_Window, m_InputSystem)
{
    m_InputSystem.addObserver(*this);
    const auto windowSize = m_Renderer.nativeWindow().getSize();
    m_Camera.setViewportSize({static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});
    m_Camera.setPosition({windowSize.x * 0.5F, windowSize.y * 0.5F});
}

Application::~Application() { m_InputSystem.removeObserver(*this); }

int Application::run()
{
    sf::Clock frameClock;
    while (m_Window.isOpen())
    {
        const float deltaSeconds = frameClock.restart().asSeconds();
        m_InputAdapter.pollEvents();
        updateCamera(deltaSeconds);
        m_Renderer.beginFrame();
        drawWorld();
        m_Renderer.endFrame();
    }
    return 0;
}

void Application::onInputEvent(const engine::input::InputEvent& event)
{
    using engine::input::InputEventType;
    if (event.type == InputEventType::Closed)
    {
        m_Window.close();
        return;
    }
    if (event.type == InputEventType::KeyPressed)
    {
        m_HeldKeys.insert(event.key);
        if (event.key == static_cast<int>(sf::Keyboard::Key::Escape))
        {
            m_Window.close();
        }
    }
    if (event.type == InputEventType::KeyReleased)
    {
        m_HeldKeys.erase(event.key);
    }
    if (event.type == InputEventType::MouseWheelScrolled)
    {
        m_Camera.zoomBy(event.mouseWheelDelta * 0.1F);
    }
    if (event.type == InputEventType::MouseButtonPressed && event.mouseButton == static_cast<int>(sf::Mouse::Button::Left))
    {
        const auto worldPosition = m_Camera.screenToWorld({static_cast<float>(event.mouseX), static_cast<float>(event.mouseY)});
        if (const auto tileCoord = m_TileMap.worldToTile(worldPosition))
        {
            m_SelectionController.selectTile(*tileCoord);
        }
        else
        {
            m_SelectionController.clearSelection();
        }
    }
}

void Application::updateCamera(const float deltaSeconds)
{
    engine::math::Vec2f moveDelta{};
    if (m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::W)) || m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::Up))) { moveDelta.y -= 1.0F; }
    if (m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::S)) || m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::Down))) { moveDelta.y += 1.0F; }
    if (m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::A)) || m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::Left))) { moveDelta.x -= 1.0F; }
    if (m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::D)) || m_HeldKeys.contains(static_cast<int>(sf::Keyboard::Key::Right))) { moveDelta.x += 1.0F; }
    m_Camera.move(moveDelta * (c_CameraSpeed * deltaSeconds));
}

void Application::drawWorld()
{
    constexpr engine::math::Vec2f tileSize{static_cast<float>(world::TileMap::c_TileSize), static_cast<float>(world::TileMap::c_TileSize)};
    for (int y = 0; y < world::TileMap::c_Height; ++y)
    {
        for (int x = 0; x < world::TileMap::c_Width; ++x)
        {
            const auto type = m_TileMap.getTileTypeAt({x, y});
            sf::Color tileColor = sf::Color(70, 135, 70);
            if (type == world::TileType::Dirt) { tileColor = sf::Color(130, 100, 60); }
            if (type == world::TileType::Blocked) { tileColor = sf::Color(80, 80, 80); }
            const engine::math::Vec2f worldPosition{static_cast<float>(x * world::TileMap::c_TileSize), static_cast<float>(y * world::TileMap::c_TileSize)};
            m_DebugRenderer.drawOutlinedRect(m_Camera, worldPosition, tileSize, tileColor, sf::Color(30, 30, 30), -1.0F);
        }
    }

    if (const auto selected = m_SelectionController.getSelectedTile())
    {
        const engine::math::Vec2f selectedWorldPosition{static_cast<float>(selected->x * world::TileMap::c_TileSize), static_cast<float>(selected->y * world::TileMap::c_TileSize)};
        m_DebugRenderer.drawOutlinedRect(m_Camera, selectedWorldPosition, tileSize, sf::Color::Transparent, sf::Color::Yellow, -3.0F);
    }
}
} // namespace redclone::core
