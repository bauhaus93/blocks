/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(CHUNK_SIZE) * BLOCK_SIZE },
    model {  CreateTranslationMatrix(origin) },
    blocktree { nullptr },
    mesh { nullptr } {
}

void Chunk::Generate(const architect::Architect& architect) {
    std::vector<BlockElement> blockQueue;

    for (int8_t y = 0; y < CHUNK_SIZE; y++) {
        for (int8_t x = 0; x < CHUNK_SIZE; x++) {
            int8_t height = static_cast<int8_t>(architect.GetChunkRelativeHeight(chunkPos, Point2i8 { x, y } ));
            if (height >= 0) {
                Point3i8 curr { x, y, height };
                while (curr[2] >= 0) {
                    BlockType type = architect.GetBlockType(chunkPos, curr);
                    blockQueue.emplace_back(std::make_pair(curr, type));
                    curr[2]--;
                }
            }
        }
    }

    if (blockQueue.size() > 0) {
        blocktree = std::make_unique<Blocktree>();
        blocktree->InsertBlocks(blockQueue);
        mesh = std::make_unique<mesh::Mesh>(blocktree->CreateMesh(architect.GetBlockManager()));
    }
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

void Chunk::UpdateBorderFaces(Chunk& curr, Chunk& neighbour, Direction relation) {
    Direction opposite = GetOpposite(relation);

    bool checkCurr = !curr.GetCheckedNeighbours().Contains(relation);
    bool checkNb = !neighbour.GetCheckedNeighbours().Contains(opposite);

    if (checkCurr || checkNb) {
        std::vector<Face> faces;

        curr.GetBlocktree().CollectBorderFaces(faces, relation);
        neighbour.GetBlocktree().CollectBorderFaces(faces, opposite);

        std::sort(faces.begin(),
                  faces.end(),
                  [](const Face& a, const Face& b) {
                      return a.GetSize() > b.GetSize();
                  });
        Facetree tree;
        tree.InsertFaces(std::move(faces));
        faces.clear();
        tree.GetFaces(faces);
        Point3i8 currOffset(0);
        Point3i8 nbOffset(0);
        switch(relation) {
            case Direction::EAST:
                currOffset[0] = CHUNK_SIZE;
                nbOffset[0] = 0;
                break;
            case Direction::WEST:
                currOffset[0] = 0;
                nbOffset[0] = CHUNK_SIZE;
                break;
            case Direction::NORTH:
                currOffset[1] = 0;
                nbOffset[1] = CHUNK_SIZE;
                break;
            case Direction::SOUTH:
                currOffset[1] = CHUNK_SIZE;
                nbOffset[1] = 0;
                break;
            case Direction::UP:
                currOffset[2] = CHUNK_SIZE;
                nbOffset[2] = 0;
                break;
            case Direction::DOWN:
                currOffset[2] = 0;
                nbOffset[2] = CHUNK_SIZE;
                break;
            default:    assert(0);
        }
        for (auto& face: faces) {
            if (face.GetDirection() == relation && checkCurr) {

            }
            if (face.GetDirection() == opposite && checkNb) {

            }

        }
    }
}

}       // namespace mc::world::chunk
