/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <queue>
#include <future>
#include <thread>
#include <atomic>
#include <chrono>
#include <cassert>

#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"
#include "graphics/Texture.hpp"
#include "ApplicationError.hpp"
#include "Chunk.hpp"
#include "SimplexNoise.hpp"



namespace mc::world::chunk {

typedef std::vector<std::unique_ptr<std::future<Chunk>>> ChunkFutureVec;

class ChunkLoader {

 public:
                ChunkLoader(const Point3i& chunkSize_,
                            const Point3f& blockSize_,
                            uint32_t maxThreads_);

    void                Start();
    void                Stop();
    bool                IsRunning() const;
    bool                HasFinishedChunks();
    void                RequestChunk(const Point3i& chunkPos);
    std::vector<Chunk>  GetLoadedChunks();


 private:
    void                    Loop();
    void                    CheckFinishedFutures();
    void                    AddFutures();

    std::atomic<bool>       stop;
    std::mutex              pendingMutex;
    std::mutex              finishedMutex;
    const Point3i           chunkSize;
    const Point3f           blockSize;
    const uint32_t          maxThreads;
    const SimplexNoise      heightNoise;
    const Texture           texture;
    std::unique_ptr<std::thread> thread;
    ChunkFutureVec          futures;
    std::queue<Point3i>     pendingChunks;
    std::vector<Chunk>      finishedChunks;
    uint32_t                activeFutures;
};

}   // namespace mc::world::chunk
