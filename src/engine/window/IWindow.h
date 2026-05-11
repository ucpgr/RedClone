#pragma once

namespace redclone::engine::window
{
class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual bool isOpen() const = 0;
    virtual void close() = 0;
    virtual void pollEvents() = 0;
    virtual void display() = 0;
};
} // namespace redclone::engine::window
