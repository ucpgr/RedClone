#include "core/CameraController.h"
#include "engine/camera/Camera2D.h"
#include "engine/input/InputEvent.h"
#include "engine/input/Key.h"

#include <iostream>

int runCameraControllerTests()
{
    redclone::core::CameraController controller;

    redclone::engine::input::InputEvent press{};
    press.type = redclone::engine::input::InputEventType::KeyPressed;
    press.key = redclone::engine::input::Key::W;
    controller.onInputEvent(press);

    redclone::engine::input::InputEvent pressEscape{};
    pressEscape.type = redclone::engine::input::InputEventType::KeyPressed;
    pressEscape.key = redclone::engine::input::Key::Escape;
    controller.onInputEvent(pressEscape);

    redclone::engine::camera::Camera2D nonMoveCamera;
    nonMoveCamera.setPosition({12.0F, 34.0F});
    controller.update(nonMoveCamera, 1.0F);
    if (nonMoveCamera.getPosition()[0] != 12.0F || nonMoveCamera.getPosition()[1] != 34.0F)
    {
        std::cerr << "CameraController non-movement key handling failed\n";
        return 1;
    }

    redclone::engine::camera::Camera2D moveCamera;
    moveCamera.setPosition({0.0F, 0.0F});
    controller.update(moveCamera, 1.0F);
    if (!(moveCamera.getPosition()[1] < -400.0F))
    {
        std::cerr << "CameraController movement failed\n";
        return 1;
    }

    return 0;
}
