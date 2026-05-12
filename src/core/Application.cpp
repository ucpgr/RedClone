#include "core/Application.h"

#include <SFML/System/Clock.hpp>

namespace redclone::core
{
Application::Application() : m_Renderer(m_Window), m_InputAdapter(m_Window, m_InputSystem)
{
    m_InputSystem.addObserver(*this);
    m_Camera.setViewportSize({1280.0F, 720.0F});
    m_Camera.setPosition({640.0F, 360.0F});
}
Application::~Application() { m_InputSystem.removeObserver(*this); }
int Application::run(){sf::Clock frameClock;while(m_Window.isOpen()){const float deltaSeconds=frameClock.restart().asSeconds();m_InputAdapter.pollEvents();updateCamera(deltaSeconds);m_Renderer.beginFrame();m_Renderer.setCamera(m_Camera);m_TileMapRenderer.render(m_Renderer,m_TileMap,m_SelectionController);m_Renderer.resetCamera();m_Renderer.endFrame();}return 0;}
void Application::onInputEvent(const engine::input::InputEvent& event)
{
    using namespace engine::input;
    if(event.type==InputEventType::Closed){m_Window.close();return;}
    if(event.type==InputEventType::KeyPressed){m_HeldKeys.insert(event.key);if(event.key==Key::Escape){m_Window.close();}}
    if(event.type==InputEventType::KeyReleased){m_HeldKeys.erase(event.key);}    
    if(event.type==InputEventType::MouseWheelScrolled){m_Camera.zoomBy(event.mouseWheelDelta*0.1F);}    
    if(event.type==InputEventType::MouseButtonPressed && event.mouseButton==MouseButton::Left){const auto worldPosition=m_Camera.screenToWorld({static_cast<float>(event.mousePosition[0]),static_cast<float>(event.mousePosition[1])}); if(const auto tileCoord=m_TileMap.worldToTile(worldPosition)){m_SelectionController.selectTile(*tileCoord);}else{m_SelectionController.clearSelection();}}
}
void Application::updateCamera(const float deltaSeconds)
{
    engine::math::Vec2f moveDelta(0.0F,0.0F);
    using engine::input::Key;
    if(m_HeldKeys.contains(Key::W)||m_HeldKeys.contains(Key::Up)){moveDelta[1]-=1.0F;}
    if(m_HeldKeys.contains(Key::S)||m_HeldKeys.contains(Key::Down)){moveDelta[1]+=1.0F;}
    if(m_HeldKeys.contains(Key::A)||m_HeldKeys.contains(Key::Left)){moveDelta[0]-=1.0F;}
    if(m_HeldKeys.contains(Key::D)||m_HeldKeys.contains(Key::Right)){moveDelta[0]+=1.0F;}
    m_Camera.move(moveDelta*(c_CameraSpeed*deltaSeconds));
}
}
