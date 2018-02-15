/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

#include "utility/Point3.hpp"

namespace mc::world {

class Position: public Point3f {
 public:
                        Position(float x, float y, float z);
                        Position(const Position& other);

    glm::vec3           GetVec() const;
    glm::mat4           CreateMatrix() const;
    void                Move(const Position& offset);
};


}   // namespace mc::world
