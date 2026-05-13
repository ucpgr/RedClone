#include "engine/assets/TileAsset.h"

namespace redclone::engine::assets
{
void TileAssetRegistry::registerSheet(TileSheetDefinition sheet)
{
    m_Sheets.push_back(std::move(sheet));
    auto& storedSheet = m_Sheets.back();
    for (auto& tile : storedSheet.tiles)
    {
        m_ByName[tile.name] = &tile;
    }
}

const TileDefinition* TileAssetRegistry::findTile(std::string_view name) const
{
    const auto it = m_ByName.find(std::string(name));
    if (it == m_ByName.end())
    {
        return nullptr;
    }

    return it->second;
}
} // namespace redclone::engine::assets
