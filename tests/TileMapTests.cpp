#include "world/TileMap.h"
#include "world/TileMapGenerator.h"

#include "engine/math/Isometric.h"

#include <iostream>
#include <vector>

int runTileMapTests()
{
    redclone::world::TileMap tileMap;
    const auto tile = tileMap.worldToTile({2.2F, 3.7F});
    if (!tile || (*tile)[0] != 2 || (*tile)[1] != 3)
    {
        std::cerr << "TileMap::worldToTile failed\n";
        return 1;
    }

    const redclone::engine::math::Vec2i invalidCoords[] = {
        {-1, 0},
        {redclone::world::TileMap::c_Width, 0},
        {0, redclone::world::TileMap::c_Height},
        {redclone::world::TileMap::c_Width, redclone::world::TileMap::c_Height},
        {9999, 9999},
    };

    for (const auto& coord : invalidCoords)
    {
        if (tileMap.getTileTypeAt(coord).has_value())
        {
            std::cerr << "TileMap safe lookup failed\n";
            return 1;
        }
        if (tileMap.getTileHeightAt(coord).has_value())
        {
            std::cerr << "TileMap safe height lookup failed\n";
            return 1;
        }
    }

    for (int y = 0; y < redclone::world::TileMap::c_Height; ++y)
    {
        for (int x = 0; x < redclone::world::TileMap::c_Width; ++x)
        {
            const auto height = tileMap.getTileHeightAt({x, y});
            if (!height || *height < redclone::world::TileMapGenerator::c_MinHeight ||
                *height > redclone::world::TileMapGenerator::c_MaxHeight)
            {
                std::cerr << "Tile height should always be in range [0,2]\n";
                return 1;
            }
        }
    }

    const auto tileSpan = tileMap.getTiles();
    std::vector<redclone::world::Tile> tileCopy(tileSpan.begin(), tileSpan.end());
    if (!redclone::world::TileMapGenerator::hasValidHeightTransitions(tileCopy, redclone::world::TileMap::c_Width,
                                                                       redclone::world::TileMap::c_Height))
    {
        std::cerr << "Tile height transitions should be valid\n";
        return 1;
    }

    std::vector<redclone::world::Tile> generatedA(redclone::world::TileMap::c_Width * redclone::world::TileMap::c_Height);
    std::vector<redclone::world::Tile> generatedB(redclone::world::TileMap::c_Width * redclone::world::TileMap::c_Height);
    redclone::world::TileMapGenerator::generateTestMap(generatedA, redclone::world::TileMap::c_Width,
                                                        redclone::world::TileMap::c_Height);
    redclone::world::TileMapGenerator::generateTestMap(generatedB, redclone::world::TileMap::c_Width,
                                                        redclone::world::TileMap::c_Height);

    if (generatedA.size() != generatedB.size())
    {
        std::cerr << "Deterministic generation size mismatch\n";
        return 1;
    }
    for (std::size_t i = 0; i < generatedA.size(); ++i)
    {
        if (generatedA[i].height != generatedB[i].height)
        {
            std::cerr << "Deterministic generation produced different heights\n";
            return 1;
        }
    }

    const redclone::engine::math::Vec2i pickCoord{12, 9};
    const auto pickFaceIndex = static_cast<std::size_t>((pickCoord[1] * redclone::world::TileMap::c_Width) + pickCoord[0]);
    const auto& pickMesh = tileMap.getTerrainMesh();
    const auto& pickFace = pickMesh.getFaces()[pickFaceIndex];
    redclone::engine::math::Vec2f projectedCenter{0.0F, 0.0F};
    for (const auto vertexIndex : pickFace.vertexIndices)
    {
        projectedCenter += redclone::engine::math::isometric::worldToIso(pickMesh.getVertices()[vertexIndex].position);
    }
    projectedCenter *= 0.25F;

    const auto pickedTile = tileMap.isoToTerrainTile(projectedCenter);
    if (!pickedTile || (*pickedTile)[0] != pickCoord[0] || (*pickedTile)[1] != pickCoord[1])
    {
        std::cerr << "TileMap mesh-aware picking should select the projected terrain face\n";
        return 1;
    }
    const auto pickedHit = tileMap.isoToTerrainHit(projectedCenter);
    if (!pickedHit || pickedHit->tileCoord[0] != pickCoord[0] || pickedHit->tileCoord[1] != pickCoord[1])
    {
        std::cerr << "TileMap mesh-aware picking should provide terrain hit data\n";
        return 1;
    }
    if (tileMap.sampleHeightAt(pickedHit->worldPosition) < 0.0F)
    {
        std::cerr << "TileMap terrain height sampling should produce usable heights\n";
        return 1;
    }

    const redclone::engine::math::Vec2i edgePickCoord{pickCoord[0] + 1, pickCoord[1]};
    const auto edgeFaceIndex =
        static_cast<std::size_t>((edgePickCoord[1] * redclone::world::TileMap::c_Width) + edgePickCoord[0]);
    const auto& edgeFace = pickMesh.getFaces()[edgeFaceIndex];
    auto edgeMidpoint =
        redclone::engine::math::isometric::worldToIso(pickMesh.getVertices()[pickFace.vertexIndices[1]].position) +
        redclone::engine::math::isometric::worldToIso(pickMesh.getVertices()[pickFace.vertexIndices[2]].position);
    edgeMidpoint *= 0.5F;
    const auto edgePickedTile = tileMap.isoToTerrainTile(edgeMidpoint);
    if (!edgePickedTile ||
        !(((*edgePickedTile)[0] == pickCoord[0] && (*edgePickedTile)[1] == pickCoord[1]) ||
          ((*edgePickedTile)[0] == edgeFace.tileCoord[0] && (*edgePickedTile)[1] == edgeFace.tileCoord[1])))
    {
        std::cerr << "TileMap mesh-aware picking should handle shared face edges\n";
        return 1;
    }

    const redclone::engine::math::Vec2i editedPickCoord{3, 4};
    if (!tileMap.setTileHeightAt(editedPickCoord, redclone::world::TileMapGenerator::c_MaxHeight))
    {
        std::cerr << "TileMap height edit failed\n";
        return 1;
    }
    const auto editedFaceIndex =
        static_cast<std::size_t>((editedPickCoord[1] * redclone::world::TileMap::c_Width) + editedPickCoord[0]);
    const auto& editedMesh = tileMap.getTerrainMesh();
    const auto& editedFace = editedMesh.getFaces()[editedFaceIndex];
    redclone::engine::math::Vec2f editedProjectedCenter{0.0F, 0.0F};
    for (const auto vertexIndex : editedFace.vertexIndices)
    {
        editedProjectedCenter += redclone::engine::math::isometric::worldToIso(editedMesh.getVertices()[vertexIndex].position);
    }
    editedProjectedCenter *= 0.25F;
    const auto editedPickedTile = tileMap.isoToTerrainTile(editedProjectedCenter);
    if (!editedPickedTile || (*editedPickedTile)[0] != editedPickCoord[0] || (*editedPickedTile)[1] != editedPickCoord[1])
    {
        std::cerr << "TileMap mesh-aware picking should use rebuilt terrain heights\n";
        return 1;
    }

    const auto outsidePick = tileMap.isoToTerrainTile({-999999.0F, -999999.0F});
    if (outsidePick.has_value())
    {
        std::cerr << "TileMap mesh-aware picking should reject points outside the terrain\n";
        return 1;
    }

    return 0;
}
