#include "engine/input/InputSystem.h"

#include <algorithm>

namespace redclone::engine::input
{
void InputSystem::addObserver(IInputObserver& observer)
{
    if (std::find(m_Observers.begin(), m_Observers.end(), &observer) == m_Observers.end())
    {
        m_Observers.push_back(&observer);
    }
}

void InputSystem::removeObserver(IInputObserver& observer)
{
    m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), &observer), m_Observers.end());
}

void InputSystem::notify(const InputEvent& event) const
{
    for (auto* observer : m_Observers)
    {
        if (observer != nullptr)
        {
            observer->onInputEvent(event);
        }
    }
}
} // namespace redclone::engine::input
