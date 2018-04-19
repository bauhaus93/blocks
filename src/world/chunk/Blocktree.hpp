/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "logger/GlobalLogger.hpp"
#include "utility/Point3.hpp"
#include "world/BlockType.hpp"
#include "world/Direction.hpp"
#include "mesh/Quad.hpp"
#include "mesh/Mesh.hpp"

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
    BlockType   GetBlockType() const { return type; }
    mesh::Mesh  CreateMesh() const;


 private:
    void            AssignChildBlocks(int8_t index, std::vector<BlockElement> blocks);
    BlockType       IsMergeable() const;
    void            ClearChildren();
    void            CollectQuads(Direction dir, Blocktree& neighbour, std::vector<mesh::Quad>& quads);
    void            CollectQuads(Direction dir, std::vector<mesh::Quad>& quads);

    Point3i8        origin;
    int8_t          size;
    BlockType       type;
    BlocktreeArray  children;
};


}   // namespace mc::world::chunk