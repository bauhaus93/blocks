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

void Chunk::UpdateBorder(const Chunk& neighbour, Direction border, const BlockManager& blockManager) {
    assert(!checkedBorders.Contains(border));
    if (mesh != nullptr) {
        //blocktree.UpdateMesh(*mesh, neighbour.blocktree, border, blockManager);
    }
    checkedBorders.Add(border);
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

}       // namespace mc::world::chunk
