#pragma once

#include "engine/camera/Camera2D.h"
#include "engine/input/IInputObserver.h"
#include "engine/input/InputSystem.h"
#include "engine/input/Key.h"
#include "engine/input/SfmlInputAdapter.h"
#include "engine/rendering/SfmlRenderer.h"
#include "engine/window/Window.h"
#include "gameplay/SelectionController.h"
#include "world/TileMap.h"
#include "world/TileMapRenderer.h"

#include <set>

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
    void updateCamera(float deltaSeconds);
    static constexpr float c_CameraSpeed = 500.0F;
    engine::window::Window m_Window;
    engine::rendering::SfmlRenderer m_Renderer;
    engine::input::InputSystem m_InputSystem;
    engine::input::SfmlInputAdapter m_InputAdapter;
    engine::camera::Camera2D m_Camera;
    world::TileMap m_TileMap;
    world::TileMapRenderer m_TileMapRenderer;
    gameplay::SelectionController m_SelectionController;
    std::set<engine::input::Key> m_HeldKeys;
};
}
