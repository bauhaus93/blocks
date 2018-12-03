/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>
#include <cstdint>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "world/BlockType.hpp"
#include "world/Direction.hpp"

namespace blocks {

class Face {
 public:
                    Face(BlockType type_, Direction dir_, Point2i8 origin_, int8_t size_);
    BlockType       GetType() const { return type; }
    Direction       GetDirection() const { return dir; }
    const Point2i8& GetOrigin() const { return origin; }
    int8_t          GetSize() const { return size; }
 private:
    BlockType   type;
    Direction   dir;
    Point2i8    origin;
    int8_t      size;
};

std::ostream& operator<<(std::ostream& os, const Face& face);

}   // namespace blocks
