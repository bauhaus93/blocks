/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>
#include <cstdint>
#include <cassert>

#include "utility/Point3.hpp"

namespace mc::world {

//TODO change enum positions (N, E, U, S, W, D)
enum class Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    UP,
    DOWN
};

uint8_t GetIndex(Direction dir);
Direction GetDirection(uint8_t index);
uint8_t GetValue(Direction dir);
Direction GetOpposite(Direction dir);
Point3i GetOffset(Direction dir);

std::ostream& operator<<(std::ostream& os, Direction dir);

}       // namespace mc::world
