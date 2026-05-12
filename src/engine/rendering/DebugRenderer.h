#pragma once

#include "engine/camera/Camera2D.h"
#include "engine/math/Vec2.h"

namespace sf
{
class Color;
}

namespace redclone::engine::rendering
{
class SfmlRenderer;

class DebugRenderer
{
public:
    explicit DebugRenderer(SfmlRenderer& renderer);

    void drawRect(const camera::Camera2D& camera, const math::Vec2f& worldPosition, const math::Vec2f& size, const sf::Color& color) const;
    void drawOutlinedRect(const camera::Camera2D& camera, const math::Vec2f& worldPosition, const math::Vec2f& size, const sf::Color& fillColor, const sf::Color& outlineColor, float outlineThickness) const;

private:
    SfmlRenderer& m_Renderer;
};
} // namespace redclone::engine::rendering
