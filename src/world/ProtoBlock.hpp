/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <cassert>
#include <map>

#include "BlockType.hpp"
#include "Direction.hpp"

namespace mc::world {

class ProtoBlock {
 public:
    explicit            ProtoBlock(BlockType type_);

    BlockType           GetType() const { return type; }
    void                AddFace(Direction dir, uint32_t atlasIndex);
    uint32_t            GetFace(Direction face) const;

 private:
    const BlockType                     type;
    std::map<Direction, uint32_t>       atlasLayer;
};



}   // namespace mc::world
