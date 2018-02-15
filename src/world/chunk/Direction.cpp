/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Direction.hpp"

namespace mc::world::chunk {

uint32_t GetIndex(Direction dir) {
    switch (dir) {
    case Direction::NORTH:  return 0;
    case Direction::EAST:   return 1;
    case Direction::SOUTH:  return 2;
    case Direction::WEST:   return 3;
    default:                throw;
    }
}

Direction GetDirection(uint32_t index) {
    switch (index) {
    case 0: return Direction::NORTH;
    case 1: return Direction::EAST;
    case 2: return Direction::SOUTH;
    case 3: return Direction::WEST;
    default:                throw;
    }
}

Direction GetOpposite(Direction dir) {
    switch (dir) {
    case Direction::NORTH:  return Direction::SOUTH;
    case Direction::EAST:   return Direction::WEST;
    case Direction::SOUTH:  return Direction::NORTH;
    case Direction::WEST:   return Direction::EAST;
    default:                throw;
    }
}

std::string GetString(Direction dir) {
    switch (dir) {
    case Direction::NORTH:  return "NORTH";
    case Direction::EAST:   return "EAST";
    case Direction::SOUTH:  return "SOUTH";
    case Direction::WEST:   return "WEST";
    default:                throw;
    }
}

}   // namespace mc::world::chunk
