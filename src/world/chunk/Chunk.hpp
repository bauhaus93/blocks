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
#include "graphics/TextureAtlas.hpp"
#include "world/Camera.hpp"
#include "world/NeighbourMask.hpp"
#include "world/Direction.hpp"
#include "world/Architect.hpp"
#include "world/Size.hpp"
#include "Block.hpp"
#include "CompositeMesh.hpp"

namespace mc::world::chunk {

class Chunk {

 public:
    explicit        Chunk(const Point3i& chunkPos_);
                    Chunk(const Chunk& other) = delete;
                    Chunk(Chunk&& other);
    Chunk&          operator=(Chunk&& other);

    void            Generate(const Architect& architect, const graphics::TextureAtlas& atlas);
    const Point3i&  GetPosition() const { return chunkPos; }
    bool            IsEmpty() const;
    bool            IsFull() const;
    void            CreateMesh(const graphics::TextureAtlas& atlas);
    bool            operator<(const Chunk& rhs) const;
    bool            operator<(const Point3i& rhsChunkPos) const;
    void            Draw(const Camera& camera) const;

 private:
    void    GenerateColumn(Point3i top,
                           const std::array<int32_t, 4>& neighbourHeight,
                           const Architect& architect);
    void    CreateNonBorderRenderCandidates();

    Point3i                     chunkPos;
    Point3f                     origin;
    glm::mat4                   model;
    std::vector<Block>          blocks;
    std::unique_ptr<mesh::Mesh> mesh;
};

typedef std::array<int32_t, CHUNK_SIZE * CHUNK_SIZE> HeightArray;



}       // namespace mc::world::chunk
