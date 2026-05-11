#include "engine/input/InputSystem.h"

#include <algorithm>

namespace redclone::engine::input
{
void InputSystem::addObserver(IInputObserver& observer)
{
    if (std::find(observers_.begin(), observers_.end(), &observer) == observers_.end())
    {
        observers_.push_back(&observer);
    }
}

void InputSystem::removeObserver(IInputObserver& observer)
{
    observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer), observers_.end());
}

void InputSystem::notify(const InputEvent& event) const
{
    for (auto* observer : observers_)
    {
        if (observer != nullptr)
        {
            observer->onInputEvent(event);
        }
    }
}
} // namespace redclone::engine::input
