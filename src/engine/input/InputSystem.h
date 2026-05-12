#pragma once

#include "engine/input/IInputObserver.h"

#include <vector>

namespace redclone::engine::input
{
class InputSystem
{
  public:
    void addObserver(IInputObserver& observer);
    void removeObserver(IInputObserver& observer);
    void notify(const InputEvent& event) const;

  private:
    std::vector<IInputObserver*> m_Observers;
};
} // namespace redclone::engine::input
