/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ChunkLoader.hpp"

namespace mc::world::chunk {

static Chunk CreateChunk(Point3i pos,
                  const architect::Architect& architect);
static void Sleep(unsigned ms);

ChunkLoader::ChunkLoader(uint32_t maxThreads_,
                         const architect::Architect& architect_):
    stop { false },
    pendingMutex { },
    finishedMutex { },
    maxThreads { maxThreads_ },
    architect { architect_ },
    controlThread { nullptr },
    generationThreads { },
    finishedChunks { } {
}

ChunkLoader::~ChunkLoader() {
    Stop();
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

void ChunkLoader::RequestChunks(const std::vector<Point3i>& requestedChunkPos) {
    pendingMutex.lock();
    pendingChunkPos.insert(pendingChunkPos.end(), requestedChunkPos.begin(), requestedChunkPos.end());
    pendingChunkPos.sort();
    pendingChunkPos.unique();
    INFO("Chunks in loading queue: ", pendingChunkPos.size());
    pendingMutex.unlock();
}

std::vector<Chunk> ChunkLoader::GetFinishedChunks() {
    std::vector<Chunk> retVec;
    
    finishedMutex.lock();
    retVec.swap(finishedChunks);
    finishedMutex.unlock();

    return retVec;
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
        auto genCount = generationThreads.size();
        TRACE("Active generation threads: ", genCount);
        if (genCount == maxThreads) {
            Sleep(10);
        } else if (genCount == 0) {
            Sleep(500);
        } else {
            Sleep(50);
        }
    }
    DEBUG("Waiting for generation threads to finish");
    while (!generationThreads.empty()) {
        HandleFinishedThreads();
        Sleep(50);
    }
    DEBUG("All generation threads finished");
    DEBUG("ChunkLoader thread stopped");
}

void ChunkLoader::HandleFinishedThreads() {
    auto iter = generationThreads.begin();
    while (iter != generationThreads.end()) {
        if (iter->get()->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            finishedMutex.lock();
            Chunk chunk = iter->get()->get();
            finishedChunks.emplace_back(std::move(chunk));
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
                std::launch::async,
                CreateChunk,
                *iter, std::cref(architect)
            )
        );
        generationThreads.push_back(std::move(fut));
        iter = pendingChunkPos.erase(iter);
    }
    pendingMutex.unlock();
}

void Sleep(unsigned ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

Chunk CreateChunk(Point3i pos, const architect::Architect& architect) {
    Chunk chunk { pos };
    chunk.Generate(architect);
    return chunk;
}

}   // namespace mc::world::chunk
