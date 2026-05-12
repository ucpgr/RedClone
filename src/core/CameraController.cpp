#include "core/CameraController.h"

#include <cmath>

namespace redclone::core
{
void CameraController::onInputEvent(const engine::input::InputEvent& event)
{
    using namespace engine::input;

    if (event.type == InputEventType::KeyPressed)
    {
        m_HeldKeys.insert(event.key);
    }

    if (event.type == InputEventType::KeyReleased)
    {
        m_HeldKeys.erase(event.key);
    }
}

void CameraController::update(engine::camera::Camera2D& camera, const float deltaSeconds) const
{
    engine::math::Vec2f moveDelta(0.0F, 0.0F);

    if (isMovementKeyHeld(engine::input::Key::W) || isMovementKeyHeld(engine::input::Key::Up))
    {
        moveDelta[1] -= 1.0F;
    }
    if (isMovementKeyHeld(engine::input::Key::S) || isMovementKeyHeld(engine::input::Key::Down))
    {
        moveDelta[1] += 1.0F;
    }
    if (isMovementKeyHeld(engine::input::Key::A) || isMovementKeyHeld(engine::input::Key::Left))
    {
        moveDelta[0] -= 1.0F;
    }
    if (isMovementKeyHeld(engine::input::Key::D) || isMovementKeyHeld(engine::input::Key::Right))
    {
        moveDelta[0] += 1.0F;
    }

    const float lengthSquared = (moveDelta[0] * moveDelta[0]) + (moveDelta[1] * moveDelta[1]);
    if (lengthSquared > 0.0F)
    {
        const float inverseLength = 1.0F / std::sqrt(lengthSquared);
        moveDelta *= inverseLength;
    }

    camera.move(moveDelta * (c_CameraSpeed * deltaSeconds));
}

bool CameraController::isMovementKeyHeld(const engine::input::Key key) const
{
    return m_HeldKeys.contains(key);
}
} // namespace redclone::core
