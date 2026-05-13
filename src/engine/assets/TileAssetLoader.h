#pragma once

#include "engine/assets/TileAsset.h"

#include <string>

namespace redclone::engine::assets
{
class TileAssetLoader
{
  public:
    static bool loadMetadataFile(const std::string& filePath, TileSheetDefinition& out, std::string& error);
    static bool loadDirectory(const std::string& directoryPath, TileAssetRegistry& out, std::string& error);
};
} // namespace redclone::engine::assets
