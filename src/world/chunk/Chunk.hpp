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
#include "world/Direction.hpp"
#include "world/architect/Architect.hpp"
#include "world/Size.hpp"
#include "Blocktree.hpp"

namespace mc::world::chunk {

class Chunk {

 public:
    explicit        Chunk(const Point3i& chunkPos_);
                    Chunk(Chunk&& other) = default;
    Chunk&          operator=(Chunk&& other) = default;

    void            Generate(const architect::Architect& architect);
    const Point3i&  GetPosition() const { return chunkPos; }
    void            Draw(const Camera& camera) const;

 private:
    void    GenerateColumn(Point3i top,
                           const std::array<int32_t, 4>& neighbourHeight,
                           const architect::Architect& architect);

    Point3i                     chunkPos;
    Point3f                     origin;
    glm::mat4                   model;
    std::unique_ptr<Blocktree>  blocktree;
    std::unique_ptr<mesh::Mesh> mesh;
};

typedef std::array<int32_t, CHUNK_SIZE * CHUNK_SIZE> HeightArray;



}       // namespace mc::world::chunk
