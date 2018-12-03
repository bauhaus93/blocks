/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "NeighbourMask.hpp"

namespace blocks{

NeighbourMask::NeighbourMask():
    mask { 0 } {
}

void NeighbourMask::Add(Direction dir) {
    mask |= GetValue(dir);
    assert(Contains(dir));
    assert(mask <= 0x3F);     // 0x3F = 0011 1111
}

void NeighbourMask::Remove(Direction dir) {
    if (Contains(dir)) {
        mask ^= GetValue(dir);
    }
    assert(!Contains(dir));
    assert(mask <= 0x3F);
}

bool NeighbourMask::Contains(Direction dir) const {
    return (mask & GetValue(dir)) != 0;
}

std::ostream& operator<<(std::ostream& os, const NeighbourMask& mask) {
    os << "(";
    for (int8_t i = 5; i >= 0; i--) {
        if (mask.Contains(GetDirection(i))) {
            os << "1";
        } else {
            os << "0";
        }
    }
    os << ")";
    return os;
}

}       // namespace chunk
