/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>
#include <cstdint>
#include <cassert>

#include "Direction.hpp"

namespace mc::world {

class ProtoBlock {
 public:
                        ProtoBlock();

    void                SetFace(Direction dir, uint32_t atlasIndex);
    uint32_t            GetFace(Direction face) const;

 private:
    std::array<uint32_t, 6>     atlasIndices;
};

}   // namespace mc::world
