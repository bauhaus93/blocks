/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <cassert>

#include "logger/GlobalLogger.hpp"
#include "utility/Point3.hpp"
#include "world/BlockType.hpp"
#include "world/Direction.hpp"
#include "world/ProtoBlock.hpp"
#include "mesh/Quad.hpp"
#include "mesh/Mesh.hpp"
#include "Facetree.hpp"

namespace mc::world::chunk {

class Blocktree;

typedef std::array<std::unique_ptr<Blocktree>, 8> BlocktreeArray;
typedef std::pair<Point3i8, BlockType> BlockElement;

class Blocktree {
 public:
                Blocktree(Point3i8 origin_, int8_t size_);
                Blocktree(Blocktree&& other) = default;
    Blocktree&  operator=(Blocktree&& rhs) = default;
    void        InsertBlocks(std::vector<BlockElement> blocks);

    mesh::Mesh  CreateMesh(const std::map<BlockType, ProtoBlock>& protoblocks) const;
    bool        HasChild(uint8_t octant) const;
    const Blocktree& GetChild(uint8_t octant) const;
    uint32_t    MaxDepth() const;
    BlockType   GetBlockType() const { return type; }
    BlockType   GetBlockType(Point3i8 pos) const;
    bool        IsLeaf() const;
    bool        IsEmpty() const;

 private:
    std::array<std::vector<BlockElement>, 8> SplitToChildren(const std::vector<BlockElement>& blocks);
    BlockType       IsMergeable() const;
    void            CreateChild(uint8_t octant);
    void            ClearChildren();
    void            CollectFaces(std::vector<Face>& faces, uint8_t layer, Direction dir) const;
    uint8_t         GetOctant(Point3i8 pos) const;

    Point3i8        origin;
    int8_t          size;
    BlockType       type;
    BlocktreeArray  children;
};


}   // namespace mc::world::chunk