#pragma once

#include "engine/rendering/IRenderer.h"

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

    sf::RenderWindow& nativeWindow();

private:
    window::Window& m_Window;
};
} // namespace redclone::engine::rendering
