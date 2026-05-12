#include "world/TileMap.h"
namespace redclone::world
{
TileMap::TileMap() : m_Tiles(c_Width * c_Height){for(int y=0;y<c_Height;++y){for(int x=0;x<c_Width;++x){auto& tile=m_Tiles[static_cast<std::size_t>(y*c_Width+x)];if((x+y)%13==0){tile.type=TileType::Blocked;}else if((x*3+y*7)%5==0){tile.type=TileType::Dirt;}else{tile.type=TileType::Grass;}}}}
std::span<const Tile> TileMap::getTiles() const { return m_Tiles; }
std::optional<engine::math::Vec2i> TileMap::worldToTile(const engine::math::Vec2f& worldPosition) const { const int tileX=static_cast<int>(worldPosition[0])/c_TileSize; const int tileY=static_cast<int>(worldPosition[1])/c_TileSize; if(tileX<0||tileY<0||tileX>=c_Width||tileY>=c_Height){return std::nullopt;} return engine::math::Vec2i(tileX,tileY);} 
TileType TileMap::getTileTypeAt(const engine::math::Vec2i& coord) const { return m_Tiles[static_cast<std::size_t>(coord[1]*c_Width+coord[0])].type; }
}
