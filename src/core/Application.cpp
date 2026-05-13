#include "core/Application.h"

#include <SFML/System/Clock.hpp>

#include "engine/assets/TileAssetLoader.h"
#include "engine/math/Isometric.h"
#include <iostream>

#include <algorithm>

namespace redclone::core
{
Application::Application() : m_Renderer(m_Window), m_InputAdapter(m_Window, m_InputSystem)
{
    std::string tileAssetError;
    if (!engine::assets::TileAssetLoader::loadDirectory("assets/tiles", m_TileAssets, tileAssetError))
    {
        std::cerr << "Tile assets failed to load: " << tileAssetError << "\n";
    }
    m_TileMapRenderer.setTileAssets(&m_TileAssets);
    m_InputSystem.addObserver(*this);
    m_Camera.setViewportSize({1280.0F, 720.0F});
    m_Camera.setPosition(engine::math::isometric::worldToIso({32.0F, 32.0F}));
}

Application::~Application()
{
    m_InputSystem.removeObserver(*this);
}

int Application::run()
{
    sf::Clock frameClock;

    while (m_Window.isOpen())
    {
        const float deltaSeconds = std::min(frameClock.restart().asSeconds(), c_MaxDeltaSeconds);

        m_InputAdapter.pollEvents();
        m_CameraController.update(m_Camera, deltaSeconds);
        m_World.update(deltaSeconds);

        m_Renderer.beginFrame();
        m_Renderer.setCamera(m_Camera);

        m_TileMapRenderer.render(m_Renderer, m_TileMap, m_SelectionController);
        m_World.render(m_Renderer);
        m_Renderer.resetCamera();
        m_Renderer.endFrame();
    }

    return 0;
}

void Application::onInputEvent(const engine::input::InputEvent& event)
{
    using namespace engine::input;

    m_CameraController.onInputEvent(event);

    if (event.type == InputEventType::Closed)
    {
        m_Window.close();
        return;
    }

    if (event.type == InputEventType::KeyPressed && event.key == Key::Escape)
    {
        m_Window.close();
    }

    if (event.type == InputEventType::MouseWheelScrolled)
    {
        m_Camera.zoomBy(event.mouseWheelDelta * 0.1F);
    }

    if (event.type == InputEventType::MouseButtonPressed)
    {
        const auto renderPosition = m_Camera.screenToWorld(
            {static_cast<float>(event.mousePosition[0]), static_cast<float>(event.mousePosition[1])});
        const auto worldPosition = engine::math::isometric::isoToWorld(renderPosition);

        if (event.mouseButton == MouseButton::Left)
        {
            m_World.selectUnitAt(worldPosition);
            const bool hasSelectedUnit = m_World.hasSelectedUnits();

            if (!hasSelectedUnit)
            {
                if (const auto tileCoord = m_TileMap.worldToTile(worldPosition))
                {
                    m_SelectionController.selectTile(*tileCoord);
                }
                else
                {
                    m_SelectionController.clearSelection();
                }
            }
            else
            {
                m_SelectionController.clearSelection();
            }
        }

        if (event.mouseButton == MouseButton::Right)
        {
            m_World.issueMoveCommandToSelected(worldPosition);
        }
    }
}
} // namespace redclone::core
