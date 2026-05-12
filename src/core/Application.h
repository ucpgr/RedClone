#pragma once

#include "engine/camera/Camera2D.h"
#include "engine/input/IInputObserver.h"
#include "engine/input/InputSystem.h"
#include "engine/input/SfmlInputAdapter.h"
#include "engine/math/Vec2.h"
#include "engine/rendering/DebugRenderer.h"
#include "engine/rendering/SfmlRenderer.h"
#include "engine/window/Window.h"
#include "gameplay/SelectionController.h"
#include "world/TileMap.h"

#include <unordered_set>

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
    void drawWorld();

    static constexpr float c_CameraSpeed = 500.0F;

    engine::window::Window m_Window;
    engine::rendering::SfmlRenderer m_Renderer;
    engine::rendering::DebugRenderer m_DebugRenderer;
    engine::input::InputSystem m_InputSystem;
    engine::input::SfmlInputAdapter m_InputAdapter;
    engine::camera::Camera2D m_Camera;
    world::TileMap m_TileMap;
    gameplay::SelectionController m_SelectionController;
    std::unordered_set<int> m_HeldKeys;
};
} // namespace redclone::core
