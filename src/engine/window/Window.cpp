#include "engine/window/Window.h"

#include <SFML/Window/VideoMode.hpp>

namespace redclone::engine::window
{
Window::Window() : m_Window(sf::VideoMode({1280u, 720u}), "RedClone") { m_Window.setFramerateLimit(60); }
bool Window::isOpen() const { return m_Window.isOpen(); }
void Window::close() { m_Window.close(); }
void Window::display() { m_Window.display(); }
sf::RenderWindow& Window::nativeWindow() { return m_Window; }
} // namespace redclone::engine::window
