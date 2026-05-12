#pragma once

#include "engine/math/MathTypes.h"

#include <optional>

namespace redclone::gameplay
{
class SelectionController
{
  public:
    void selectTile(const engine::math::Vec2i& tileCoord);
    void clearSelection();
    [[nodiscard]] std::optional<engine::math::Vec2i> getSelectedTile() const;

  private:
    std::optional<engine::math::Vec2i> m_SelectedTile;
};
} // namespace redclone::gameplay
