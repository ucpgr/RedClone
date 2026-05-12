#include "engine/rendering/SfmlRenderer.h"

#include "engine/window/Window.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>

namespace redclone::engine::rendering
{
SfmlRenderer::SfmlRenderer(window::Window& window) : m_Window(window) {}
void SfmlRenderer::beginFrame()
{
    m_Window.nativeWindow().clear(sf::Color::Black);
}
void SfmlRenderer::endFrame()
{
    m_Window.display();
}
void SfmlRenderer::setCamera(const camera::Camera2D& camera)
{
    sf::View view;
    const auto viewport = camera.getViewportSize();
    view.setSize({viewport[0] / camera.getZoom(), viewport[1] / camera.getZoom()});
    const auto pos = camera.getPosition();
    view.setCenter({pos[0], pos[1]});
    m_Window.nativeWindow().setView(view);
}
void SfmlRenderer::resetCamera()
{
    m_Window.nativeWindow().setView(m_Window.nativeWindow().getDefaultView());
}
auto SfmlRenderer::toSfmlColor(const Color& color)
{
    return sf::Color(color.r, color.g, color.b, color.a);
}
void SfmlRenderer::drawFilledRect(const math::Vec2f& worldPosition, const math::Vec2f& size,
                                  const Color& color)
{
    drawOutlinedRect(worldPosition, size, color, color, 0.0F);
}
void SfmlRenderer::drawOutlinedRect(const math::Vec2f& worldPosition, const math::Vec2f& size,
                                    const Color& fillColor, const Color& outlineColor,
                                    const float outlineThickness)
{
    sf::RectangleShape rectangle;
    rectangle.setPosition({worldPosition[0], worldPosition[1]});
    rectangle.setSize({size[0], size[1]});
    rectangle.setFillColor(toSfmlColor(fillColor));
    rectangle.setOutlineColor(toSfmlColor(outlineColor));
    rectangle.setOutlineThickness(outlineThickness);
    m_Window.nativeWindow().draw(rectangle);
}
} // namespace redclone::engine::rendering
