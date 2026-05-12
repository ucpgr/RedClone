#pragma once

#include "engine/camera/Camera2D.h"
#include "engine/input/InputEvent.h"
#include "engine/input/Key.h"

#include <set>

namespace redclone::core
{
class CameraController
{
  public:
    void onInputEvent(const engine::input::InputEvent& event);
    void update(engine::camera::Camera2D& camera, float deltaSeconds) const;

  private:
    [[nodiscard]] bool isMovementKeyHeld(engine::input::Key key) const;

    static constexpr float c_CameraSpeed = 500.0F;
    std::set<engine::input::Key> m_HeldKeys;
};
} // namespace redclone::core
