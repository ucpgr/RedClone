#include "engine/assets/TileAsset.h"

namespace redclone::engine::assets
{
std::size_t TileAssetRegistry::TransparentStringHash::operator()(std::string_view value) const noexcept
{
    return std::hash<std::string_view>{}(value);
}

std::size_t TileAssetRegistry::TransparentStringHash::operator()(const std::string& value) const noexcept
{
    return (*this)(std::string_view{value});
}

std::size_t TileAssetRegistry::TransparentStringHash::operator()(const char* value) const noexcept
{
    return (*this)(std::string_view{value});
}

void TileAssetRegistry::registerSheet(TileSheetDefinition sheet, std::shared_ptr<sf::Texture> texture)
{
    const std::size_t sheetIndex = m_Sheets.size();
    m_Sheets.push_back({std::move(sheet), std::move(texture)});

    const auto& tiles = m_Sheets.back().definition.tiles;
    for (std::size_t tileIndex = 0; tileIndex < tiles.size(); ++tileIndex)
    {
        m_ByName[tiles[tileIndex].name] = {sheetIndex, tileIndex};
    }
}

const TileDefinition* TileAssetRegistry::findTile(std::string_view name) const
{
    const auto lookup = findTileWithTexture(name);
    return lookup.tile;
}

TileLookup TileAssetRegistry::findTileWithTexture(std::string_view name) const
{
    const auto it = m_ByName.find(name);
    if (it == m_ByName.end())
    {
        return {};
    }

    const auto [sheetIndex, tileIndex] = it->second;
    const auto& record = m_Sheets.at(sheetIndex);
    if (tileIndex >= record.definition.tiles.size())
    {
        return {};
    }

    return {&record.definition.tiles[tileIndex], record.texture.get()};
}

std::size_t TileAssetRegistry::sheetCount() const { return m_Sheets.size(); }

std::size_t TileAssetRegistry::tileCount() const { return m_ByName.size(); }

bool TileAssetRegistry::containsTile(std::string_view name) const
{
    return m_ByName.contains(name);
}
} // namespace redclone::engine::assets
