/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ChunkLoader.hpp"

namespace mc::world::chunk {

static Chunk CreateChunk(Point3i pos,
                  const Architect& architect);

ChunkLoader::ChunkLoader(uint32_t maxThreads_, const Architect& architect_):
    stop { false },
    pendingMutex { },
    finishedMutex { },
    maxThreads { maxThreads_ },
    architect { architect_ },
    controlThread { nullptr },
    generationThreads { },
    finishedChunks { } {
}

bool ChunkLoader::IsRunning() const {
    return controlThread != nullptr;
}

bool ChunkLoader::HasFinishedChunks() {
    bool retVal;
    finishedMutex.lock();
    retVal = !finishedChunks.empty();
    finishedMutex.unlock();
    return retVal;
}

//Currently allowing loaded same chunk multiple times!
void ChunkLoader::RequestChunks(const std::vector<Point3i>& requestedChunkPos) {
    pendingMutex.lock();
    pendingChunkPos.insert(pendingChunkPos.end(), requestedChunkPos.begin(), requestedChunkPos.end());
    pendingMutex.unlock();
}

std::map<Point3i, Chunk> ChunkLoader::GetFinishedChunks() {
    std::map<Point3i, Chunk> retMap;
    
    finishedMutex.lock();
    retMap.swap(finishedChunks);
    finishedMutex.unlock();

    return retMap;
}

void ChunkLoader::Start() {
    if (!IsRunning()) {
        DEBUG("Starting ChunkLoader thread");
        controlThread = std::make_unique<std::thread>(&ChunkLoader::Loop, this);
    } else {
        WARN("Wanted to start ChunkLoader thread, but was already running");
    }
}

void ChunkLoader::Stop() {
    DEBUG("Requesting ChunkLoader thread to stop");
    if (IsRunning()) {
        stop = true;
        controlThread->join();
        controlThread = nullptr;
    } else {
        WARN("Wanted to stop ChunkLoaded thread, but was not running");
    }
}

void ChunkLoader::Loop() {
    DEBUG("ChunkLoader thread started");
    stop = false;
    while (!stop) {
        HandleFinishedThreads();
        CreateGenerationThreads();
        TRACE("Active generation threads: ", generationThreads.size());
        if (generationThreads.size() == maxThreads) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else if (generationThreads.size() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    DEBUG("ChunkLoader thread finished");
}

void ChunkLoader::HandleFinishedThreads() {
    auto iter = generationThreads.begin();
    while (iter != generationThreads.end()) {
        if (iter->get()->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            finishedMutex.lock();
            Chunk chunk = iter->get()->get();
            finishedChunks.emplace(chunk.GetPosition(), std::move(chunk));
            finishedMutex.unlock();
            iter = generationThreads.erase(iter); 
        } else {
            ++iter;
        }
    }
}

void ChunkLoader::CreateGenerationThreads() {
    pendingMutex.lock();
    auto iter = pendingChunkPos.begin();
    while (iter != pendingChunkPos.end() && 
           generationThreads.size() < maxThreads) {
        
        std::unique_ptr<std::future<Chunk>> fut =
            std::make_unique<std::future<Chunk>>(std::async(
                CreateChunk,
                *iter, std::cref(architect)
            )
        );
        generationThreads.push_back(std::move(fut));
        iter = pendingChunkPos.erase(iter);
    }
    pendingMutex.unlock();
}

static Chunk CreateChunk(Point3i pos,
                  const Architect& architect) {
    Chunk chunk { pos };
    chunk.Generate(architect);
    return chunk;
}

}   // namespace mc::world::chunk
