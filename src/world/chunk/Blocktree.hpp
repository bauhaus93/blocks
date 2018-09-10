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
#include "world/Size.hpp"
#include "LayerFaces.hpp"

namespace mc::world::chunk {

class Blocktree;

typedef std::array<std::unique_ptr<Blocktree>, 8> BlocktreeArray;
typedef std::pair<Point3i8, BlockType> BlockElement;

class Blocktree {
 public:
                        Blocktree();
                        Blocktree(Point3i8 origin_, int8_t size_);
                        Blocktree(Blocktree&& other) = default;
    Blocktree&          operator=(Blocktree&& rhs) = default;
    void                InsertBlocks(const std::vector<BlockElement>& blocks);

    LayerFaces          CreateFaces() const;
    std::vector<Face>   CreateFaces(uint8_t layer, uint8_t axis) const;
    bool                HasChild(uint8_t octant) const;
    const Blocktree&    GetChild(uint8_t octant) const;
    uint32_t            MaxDepth() const;
    BlockType           GetBlockType() const { return type; }
    BlockType           GetBlockType(Point3i8 pos) const;
    bool                IsLeaf() const;
    bool                IsEmpty() const;

 private:
    std::array<std::vector<BlockElement>, 8> SplitToChildren(const std::vector<BlockElement>& blocks);
    BlockType       IsMergeable() const;
    void            CreateChild(uint8_t octant);
    void            ClearChildren();
    void            CollectFaces(std::map<uint8_t, std::vector<Face>>& layerMap, uint8_t axis) const;
    void            CollectFaces(std::vector<Face>& faces, uint8_t layer, uint8_t axis) const;
    uint8_t         GetOctant(Point3i8 pos) const;

    Point3i8        origin;
    int8_t          size;
    BlockType       type;
    BlocktreeArray  children;
};


}   // namespace mc::world::chunk
