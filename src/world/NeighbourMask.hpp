/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>
#include <cstdint>
#include <cassert>

#include "Direction.hpp"

namespace mc::world {

class NeighbourMask {
 public:

            NeighbourMask();
    void    Add(Direction dir);
    void    Remove(Direction dir);
    bool    Contains(Direction dir) const;
    bool    IsFull() const { return mask == 0x3F; }
    bool    IsEmpty() const { return mask == 0; }

 private:
    uint8_t mask;
};

std::ostream& operator<<(std::ostream& os, const NeighbourMask& mask);


}       // namespace mc::world
