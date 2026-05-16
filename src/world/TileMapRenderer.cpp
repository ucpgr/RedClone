#include "world/TileMapRenderer.h"
#include "core/Logger.h"

#include "engine/math/Isometric.h"

#include <algorithm>
#include <array>
#include <vector>

namespace redclone::world
{
void TileMapRenderer::setTileAssets(const engine::assets::TileAssetRegistry* tileAssets)
{
    (void)tileAssets;
}

void TileMapRenderer::render(engine::rendering::IRenderer& renderer, const TileMap& tileMap,
                             const gameplay::SelectionController& selectionController) const
{
    if (!m_LoggedMeshPath)
    {
        REDCLONE_LOG_INFO("TileMapRenderer using projected terrain mesh rendering.");
        m_LoggedMeshPath = true;
    }

    const auto& terrainMesh = tileMap.getTerrainMesh();
    const auto vertices = terrainMesh.getVertices();
    std::vector<const TerrainFace*> drawFaces;
    for (const auto& face : terrainMesh.getFaces())
    {
        drawFaces.push_back(&face);
    }
    std::ranges::sort(drawFaces,
                      [](const TerrainFace* lhs, const TerrainFace* rhs) { return lhs->sortKey < rhs->sortKey; });

    for (const auto* face : drawFaces)
    {
        const std::array points = {
            projectTerrainVertex(vertices[face->vertexIndices[0]].position),
            projectTerrainVertex(vertices[face->vertexIndices[1]].position),
            projectTerrainVertex(vertices[face->vertexIndices[2]].position),
            projectTerrainVertex(vertices[face->vertexIndices[3]].position),
        };
        renderer.drawConvexPolygon(points, terrainColor(face->type, face->shade), {24, 32, 28, 190}, -0.75F);
    }

    if (const auto selected = selectionController.getSelectedTile())
    {
        for (const auto& face : terrainMesh.getFaces())
        {
            if (face.tileCoord != *selected)
            {
                continue;
            }
            const std::array points = {
                projectTerrainVertex(vertices[face.vertexIndices[0]].position),
                projectTerrainVertex(vertices[face.vertexIndices[1]].position),
                projectTerrainVertex(vertices[face.vertexIndices[2]].position),
                projectTerrainVertex(vertices[face.vertexIndices[3]].position),
            };
            renderer.drawConvexPolygon(points, {0, 0, 0, 0}, {255, 255, 0, 255}, -3.0F);
            break;
        }
    }
}

engine::math::Vec2f TileMapRenderer::projectTerrainVertex(const engine::math::Vec3f& position)
{
    return engine::math::isometric::worldToIso(position);
}

engine::rendering::Color TileMapRenderer::terrainColor(const TileType type, const float shade)
{
    engine::rendering::Color base{74, 139, 74, 255};
    if (type == TileType::Dirt)
    {
        base = {136, 103, 62, 255};
    }
    if (type == TileType::Blocked)
    {
        base = {86, 88, 86, 255};
    }

    return {static_cast<unsigned char>(std::clamp(static_cast<float>(base.r) * shade, 0.0F, 255.0F)),
            static_cast<unsigned char>(std::clamp(static_cast<float>(base.g) * shade, 0.0F, 255.0F)),
            static_cast<unsigned char>(std::clamp(static_cast<float>(base.b) * shade, 0.0F, 255.0F)), base.a};
}
} // namespace redclone::world
