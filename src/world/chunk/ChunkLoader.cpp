/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ChunkLoader.hpp"

namespace mc::world::chunk {



void RunGeneration(Arguments& args) {
    args.chunk.Generate(args.heightNoise, args.texture);
}

ChunkLoader::ChunkLoader(const Point3i& pos,
                         const Point3i& chunkSize,
                         const Point3i& blockSize,
                         const SimplexNoise& heightNoise,
                         const Texture& texture):
    finished { false },
    retrieved { false },
    chunk { pos, chunkSize, blockSize },
    args { chunk, heightNoise, texture },
    thread(&RunGeneration, args) {
    TRACE("Launching thread for loading chunk ", pos);
    thread.launch();
}

void ChunkLoader::Wait() {
    if (!finished) {
        thread.wait();
        finished = true;
    }
}

Chunk&& ChunkLoader::GetChunk() {
    if (finished) {
        if (!retrieved) {
            retrieved = true;
            return std::move(chunk);
        } else {
            throw ApplicationError(
                "ChunkLoaderError",
                __FUNCTION__,
                "Wanted to retrieve already retrieved chunk");
        }
    } else {
        throw ApplicationError(
            "ChunkLoaderError",
            __FUNCTION__,
            "Wanted to retrieve not yet loaded chunk");
    }
}


}   // namespace mc::world::chunk
