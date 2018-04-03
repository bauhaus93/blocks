/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <cstdint>
#include <bitset>
#include <functional>

#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/PointTransformation.hpp"
#include "utility/Defs.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Mesh.hpp"
#include "world/Block.hpp"
#include "world/Camera.hpp"
#include "world/NeighbourMask.hpp"
#include "world/Direction.hpp"

#include "CompositeMesh.hpp"
#include "SimplexNoise.hpp"

namespace mc::world::chunk {

class Chunk {

 public:
    static constexpr int32_t   SIZE = 16;

    explicit        Chunk(const Point3i& chunkPos_);
                    Chunk(const Chunk& other) = delete;
                    Chunk(Chunk&& other);
    Chunk&          operator=(Chunk&& other);

    void            Generate(const SimplexNoise& noise, const Texture& texture);
    const Point3i&  GetPosition() const { return chunkPos; }
    bool            IsEmpty() const;
    bool            IsFull() const;
    bool            BlockExists(const Point3i& blockPos) const;
    bool            AllNeighboursChecked() const { return checkedNeighbours.IsFull(); }
    bool            CheckedNeighbour(Direction dir) const { return checkedNeighbours.Contains(dir); }
    void            UpdateBlockVisibility(Direction dir, Chunk& neighbour);
    bool            operator<(const Chunk& rhs) const;
    bool            operator<(const Point3i& rhsChunkPos) const;
    void            Draw(const Camera& camera) const;

 private:
    void    GenerateColumn(Point3i top, const Texture& texture, std::array<int32_t, 4>& neighbourHeight);
    void    CreateNonBorderRenderCandidates();
    void    CreateMesh();

    Point3i         chunkPos;
    Point3f         origin;
    glm::mat4       model;
    NeighbourMask   checkedNeighbours;
    Map3D<Block>    blocks;
    std::unique_ptr<mesh::Mesh> mesh;
};

typedef std::array<int32_t, Chunk::SIZE * Chunk::SIZE> HeightArray;

}       // namespace mc::world::chunk
