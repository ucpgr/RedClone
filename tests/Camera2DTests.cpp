#include "core/CameraController.h"
#include "engine/camera/Camera2D.h"
#include "engine/input/InputEvent.h"
#include "gameplay/SelectionController.h"
#include "world/TileMap.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace
{
bool approx(const float a, const float b, const float epsilon = 0.0001F)
{
    return std::fabs(a - b) <= epsilon;
}

int runTests()
{
    using redclone::engine::camera::Camera2D;
    using redclone::engine::math::Vec2f;

    Camera2D camera;
    camera.setViewportSize({1280.0F, 720.0F});
    camera.setPosition({640.0F, 360.0F});
    camera.setZoom(2.0F);

    const Vec2f screen(640.0F, 360.0F);
    const auto world = camera.screenToWorld(screen);
    if (!approx(world[0], 640.0F) || !approx(world[1], 360.0F))
    {
        std::cerr << "Camera2D::screenToWorld failed\n";
        return 1;
    }

    const auto backToScreen = camera.worldToScreen(world);
    if (!approx(backToScreen[0], screen[0]) || !approx(backToScreen[1], screen[1]))
    {
        std::cerr << "Camera2D::worldToScreen failed\n";
        return 1;
    }

    redclone::world::TileMap tileMap;
    const auto tile = tileMap.worldToTile({64.0F, 96.0F});
    if (!tile || (*tile)[0] != 2 || (*tile)[1] != 3)
    {
        std::cerr << "TileMap::worldToTile failed\n";
        return 1;
    }

    if (tileMap.getTileTypeAt({-1, 0}).has_value())
    {
        std::cerr << "TileMap safe lookup failed\n";
        return 1;
    }

    redclone::gameplay::SelectionController selection;
    selection.selectTile({1, 2});
    if (!selection.getSelectedTile().has_value())
    {
        std::cerr << "SelectionController select failed\n";
        return 1;
    }
    selection.clearSelection();
    if (selection.getSelectedTile().has_value())
    {
        std::cerr << "SelectionController clear failed\n";
        return 1;
    }

    redclone::core::CameraController controller;
    redclone::engine::input::InputEvent press{};
    press.type = redclone::engine::input::InputEventType::KeyPressed;
    press.key = redclone::engine::input::Key::W;
    controller.onInputEvent(press);

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
} // namespace

int main()
{
    return runTests();
}
