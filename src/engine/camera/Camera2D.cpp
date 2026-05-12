#include "engine/camera/Camera2D.h"

#include <algorithm>

namespace redclone::engine::camera
{
void Camera2D::setPosition(const math::Vec2f& position) { m_Position = position; }
void Camera2D::move(const math::Vec2f& delta) { m_Position += delta; }
math::Vec2f Camera2D::getPosition() const { return m_Position; }

void Camera2D::setZoom(const float zoom) { m_Zoom = std::clamp(zoom, c_MinZoom, c_MaxZoom); }
void Camera2D::zoomBy(const float deltaZoom) { setZoom(m_Zoom + deltaZoom); }
float Camera2D::getZoom() const { return m_Zoom; }

void Camera2D::setViewportSize(const math::Vec2f& viewportSize) { m_ViewportSize = viewportSize; }
math::Vec2f Camera2D::getViewportSize() const { return m_ViewportSize; }

math::Vec2f Camera2D::screenToWorld(const math::Vec2f& screen) const
{
    const auto viewportCenter = m_ViewportSize * 0.5F;
    return ((screen - viewportCenter) * (1.0F / m_Zoom)) + m_Position;
}

math::Vec2f Camera2D::worldToScreen(const math::Vec2f& world) const
{
    const auto viewportCenter = m_ViewportSize * 0.5F;
    return ((world - m_Position) * m_Zoom) + viewportCenter;
}
} // namespace redclone::engine::camera
