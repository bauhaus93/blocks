/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(CHUNK_SIZE) * BLOCK_SIZE },
    model {  CreateTranslationMatrix(origin) },
    checkedBorders { },
    blocktree { },
    mesh { nullptr } {
}

void Chunk::InsertBlocks(const std::vector<BlockElement>& blocks) {
    blocktree.InsertBlocks(blocks);
}

void Chunk::CreateMesh(const BlockManager& blockManager) {
    std::vector<mesh::Quad> quads;
    LayerFaces faces = blocktree.CreateFaces();
    for (uint8_t axis = 0; axis < 3; axis++) {
        for(auto& layer: faces[axis]) {
            if (layer.first == 0 || layer.first == CHUNK_SIZE) {
                borderFaces[axis].emplace(layer.first, std::move(layer.second));
            } else {
                Facetree ft;
                ft.InsertFaces(std::move(layer.second));
                ft.CreateQuads(blockManager, axis, layer.first, quads);
            }
        }
    }
    mesh = std::make_unique<mesh::Mesh>(quads);
}

void Chunk::UpdateBorders(Chunk& neighbour, Direction border, const BlockManager& blockManager) {
    assert(!checkedBorders.Contains(border));
    assert(!neighbour.checkedBorders.Contains(GetOpposite(border)));
    uint8_t axis = 0;
    uint8_t layer = 0;
    switch (border) {
    case Direction::EAST:   layer = CHUNK_SIZE; axis = 0;   break;
    case Direction::WEST:   layer = 0;          axis = 0;   break;
    case Direction::SOUTH:  layer = CHUNK_SIZE; axis = 1;   break;
    case Direction::NORTH:  layer = 0;          axis = 1;   break;
    case Direction::UP:     layer = CHUNK_SIZE; axis = 2;   break;
    case Direction::DOWN:   layer = 0;          axis = 2;   break;
    default: assert(0);
    }
    uint8_t layerNb = layer == 0 ? CHUNK_SIZE : 0;
    Facetree ft;
    auto findCurr = borderFaces[axis].find(layer);
    if (findCurr != borderFaces[axis].end()) {
        ft.InsertFaces(std::move(findCurr->second));
        borderFaces[axis].erase(findCurr);
    }
    auto findNb = borderFaces[axis].find(layerNb);
    if (findNb != borderFaces[axis].end()) {
        ft.InsertFaces(std::move(findNb->second));
        borderFaces[axis].erase(findNb);
    }
    if (mesh != nullptr) {
        std::vector<mesh::Quad> quads;
        ft.CreateQuadsByDirection(blockManager, axis, layer, quads, border);
        //mesh->AddQuads(quads);
        mesh = std::make_unique<mesh::Mesh>(quads);
    }
    if (neighbour.mesh != nullptr) {
        std::vector<mesh::Quad> quads;
        ft.CreateQuadsByDirection(blockManager, axis, layerNb, quads, GetOpposite(border));
        //neighbour.mesh->AddQuads(quads);
        mesh = std::make_unique<mesh::Mesh>(quads);
    }

    checkedBorders.Add(border);
    neighbour.checkedBorders.Add(GetOpposite(border));
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

}       // namespace mc::world::chunk
