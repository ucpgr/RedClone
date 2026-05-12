#pragma once

#include "engine/math/MathTypes.h"

namespace redclone::engine::camera
{
class Camera2D
{
  public:
    static constexpr float c_MinZoom = 0.25F;
    static constexpr float c_MaxZoom = 4.0F;

    void setPosition(const math::Vec2f& position);
    void move(const math::Vec2f& delta);
    [[nodiscard]] math::Vec2f getPosition() const;

    void setZoom(float zoom);
    void zoomBy(float deltaZoom);
    [[nodiscard]] float getZoom() const;

    void setViewportSize(const math::Vec2f& viewportSize);
    [[nodiscard]] math::Vec2f getViewportSize() const;

    [[nodiscard]] math::Vec2f screenToWorld(const math::Vec2f& screen) const;
    [[nodiscard]] math::Vec2f worldToScreen(const math::Vec2f& world) const;

  private:
    math::Vec2f m_Position{};
    float m_Zoom{1.0F};
    math::Vec2f m_ViewportSize{1280.0F, 720.0F};
};
} // namespace redclone::engine::camera
