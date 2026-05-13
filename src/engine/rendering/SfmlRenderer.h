#pragma once

#include "engine/rendering/IRenderer.h"

#include <span>

namespace sf
{
class RenderWindow;
}
namespace redclone::engine::window
{
class Window;
}

namespace redclone::engine::rendering
{
class SfmlRenderer final : public IRenderer
{
  public:
    explicit SfmlRenderer(window::Window& window);
    void beginFrame() override;
    void endFrame() override;
    void setCamera(const camera::Camera2D& camera) override;
    void resetCamera() override;
    void drawFilledRect(const math::Vec2f& worldPosition, const math::Vec2f& size,
                        const Color& color) override;
    void drawOutlinedRect(const math::Vec2f& worldPosition, const math::Vec2f& size, const Color& fillColor,
                          const Color& outlineColor, float outlineThickness) override;
    void drawConvexPolygon(std::span<const math::Vec2f> points, const Color& fillColor,
                           const Color& outlineColor, float outlineThickness) override;
    void drawTexturedSprite(const sf::Texture& texture, const math::IntRect& sourceRect,
                            const math::Vec2f& worldPosition) override;

  private:
    static auto toSfmlColor(const Color& color);
    window::Window& m_Window;
};
} // namespace redclone::engine::rendering
