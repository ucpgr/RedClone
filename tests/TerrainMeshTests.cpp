#include "world/TerrainMesh.h"
#include "world/Tile.h"

#include "TestCommon.h"

#include <iostream>
#include <vector>

int runTerrainMeshTests()
{
    std::vector<redclone::world::Tile> tiles{
        {redclone::world::TileType::Grass, 0},
        {redclone::world::TileType::Dirt, 2},
        {redclone::world::TileType::Blocked, 1},
        {redclone::world::TileType::Grass, 1},
    };

    redclone::world::TerrainMesh mesh;
    mesh.rebuild(tiles, 2, 2);

    if (mesh.getVertices().size() != 9)
    {
        std::cerr << "TerrainMesh should create shared grid vertices\n";
        return 1;
    }
    if (mesh.getFaces().size() != 4)
    {
        std::cerr << "TerrainMesh should create one surface face per source tile\n";
        return 1;
    }

    const auto center = mesh.getVertices()[4].position;
    if (!approx(center[0], 1.0F) || !approx(center[1], 1.0F) || !approx(center[2], 1.0F))
    {
        std::cerr << "TerrainMesh should average neighboring heights at shared corners\n";
        return 1;
    }

    const auto& dirtFace = mesh.getFaces()[1];
    if (dirtFace.tileCoord[0] != 1 || dirtFace.tileCoord[1] != 0)
    {
        std::cerr << "TerrainMesh should retain source tile coordinates on faces\n";
        return 1;
    }
    if (dirtFace.type != redclone::world::TileType::Dirt)
    {
        std::cerr << "TerrainMesh should preserve tile terrain types on faces\n";
        return 1;
    }
    if (dirtFace.shade <= 0.0F)
    {
        std::cerr << "TerrainMesh should calculate a usable face shade\n";
        return 1;
    }

    std::vector<redclone::world::Tile> southEastHighTiles{
        {redclone::world::TileType::Grass, 2},
        {redclone::world::TileType::Grass, 1},
        {redclone::world::TileType::Grass, 1},
        {redclone::world::TileType::Grass, 0},
    };
    std::vector<redclone::world::Tile> northWestFacingTiles{
        {redclone::world::TileType::Grass, 0},
        {redclone::world::TileType::Grass, 1},
        {redclone::world::TileType::Grass, 1},
        {redclone::world::TileType::Grass, 2},
    };
    redclone::world::TerrainMesh southEastHighMesh;
    redclone::world::TerrainMesh northWestFacingMesh;
    redclone::world::TerrainMesh flatMesh;
    southEastHighMesh.rebuild(southEastHighTiles, 2, 2);
    northWestFacingMesh.rebuild(northWestFacingTiles, 2, 2);
    for (auto& tile : southEastHighTiles)
    {
        tile.height = 0;
    }
    flatMesh.rebuild(southEastHighTiles, 2, 2);
    const float northWestFacingShade = northWestFacingMesh.getFaces()[0].shade;
    const float southEastHighShade = southEastHighMesh.getFaces()[0].shade;
    const float flatShade = flatMesh.getFaces()[0].shade;
    if (northWestFacingShade <= southEastHighShade)
    {
        std::cerr << "TerrainMesh should use a north-west point light for face shading\n";
        return 1;
    }
    if (flatShade <= southEastHighShade)
    {
        std::cerr << "TerrainMesh flat tops should receive direct north-west light, not read as shadowed\n";
        return 1;
    }
    if ((northWestFacingShade - southEastHighShade) < 0.05F)
    {
        std::cerr << "TerrainMesh north-west point light should produce visible slope contrast\n";
        return 1;
    }

    tiles[1].height = 0;
    mesh.updateTile(tiles, 2, 2, {1, 0});
    const auto updatedCenter = mesh.getVertices()[4].position;
    if (!approx(updatedCenter[2], 0.5F))
    {
        std::cerr << "TerrainMesh partial update should refresh affected shared corner heights\n";
        return 1;
    }

    return 0;
}
