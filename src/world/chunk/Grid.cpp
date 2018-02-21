/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Grid.hpp"

namespace mc::world::chunk {


Element* LinkGrid(ElementMap& grid,
                  const Point2i& min,
                  const Point2i& max) {
    for (auto y = min[1]; y <= max[1]; y++) {
        for (auto x = min[0]; x <= max[0]; x++) {
            Point2i pos(x, y);
            if (x > min[0]) {
                Point2i western(x - 1, y);
                grid.at(pos)->Link(grid.at(western), Direction::WEST);
            }
            if (y > min[1]) {
                Point2i northern(x , y - 1);
                grid.at(pos)->Link(grid.at(northern), Direction::NORTH);
            }
        }
    }
    Element* center = grid.at((min + max) / 2);
    return center;
}

Grid::Grid(int32_t gridSize,
           Point2i chunkSize_,
           Point3f blockSize_):
    size { gridSize },
    chunkSize { chunkSize_ },
    blockSize { blockSize_ },
    heightNoise { },
    center { nullptr },
    texture { "grass.bmp" } {
    if (size % 2 != 0) {
        throw ApplicationError("Chunk grid size error",
                                __FUNCTION__,
                                "Grid size must be divisible by 2");
    }
}

Element* Grid::GenerateElement(Point2i pos) {
    Chunk chunk { pos, chunkSize, blockSize };
    chunk.Generate(heightNoise, texture);
    return new Element(std::move(chunk));
}

void Grid::SetCenter(Point3f worldPos) {
    Point2i gridPos(static_cast<int32_t>(worldPos[0] / chunkSize[0] / blockSize[0]),
                    static_cast<int32_t>(worldPos[1] / chunkSize[1] / blockSize[1]));
    SetCenter(gridPos);
}

void Grid::SetCenter(Point2i gridPos) {
    if (center == nullptr) {
        ElementMap tmpGrid;
        Point2i sizeHalf(size / 2, size / 2);
        Point2i min (gridPos - sizeHalf);
        Point2i max (gridPos + sizeHalf);

        for (auto y = min[1]; y <= max[1]; y++) {
            for (auto x = min[0]; x <= max[0]; x++) {
                Point2i currPos(x, y);
                tmpGrid.insert(std::make_pair(currPos, GenerateElement(currPos)));
            }
        }
        center = LinkGrid(tmpGrid, min, max);
        assert(center->GetChunk().GetPosition() == gridPos);
    }
}

void Grid::DrawBlocks(const Camera& camera, const Mesh& mesh) const {

    for (auto currY = center; currY != nullptr; currY = currY->GetNeighbour(Direction::NORTH)) {
        currY->GetChunk().DrawBlocks(camera, mesh);
        for (auto currX = currY->GetNeighbour(Direction::EAST); currX != nullptr; currX = currX->GetNeighbour(Direction::EAST)) {
            currX->GetChunk().DrawBlocks(camera, mesh);
        }
        for (auto currX = currY->GetNeighbour(Direction::WEST); currX != nullptr; currX = currX->GetNeighbour(Direction::WEST)) {
            currX->GetChunk().DrawBlocks(camera, mesh);
        }
    }
    for (auto currY = center->GetNeighbour(Direction::SOUTH); currY != nullptr; currY = currY->GetNeighbour(Direction::SOUTH)) {
        currY->GetChunk().DrawBlocks(camera, mesh);
        for (auto currX = currY->GetNeighbour(Direction::EAST); currX != nullptr; currX = currX->GetNeighbour(Direction::EAST)) {
            currX->GetChunk().DrawBlocks(camera, mesh);
        }
        for (auto currX = currY->GetNeighbour(Direction::WEST); currX != nullptr; currX = currX->GetNeighbour(Direction::WEST)) {
            currX->GetChunk().DrawBlocks(camera, mesh);
        }
    }
}


}   // namespace mc::world::chunk
