#include "engine/camera/Camera2D.h"

#include "TestCommon.h"

#include <iostream>

int runCamera2DTests()
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

    return 0;
}
