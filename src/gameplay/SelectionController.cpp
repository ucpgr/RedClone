#include "gameplay/SelectionController.h"

namespace redclone::gameplay
{
void SelectionController::selectTile(const engine::math::Vec2i& tileCoord)
{
    m_SelectedTile = tileCoord;
}
void SelectionController::clearSelection()
{
    m_SelectedTile.reset();
}
std::optional<engine::math::Vec2i> SelectionController::getSelectedTile() const
{
    return m_SelectedTile;
}
} // namespace redclone::gameplay
