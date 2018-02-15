/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <string>

namespace mc::world::chunk {

enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

uint32_t GetIndex(Direction dir);
Direction GetDirection(uint32_t index);
Direction GetOpposite(Direction dir);
std::string GetString(Direction dir);

}   // namespace mc::world::chunk
