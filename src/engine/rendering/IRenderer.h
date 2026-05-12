#pragma once

#include "engine/camera/Camera2D.h"
#include "engine/math/MathTypes.h"
#include "engine/rendering/Color.h"

namespace redclone::engine::rendering
{
class IRenderer
{
  public:
    virtual ~IRenderer() = default;
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void setCamera(const camera::Camera2D& camera) = 0;
    virtual void resetCamera() = 0;
    virtual void drawFilledRect(const math::Vec2f& worldPosition, const math::Vec2f& size,
                                const Color& color) = 0;
    virtual void drawOutlinedRect(const math::Vec2f& worldPosition, const math::Vec2f& size,
                                  const Color& fillColor, const Color& outlineColor,
                                  float outlineThickness) = 0;
};
} // namespace redclone::engine::rendering
