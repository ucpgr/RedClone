#include "core/Application.h"
#include "core/Logger.h"

#include <SFML/System/Clock.hpp>

#include "engine/assets/TileAssetLoader.h"
#include "engine/math/Isometric.h"
#include <algorithm>

namespace redclone::core
{
Application::Application() : m_Renderer(m_Window), m_InputAdapter(m_Window, m_InputSystem)
{
    REDCLONE_LOG_INFO("Application startup.");
#ifndef REDCLONE_ASSET_DIR
#define REDCLONE_ASSET_DIR "assets"
#endif
    const std::string tileAssetDirectory = std::string(REDCLONE_ASSET_DIR) + "/tiles";
    REDCLONE_LOG_INFO(std::string("Configured asset directory: ") + REDCLONE_ASSET_DIR);

    std::string tileAssetError;
    if (!engine::assets::TileAssetLoader::loadDirectory(tileAssetDirectory, m_TileAssets, tileAssetError))
    {
        REDCLONE_LOG_WARNING(std::string("Tile assets failed to load: ") + tileAssetError);
    }
    REDCLONE_LOG_INFO(std::string("Tile asset result: sheets=") + std::to_string(m_TileAssets.sheetCount()) +
                      ", tiles=" + std::to_string(m_TileAssets.tileCount()));
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
        m_World.render(m_Renderer, m_TileMap);
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

    if (event.type == InputEventType::WindowResized)
    {
        m_Camera.setViewportSize(
            {static_cast<float>(event.windowSize[0]), static_cast<float>(event.windowSize[1])});
    }

    if (event.type == InputEventType::MouseButtonPressed)
    {
        const auto renderPosition = m_Camera.screenToWorld(
            {static_cast<float>(event.mousePosition[0]), static_cast<float>(event.mousePosition[1])});
        const auto terrainHit = m_TileMap.isoToTerrainHit(renderPosition);
        const auto worldPosition = terrainHit ? terrainHit->worldPosition : engine::math::isometric::isoToWorld(renderPosition);
        REDCLONE_LOG_DEBUG(std::string("Mouse press screen(") + std::to_string(event.mousePosition[0]) + "," +
                           std::to_string(event.mousePosition[1]) + ") iso(" + std::to_string(renderPosition[0]) +
                           "," + std::to_string(renderPosition[1]) + ") world(" + std::to_string(worldPosition[0]) +
                           "," + std::to_string(worldPosition[1]) + ")");

        if (event.mouseButton == MouseButton::Left)
        {
            m_World.selectUnitAt(renderPosition, m_TileMap);
            const bool hasSelectedUnit = m_World.hasSelectedUnits();

            if (!hasSelectedUnit)
            {
                if (terrainHit.has_value())
                {
                    m_SelectionController.selectTile(terrainHit->tileCoord);
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
            if (terrainHit.has_value())
            {
                m_World.issueMoveCommandToSelected(terrainHit->worldPosition);
            }
        }
    }
}
} // namespace redclone::core
