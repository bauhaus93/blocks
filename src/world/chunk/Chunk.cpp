/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

Chunk::Chunk(const Point3i& chunkPos_, Blocktree&& blocktree_, mesh::Mesh&& mesh_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(CHUNK_SIZE) * BLOCK_SIZE },
    model {  CreateTranslationMatrix(origin) },
    blocktree { std::move(blocktree_) },
    mesh { std::make_unique<mesh::Mesh>(std::move(mesh_)) } {
}

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(CHUNK_SIZE) * BLOCK_SIZE },
    model {  CreateTranslationMatrix(origin) },
    blocktree { },
    mesh { nullptr } {
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

}       // namespace mc::world::chunk
