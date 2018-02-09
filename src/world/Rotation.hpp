/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cmath>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

namespace mc {

class Rotation {
 public:
                        Rotation(float x, float y, float z);
    const glm::vec3&    GetVec() const;

    glm::vec3           CreateDirection() const;
    glm::mat4           CreateMatrix() const;

    void        Rotate(const Rotation& offset);
    void        EnforceBoundary(const Rotation& min, const Rotation& max);
 private:
    glm::vec3   angle;
};


}   // namespace mc
