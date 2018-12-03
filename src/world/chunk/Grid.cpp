/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace blocks{

Grid::Grid(const Architect& architect_):
    drawDistance { 5 },
    refreshDistance { 1 },
    architect { architect_ },
    centerPos(0),
    chunkLoader { 20, architect } {
    chunkLoader.Start();
    LoadNewChunks();
}

void Grid::GivePositionUpdate(Point3f worldPos) {
    Point3i gridPos(worldPos / CHUNK_SIZE / BLOCK_SIZE);
    GivePositionUpdate(gridPos);
}

void Grid::SetDrawDistance(int32_t drawDistance_) {
    drawDistance = drawDistance_;
    refreshDistance = std::max(drawDistance / 5, 1);
    UnloadOldChunks();
    LoadNewChunks();
    TRACE("Set draw distance to ", drawDistance);
}

uint32_t Grid::CountChunkTriangles() const {
    std::size_t triangles = 0;
    for (const auto& p: loadedChunks) {
        if (!p.second.IsEmpty()) {
            triangles += p.second.GetMesh().GetTriangleCount();
        }
    }
    return static_cast<uint32_t>(triangles);
}

void Grid::GivePositionUpdate(Point3i gridPos) {

    Point3i diff = centerPos - gridPos;
    for (uint8_t i = 0; i < 3; i++) {
        if (abs(diff[i]) >= refreshDistance) {
            centerPos = gridPos;
            INFO("New center chunk: ", centerPos, ", active chunks: ", loadedChunks.size(), ", triangles: ", CountChunkTriangles());
            UnloadOldChunks();
            LoadNewChunks();
            break;
        }
    }
}

void Grid::LoadNewChunks() {
    std::vector<Point3i> requestChunks;

    /*requestChunks.emplace_back(0, 0, 0);
    requestChunks.emplace_back(1, 0, 0);
    chunkLoader.RequestChunks(requestChunks);
    return;*/

    Point2i curr { centerPos[0], centerPos[1] };
    RequestChunksZ(Point2i { centerPos[0], centerPos[1] }, requestChunks);

    for (int32_t offset = 1; offset < drawDistance; offset++) {
        for (int32_t y = centerPos[1] - offset; y < centerPos[1] + offset; y++) {
            Point2i pos { centerPos[0] - offset, y };
            RequestChunksZ(pos, requestChunks);
        }
        for (int32_t x = centerPos[0] - offset; x < centerPos[0] + offset; x++) {
            Point2i pos { x, centerPos[1] + offset };
            RequestChunksZ(pos, requestChunks);
        }
        for (int32_t y = centerPos[1] + offset; y > centerPos[1] - offset; y--) {
            Point2i pos { centerPos[0] + offset, y };
            RequestChunksZ(pos, requestChunks);
        }

        for (int32_t x = centerPos[0] + offset; x > centerPos[0] - offset; x--) {
            Point2i pos { x, centerPos[1] - offset };
            RequestChunksZ(pos, requestChunks);
        }
    }

    DEBUG("Requesting ", requestChunks.size(), " chunks for loading");
    chunkLoader.RequestChunks(requestChunks);
}

void Grid::RequestChunksZ(const Point2i& chunkPos, std::vector<Point3i>& requestChunks) {
    auto pair = architect.GetMinMaxGlobalHeight(chunkPos);
    int32_t minZ = std::max(pair.first / CHUNK_SIZE, centerPos[2] - drawDistance);
    int32_t maxZ = std::min(pair.second / CHUNK_SIZE + 2, centerPos[2] + drawDistance);
    for (auto z = minZ; z < maxZ; z++) {
        Point3i p(chunkPos[0], chunkPos[1], z);
        if (p.GetDistance(centerPos) < drawDistance) {
            if (loadedChunks.find(p) == loadedChunks.end()) {
                requestChunks.push_back(p);
            }
        }
    }

}

void Grid::UnloadOldChunks() {
    uint32_t unloadCount = 0;

    auto iter = loadedChunks.begin();
    while (iter != loadedChunks.end()) {
        if (iter->first.GetDistance(centerPos) < drawDistance) {
            ++iter;
        } else {
            loadedChunks.erase(iter++);
            unloadCount++;
        }
    }

    INFO("Unloading ", unloadCount, " chunks");
}

void Grid::Update() {
    if (chunkLoader.HasFinishedChunks()) {
        //sf::Clock clock;
        std::vector<Chunk> newChunks = chunkLoader.GetFinishedChunks();
        //UpdateChunkBorders(newChunks);
        for (auto iter = std::make_move_iterator(newChunks.begin());
            iter != std::make_move_iterator(newChunks.end()); ++iter) {
            loadedChunks.emplace(iter->GetPosition(), *iter);
        }

        //DEBUG("Updating chunks took ", clock.getElapsedTime().asMilliseconds(), "ms");
    }
}

void Grid::UpdateChunkBorders(std::vector<Chunk>& newChunks) {
    for (auto& chunk: newChunks) {
        for (uint8_t i = 0; i < 6; i++) {
            Direction border = GetDirection(i);
            Point3i neighbourPos = chunk.GetPosition() + GetOffset(border);
            auto neighbourFind = loadedChunks.find(neighbourPos);
            if (neighbourFind != loadedChunks.end()) {
                Chunk& neighbour = neighbourFind->second;
                chunk.UpdateBorders(neighbour, border, architect.GetBlockManager());
            }
        }

    }
}

void Grid::Draw(const Camera& camera) const {
    for (auto chunkIter = loadedChunks.cbegin(); chunkIter != loadedChunks.end(); ++chunkIter) {
        chunkIter->second.Draw(camera);
    }
}

}   // namespace chunk
