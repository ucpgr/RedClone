#include "engine/rendering/SfmlRenderer.h"

#include "engine/window/Window.h"

#include <SFML/Graphics/Color.hpp>

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

sf::RenderWindow& SfmlRenderer::nativeWindow()
{
    return m_Window.nativeWindow();
}
} // namespace redclone::engine::rendering
