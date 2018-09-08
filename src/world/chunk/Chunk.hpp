/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <cstdint>
#include <bitset>
#include <functional>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/PointTransformation.hpp"
#include "utility/Defs.hpp"
#include "graphics/TextureAtlas.hpp"
#include "world/Camera.hpp"
#include "world/Direction.hpp"
#include "world/BlockManager.hpp"
#include "world/Size.hpp"
#include "Blocktree.hpp"
#include "NeighbourMask.hpp"

namespace mc::world::chunk {

class Chunk {

 public:
                            Chunk(const Point3i& chunkPos_,
                                  Blocktree&& blocktree_,
                                  mesh::Mesh&& mesh_);
    explicit                Chunk(const Point3i& chunkPos_);
                            Chunk(Chunk&& other) = default;
    Chunk&                  operator=(Chunk&& other) = default;

    const Point3i&          GetPosition() const { return chunkPos; }
    bool                    IsEmpty() const { return blocktree.IsEmpty(); }
    void                    Draw(const Camera& camera) const;
    const Blocktree&        GetBlocktree() const { return blocktree; };
    const mesh::Mesh&       GetMesh() const { assert(mesh != nullptr); return *mesh; }

 private:

    Point3i                     chunkPos;
    Point3f                     origin;
    glm::mat4                   model;

    Blocktree                   blocktree;
    std::unique_ptr<mesh::Mesh> mesh;
};

}       // namespace mc::world::chunk
