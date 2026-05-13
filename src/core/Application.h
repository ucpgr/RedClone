#pragma once

#include "core/CameraController.h"
#include "engine/assets/TileAsset.h"
#include "engine/camera/Camera2D.h"
#include "engine/input/IInputObserver.h"
#include "engine/input/InputSystem.h"
#include "engine/input/SfmlInputAdapter.h"
#include "engine/rendering/SfmlRenderer.h"
#include "engine/window/Window.h"
#include "gameplay/SelectionController.h"
#include "world/TileMap.h"
#include "world/TileMapRenderer.h"
#include "world/World.h"

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
    static constexpr float c_MaxDeltaSeconds = 1.0F / 15.0F;

    engine::window::Window m_Window;
    engine::rendering::SfmlRenderer m_Renderer;
    engine::input::InputSystem m_InputSystem;
    engine::input::SfmlInputAdapter m_InputAdapter;
    engine::camera::Camera2D m_Camera;
    CameraController m_CameraController;
    world::TileMap m_TileMap;
    world::TileMapRenderer m_TileMapRenderer;
    gameplay::SelectionController m_SelectionController;
    world::World m_World;
    engine::assets::TileAssetRegistry m_TileAssets;
};
} // namespace redclone::core
