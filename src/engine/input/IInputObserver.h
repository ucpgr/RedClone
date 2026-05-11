#pragma once

#include "engine/input/InputEvent.h"

namespace redclone::engine::input
{
class IInputObserver
{
public:
    virtual ~IInputObserver() = default;
    virtual void onInputEvent(const InputEvent& event) = 0;
};
} // namespace redclone::engine::input
