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
    texture { "data/grass.bmp" },
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

void ChunkLoader::RequestChunks(const std::set<Point3i>& requestedChunkPos) {
    std::set<Point3i> handledUnion;
    handledMutex.lock();
    std::set_union(handledChunkPos.begin(),
                   handledChunkPos.end(),
                   requestedChunkPos.begin(),
                   requestedChunkPos.end(),
                   std::inserter(handledUnion, handledUnion.end())
                   );
    handledChunkPos.swap(handledUnion);
    handledMutex.unlock();
}

std::vector<Chunk> ChunkLoader::GetLoadedChunks() {
    std::vector<Chunk> newChunks;
    std::set<Point3i> handledDiff;
    handledMutex.lock();
    finishedMutex.lock();
    std::set_difference(handledChunkPos.begin(),
                        handledChunkPos.end(),
                        finishedChunks.begin(),
                        finishedChunks.end(),
                        std::inserter(handledDiff, handledDiff.end())
                        );
    handledChunkPos.swap(handledDiff);
    newChunks.swap(finishedChunks);
    finishedMutex.unlock();
    handledMutex.unlock();

    return newChunks;
}

void ChunkLoader::Start() {
    if (!IsRunning()) {
        DEBUG("Starting ChunkLoader thread");
        thread = std::make_unique<std::thread>(&ChunkLoader::Loop, this);
    } else {
        WARN("Wanted to start ChunkLoader thread, but was already running");
    }
}

void ChunkLoader::Stop() {
    DEBUG("Requesting ChunkLoader thread to stop");
    assert(thread != nullptr);
    stop = true;
    thread->join();
    thread = nullptr;
}

void ChunkLoader::Loop() {
    DEBUG("ChunkLoader thread started");
    stop = false;
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
}

void ChunkLoader::CheckFinishedFutures() {
    static const std::chrono::milliseconds time(0);
    for (uint32_t i = 0; i < futures.size(); i++) {
        if (futures[i] != nullptr) {
            if (futures[i]->wait_for(time) == std::future_status::ready) {
                finishedMutex.lock();
                finishedChunks.insert(futures[i]->get());
                finishedMutex.unlock();
                futures[i] = nullptr;
                activeFutures--;
            }
        }
    }
}

void ChunkLoader::AddFutures() {

    if (activeFutures < maxThreads) {
        handledMutex.lock();
        finishedMutex.lock();
        std::set<Point3i> pending;

        std::set_difference(handledChunkPos.begin(),
                            handledChunkPos.end(),
                            finishedChunks.begin(),
                            finishedChunks.end(),
                            std::inserter(pending, pending.end()));

        finishedMutex.unlock();
        handledMutex.unlock();
        TRACE("Pending chunks for generation: ", pending.size());
        for (auto pointIter = pending.begin();
             pointIter != pending.end() && activeFutures < maxThreads;
             ++pointIter) {
            std::unique_ptr<std::future<Chunk>> fut =
                std::make_unique<std::future<Chunk>>(std::async(
                    CreateChunk,*pointIter,
                    std::cref(heightNoise),
                    std::cref(texture)));
            if (futures.size() < maxThreads) {
                activeFutures++;
                futures.emplace_back(std::move(fut));
            } else {
                #ifndef NDEBUG
                bool added = false;
                #endif
                for (auto futIter = futures.begin();
                     futIter != futures.end();
                     ++futIter) {
                    if (*futIter== nullptr) {
                        activeFutures++;
                        *futIter = std::move(fut);
                        #ifndef NDEBUG
                        added = true;
                        #endif
                        break;
                    }
                }
                assert(added);
            }
        }
    }
}

Chunk CreateChunk(Point3i pos,
                  const SimplexNoise& heightNoise,
                  const Texture& texture) {
    Chunk chunk { pos };
    chunk.Generate(heightNoise, texture);
    return chunk;
}



}   // namespace mc::world::chunk
