/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace blocks{

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
    std::vector<Quad> quads;
    LayerFaces faces = blocktree.CreateFaces();
    for (uint8_t axis = 0; axis < 3; axis++) {
        for(auto& layer: faces[axis]) {
            if (layer.first != 0 && layer.first != CHUNK_SIZE) {
                Facetree ft;
                ft.InsertFaces(std::move(layer.second));
                ft.CreateQuads(blockManager, axis, layer.first, quads);
            }
        }
    }
    mesh = std::make_unique<Mesh>(quads);
}

void Chunk::UpdateBorders(Chunk& neighbour, Direction border, const BlockManager& blockManager) {
    assert(!checkedBorders.Contains(border));
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
    std::vector<Face> faces;
    if (mesh != nullptr) {
        faces = blocktree.CreateFaces(layer, axis);
    }
    if (neighbour.mesh != nullptr) {
        std::vector<Face> newFaces;
        newFaces = neighbour.blocktree.CreateFaces(layerNb, axis);
        faces.insert(faces.end(), newFaces.begin(), newFaces.end());
        std::sort(faces.begin(), faces.end(),
                  [](const Face& a, const Face& b) {
                      return a.GetSize() > b.GetSize();
                  });
    }
    ft.InsertFaces(std::move(faces));

    if (mesh != nullptr) {
        std::vector<Quad> quads;
        ft.CreateQuadsByDirection(blockManager, axis, layer, quads, border);
        mesh->AddQuads(quads);
        //mesh = std::make_unique<Mesh>(quads);
    }
    if (neighbour.mesh != nullptr) {
        std::vector<Quad> quads;
        ft.CreateQuadsByDirection(blockManager, axis, layerNb, quads, GetOpposite(border));
        neighbour.mesh->AddQuads(quads);
        //neighbour.mesh = std::make_unique<Mesh>(quads);
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

}       // namespace chunk
