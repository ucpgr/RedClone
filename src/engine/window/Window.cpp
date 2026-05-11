#include "engine/window/Window.h"

#include <SFML/Window/VideoMode.hpp>

namespace redclone::engine::window
{
Window::Window() : window_(sf::VideoMode({1280u, 720u}), "RedClone")
{
    window_.setFramerateLimit(60);
}

bool Window::isOpen() const
{
    return window_.isOpen();
}

void Window::close()
{
    window_.close();
}

void Window::pollEvents()
{
}

void Window::display()
{
    window_.display();
}

sf::RenderWindow& Window::nativeWindow()
{
    return window_;
}
} // namespace redclone::engine::window
