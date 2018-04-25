/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(CHUNK_SIZE) * BLOCK_SIZE },
    model {  CreateTranslationMatrix(origin) * glm::mat4(1.0f) },
    blocktree { nullptr },
    mesh { nullptr } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    origin { other.origin },
    model { std::move(other.model) },
    blocktree { std::move(other.blocktree) },
    mesh { std::move(other.mesh) } {
}

Chunk& Chunk::operator=(Chunk&& other) {
    if (this != &other) {
        chunkPos = other.chunkPos;
        origin = other.origin;
        model = std::move(other.model);
        blocktree = std::move(other.blocktree);
        mesh = std::move(other.mesh);
    }
    return *this;
}

void Chunk::Generate(const architect::Architect& architect) {
    sf::Clock clock;
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
        blocktree = std::make_unique<Blocktree>(Point3i8(0), CHUNK_SIZE);
        blocktree->InsertBlocks(blockQueue);
        mesh = std::make_unique<mesh::Mesh>(blocktree->CreateMesh(architect.GetProtoBlocks()));
    }

    TRACE("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms");
}

void Chunk::CreateMesh() {
    /*mesh = CreateCompositeMesh(blocktree);
    if (mesh->IsEmpty()) {
        mesh = nullptr;
    }*/
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

}       // namespace mc::world::chunk
