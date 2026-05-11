#pragma once

namespace redclone::engine::rendering
{
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
};
} // namespace redclone::engine::rendering
