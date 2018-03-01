/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <SFML/System.hpp>

#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"
#include "graphics/Texture.hpp"
#include "ApplicationError.hpp"
#include "Chunk.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::chunk {

struct Arguments {
    Chunk& chunk;
    const SimplexNoise& heightNoise;
    const Texture& texture;
};

class ChunkLoader {

 public:
                    ChunkLoader(const Point3i& pos,
                                const Point3i& chunkSize,
                                const Point3i& blockSize,
                                const SimplexNoise& heightNoise,
                                const Texture& texture);
                    ChunkLoader(const ChunkLoader& other) = delete;
                    ChunkLoader(ChunkLoader&& other) = delete;
    void            Wait();
    bool            AlreadyRetrieved() const { return retrieved; }
    Chunk&&         GetChunk();
 private:
     bool           finished;
     bool           retrieved;
     Chunk          chunk;
     Arguments      args;
     sf::Thread     thread;
};

}   // namespace mc::world::chunk
