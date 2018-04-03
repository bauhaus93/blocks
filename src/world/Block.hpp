/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"

#include "Camera.hpp"
#include "NeighbourMask.hpp"
#include "Direction.hpp"

namespace mc::world {

class Block {

 public:
    static constexpr float   SIZE = 1.0f;

                Block();
                Block(const Block& other);
    void        AddNeighbour(Direction dir);
    void        RemoveNeighbour(Direction dir);
    bool        IsVisible() const { return !neighbours.IsFull(); }
    bool        HasNeighbour(Direction dir) const { return neighbours.Contains(dir); }
 private:
    NeighbourMask       neighbours;
};



}   // namespace mc::world
