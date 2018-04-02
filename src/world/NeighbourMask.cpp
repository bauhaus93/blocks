/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "NeighbourMask.hpp"

namespace mc::world {

NeighbourMask::NeighbourMask():
    mask { 0 } {
}

void NeighbourMask::Add(Direction dir) {
    mask |= GetValue(dir);
    assert(mask <= 0x3F);     // 0x3F = 0011 1111
}

void NeighbourMask::Remove(Direction dir) {
    mask ^= GetValue(dir);
    assert(mask <= 0x3F);
}

bool NeighbourMask::Contains(Direction dir) const {
    return mask & GetValue(dir);
}

}       // namespace mc::world
