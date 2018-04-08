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
#include <list>

#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"
#include "world/Architect.hpp"
#include "ApplicationError.hpp"
#include "Chunk.hpp"

namespace mc::world::chunk {

typedef std::list<std::unique_ptr<std::future<Chunk>>> ChunkFutures;

class ChunkLoader {

 public:
    explicit            ChunkLoader(uint32_t maxThreads_, const Architect& architect_);

    void                Start();
    void                Stop();
    bool                IsRunning() const;
    bool                HasFinishedChunks();
    void                RequestChunks(const std::vector<Point3i>& requestedChunkPos);
    std::vector<Chunk>  GetFinishedChunks();


 private:
    void                    Loop();
    void                    HandleFinishedThreads();
    void                    CreateGenerationThreads();

    std::atomic<bool>       stop;
    std::mutex              pendingMutex;
    std::mutex              finishedMutex;
    const uint32_t          maxThreads;
    const Architect&        architect;
    std::unique_ptr<std::thread> controlThread;
    ChunkFutures            generationThreads;
    std::vector<Chunk>      finishedChunks;
    std::list<Point3i>      pendingChunkPos;
};

}   // namespace mc::world::chunk
