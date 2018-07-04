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

    // including neighbouring border blocks
    for (int8_t y = -1; y <= CHUNK_SIZE; y++) {
        for (int8_t x = -1; x <= CHUNK_SIZE; x++) {
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

}       // namespace mc::world::chunk
