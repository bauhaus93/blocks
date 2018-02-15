/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

#include "utility/Point3.hpp"

namespace mc::world {

class Rotation: public Point3f {
 public:
                        Rotation(float x, float y, float z);
                        Rotation(const Rotation& other);
    glm::vec3           GetVec() const;

    glm::vec3           CreateDirection() const;
    glm::mat4           CreateMatrix() const;

    void        Rotate(const Rotation& offset);
    void        Rotate(const Rotation& offset,
                       const Rotation& min,
                       const Rotation& max);
};


}   // namespace mc::world
