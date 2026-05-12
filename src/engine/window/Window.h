#pragma once

#include "engine/window/IWindow.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace redclone::engine::input
{
class SfmlInputAdapter;
}
namespace redclone::engine::rendering
{
class SfmlRenderer;
}

namespace redclone::engine::window
{
class Window final : public IWindow
{
  public:
    Window();

    bool isOpen() const override;
    void close() override;
    void display() override;

  private:
    sf::RenderWindow m_Window;

    sf::RenderWindow& nativeWindow();

    friend class redclone::engine::input::SfmlInputAdapter;
    friend class redclone::engine::rendering::SfmlRenderer;
};
} // namespace redclone::engine::window
