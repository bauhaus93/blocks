/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <queue>
#include <future>
#include <thread>
#include <atomic>
#include <chrono>
#include <cassert>
#include <algorithm>
#include <set>

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
    explicit            ChunkLoader(uint32_t maxThreads_);

    void                Start();
    void                Stop();
    bool                IsRunning() const;
    bool                HasLoadedChunks();
    void                RequestChunks(const std::set<Point3i>& requestedChunkPos);
    std::vector<Chunk>  GetLoadedChunks();


 private:
    void                    Loop();
    void                    CheckFinishedFutures();
    void                    AddFutures();

    std::atomic<bool>       stop;
    std::mutex              handledMutex;
    std::mutex              finishedMutex;
    const uint32_t          maxThreads;
    const SimplexNoise      heightNoise;
    const Texture           texture;
    std::unique_ptr<std::thread> thread;
    ChunkFutureVec          futures;
    std::set<Chunk>         finishedChunks;
    std::set<Point3i>       handledChunkPos;
    uint32_t                activeFutures;
};

}   // namespace mc::world::chunk
