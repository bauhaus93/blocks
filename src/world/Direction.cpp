/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Direction.hpp"

namespace mc::world {

uint8_t GetIndex(Direction dir) {
    switch (dir) {
        case Direction::NORTH:  return 0;
        case Direction::EAST:   return 1;
        case Direction::SOUTH:  return 2;
        case Direction::WEST:   return 3;
        case Direction::UP:     return 4;
        case Direction::DOWN:   return 5;
        default: assert(0);
    }
    return 0;
}

Direction GetDirection(uint8_t index) {
    assert(index < 6);
    switch (index) {
        case 0: return Direction::NORTH;
        case 1: return Direction::EAST;
        case 2: return Direction::SOUTH;
        case 3: return Direction::WEST;
        case 4: return Direction::UP;
        case 5: return Direction::DOWN;
        default: assert(0);
    }
    return Direction::NORTH;
}

uint8_t GetValue(Direction dir) {
    return (1 << GetIndex(dir));
}

Direction GetOpposite(Direction dir) {
    switch (dir) {
        case Direction::NORTH:  return Direction::SOUTH;
        case Direction::EAST:   return Direction::WEST;
        case Direction::SOUTH:  return Direction::NORTH;
        case Direction::WEST:   return Direction::EAST;
        case Direction::UP:     return Direction::DOWN;
        case Direction::DOWN:   return Direction::UP;
        default: assert(0);
    }
    return Direction::NORTH;
}

Point3i GetOffset(Direction dir) {
    switch (dir) {
        case Direction::NORTH:  return Point3i(0, -1, 0);
        case Direction::EAST:   return Point3i(1, 0, 0);
        case Direction::SOUTH:  return Point3i(0, 1, 0);
        case Direction::WEST:   return Point3i(-1, 0, 0);
        case Direction::UP:     return Point3i(0, 0, 1);
        case Direction::DOWN:   return Point3i(0, 0, -1);
        default: assert(0);
    }
    return Point3i(0);
}

std::ostream& operator<<(std::ostream& os, Direction dir) {
    switch (dir) {
        case Direction::NORTH:  os << "North";  break;
        case Direction::EAST:   os << "East";   break;
        case Direction::SOUTH:  os << "South";  break;
        case Direction::WEST:   os << "West";   break;
        case Direction::UP:     os << "Up";     break;
        case Direction::DOWN:   os << "Down";   break;
        default: assert(0);
    }
    return os;
}

}       // namespace mc::world
