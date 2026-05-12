#include "engine/rendering/DebugRenderer.h"

#include "engine/rendering/SfmlRenderer.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace redclone::engine::rendering
{
DebugRenderer::DebugRenderer(SfmlRenderer& renderer) : m_Renderer(renderer) {}

void DebugRenderer::drawRect(const camera::Camera2D& camera, const math::Vec2f& worldPosition, const math::Vec2f& size, const sf::Color& color) const
{
    drawOutlinedRect(camera, worldPosition, size, color, color, 0.0F);
}

void DebugRenderer::drawOutlinedRect(const camera::Camera2D& camera, const math::Vec2f& worldPosition, const math::Vec2f& size, const sf::Color& fillColor, const sf::Color& outlineColor, const float outlineThickness) const
{
    sf::RectangleShape rectangle;
    rectangle.setSize({size.x * camera.getZoom(), size.y * camera.getZoom()});
    const auto screenPos = camera.worldToScreen(worldPosition);
    rectangle.setPosition({screenPos.x, screenPos.y});
    rectangle.setFillColor(fillColor);
    rectangle.setOutlineColor(outlineColor);
    rectangle.setOutlineThickness(outlineThickness);
    m_Renderer.nativeWindow().draw(rectangle);
}
} // namespace redclone::engine::rendering
