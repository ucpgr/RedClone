#pragma once

#include "engine/rendering/IRenderer.h"

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

private:
    window::Window& window_;
};
} // namespace redclone::engine::rendering
