#include "engine/rendering/SfmlRenderer.h"

#include "engine/window/Window.h"

#include <SFML/Graphics/Color.hpp>

namespace redclone::engine::rendering
{
SfmlRenderer::SfmlRenderer(window::Window& window) : window_(window)
{
}

void SfmlRenderer::beginFrame()
{
    window_.nativeWindow().clear(sf::Color::Black);
}

void SfmlRenderer::endFrame()
{
    window_.display();
}
} // namespace redclone::engine::rendering
