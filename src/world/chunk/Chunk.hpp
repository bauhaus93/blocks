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
#include "mesh/Mesh.hpp"
#include "mesh/Quad.hpp"
#include "Blocktree.hpp"
#include "Facetree.hpp"
#include "NeighbourMask.hpp"
#include "LayerFaces.hpp"

namespace blocks{

class Chunk {

 public:
    explicit                Chunk(const Point3i& chunkPos_);
                            Chunk(Chunk&& other) = default;
    Chunk&                  operator=(Chunk&& other) = default;

    void                    InsertBlocks(const std::vector<BlockElement>& blocks);

    void                    CreateMesh(const BlockManager& blockManager);
    void                    UpdateBorders(Chunk& neighbour,
                                          Direction border,
                                          const BlockManager& blockManager);

    const Point3i&          GetPosition() const { return chunkPos; }
    bool                    IsEmpty() const { return blocktree.IsEmpty(); }
    void                    Draw(const Camera& camera) const;
    const Blocktree&        GetBlocktree() const { return blocktree; };
    const Mesh&       GetMesh() const { assert(mesh != nullptr); return *mesh; }
    bool                    IsBorderChecked(Direction border) const { return checkedBorders.Contains(border); }

 private:

    Point3i                     chunkPos;
    Point3f                     origin;
    glm::mat4                   model;
    NeighbourMask               checkedBorders;

    Blocktree                   blocktree;
    std::unique_ptr<Mesh> mesh;
};

}       // namespace chunk
