/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ChunkLoader.hpp"

namespace mc::world::chunk {

Chunk CreateChunk(Point3i pos,
                  const SimplexNoise& heightNoise,
                  const Texture& texture);

ChunkLoader::ChunkLoader(uint32_t maxThreads_):
    stop { false },
    pendingMutex { },
    finishedMutex { },
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

bool ChunkLoader::HasLoadedChunks() {
    bool retVal;
    finishedMutex.lock();
    retVal = !finishedChunks.empty();
    finishedMutex.unlock();
    return retVal;
}

void ChunkLoader::RequestChunk(const Point3i& chunkPos) {
    activeMutex.lock();
    if (std::find(activeChunks.begin(), activeChunks.end(), chunkPos) != activeChunks.end()) {
        activeMutex.unlock();
        TRACE("Requested chunk ", chunkPos, " for loading, but is currently being loaded");
        return;
    }
    activeMutex.unlock();

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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

                futures[i] = nullptr;
                activeMutex.lock();
                auto foundElement = std::find(activeChunks.begin(),
                                             activeChunks.end(),
                                             finishedChunks.back().GetPosition());
                assert(foundElement != activeChunks.end());
                activeChunks.erase(foundElement);
                activeMutex.unlock();
                finishedMutex.unlock();
                activeFutures--;
            }
        }
    }
}

void ChunkLoader::AddFutures() {
    pendingMutex.lock();
    activeMutex.lock();
    while (!pendingChunks.empty() && activeFutures < maxThreads) {
        Point3i chunkPos = pendingChunks.front();
        std::unique_ptr<std::future<Chunk>> fut = std::make_unique<std::future<Chunk>>(
            std::async(CreateChunk,
                       chunkPos,
                       std::cref(heightNoise),
                       std::cref(texture)));
        pendingChunks.pop();
        activeChunks.push_back(chunkPos);

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
    activeMutex.unlock();
    pendingMutex.unlock();
}

Chunk CreateChunk(Point3i pos,
                  const SimplexNoise& heightNoise,
                  const Texture& texture) {
    Chunk chunk { pos };
    chunk.Generate(heightNoise, texture);
    return chunk;
}



}   // namespace mc::world::chunk
