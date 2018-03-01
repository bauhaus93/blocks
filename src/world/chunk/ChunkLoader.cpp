/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ChunkLoader.hpp"

namespace mc::world::chunk {

Chunk CreateChunk(Point3i pos,
                  const Point3i& chunkSize,
                  const Point3f& blockSize,
                  const SimplexNoise& heightNoise,
                  const Texture& texture);

ChunkLoader::ChunkLoader(const Point3i& chunkSize_,
                         const Point3f& blockSize_,
                         uint32_t maxThreads_):
    stop { false },
    pendingMutex { },
    finishedMutex { },
    chunkSize(chunkSize_),
    blockSize(blockSize_),
    maxThreads { maxThreads_ },
    heightNoise { },
    texture { "grass.bmp" },
    thread { nullptr },
    futures { },
    pendingChunks { },
    finishedChunks { },
    activeFutures { 0 } {
    futures.reserve(maxThreads);
}

bool ChunkLoader::IsRunning() const {
    return thread != nullptr;
}

bool ChunkLoader::HasFinishedChunks() {
    bool retVal;
    finishedMutex.lock();
    retVal = !finishedChunks.empty();
    finishedMutex.unlock();
    return retVal;
}

void ChunkLoader::RequestChunk(const Point3i& chunkPos) {
    pendingMutex.lock();
    pendingChunks.push(chunkPos);
    pendingMutex.unlock();
}

std::vector<Chunk> ChunkLoader::GetLoadedChunks() {
    std::vector<Chunk> newChunks;
    finishedMutex.lock();
    newChunks.swap(finishedChunks);
    finishedMutex.unlock();
    return newChunks;
}

void ChunkLoader::Start() {
    if (!IsRunning()) {
        DEBUG("Starting ChunkLoader thread");
        thread = std::make_unique<std::thread>(&ChunkLoader::Loop, this);
    } else {
        DEBUG("Wanted to start ChunkLoader thread, but was already running");
    }
}

void ChunkLoader::Stop() {
    DEBUG("Requesting ChunkLoader thread to stop");
    stop = true;
    thread->join();
    thread = nullptr;
}

void ChunkLoader::Loop() {
    DEBUG("ChunkLoader thread started");
    while (!stop) {
        CheckFinishedFutures();
        AddFutures();
        if (activeFutures == maxThreads) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    DEBUG("ChunkLoader thread finished");
    stop = false;
}

void ChunkLoader::CheckFinishedFutures() {
    static const std::chrono::milliseconds time(0);
    for (uint32_t i = 0; i < futures.size(); i++) {
        if (futures[i] != nullptr) {
            if (futures[i]->wait_for(time) == std::future_status::ready) {
                finishedMutex.lock();
                finishedChunks.emplace_back(futures[i]->get());
                finishedMutex.unlock();
                futures[i] = nullptr;
                activeFutures--;
            }
        }
    }
}

void ChunkLoader::AddFutures() {
    pendingMutex.lock();
    while (!pendingChunks.empty() && activeFutures < maxThreads) {
        Point3i chunkPos = pendingChunks.front();
        std::unique_ptr<std::future<Chunk>> fut = std::make_unique<std::future<Chunk>>(
            std::async(CreateChunk,
                       chunkPos,
                       std::cref(chunkSize),
                       std::cref(blockSize),
                       std::cref(heightNoise),
                       std::cref(texture)));
        pendingChunks.pop();

        if (futures.size() < maxThreads) {
            activeFutures++;
            futures.emplace_back(std::move(fut));
        } else {
            bool added = false;
            for (uint32_t i = 0; i < futures.size(); i++) {
                if (futures[i] == nullptr) {
                    activeFutures++;
                    futures[i] = std::move(fut);
                    added = true;
                    break;
                }
            }
            assert(added);
        }
    }

    pendingMutex.unlock();
}

Chunk CreateChunk(Point3i pos,
                  const Point3i& chunkSize,
                  const Point3f& blockSize,
                  const SimplexNoise& heightNoise,
                  const Texture& texture) {
    Chunk chunk { pos, chunkSize, blockSize };
    chunk.Generate(heightNoise, texture);
    return chunk;
}



}   // namespace mc::world::chunk
