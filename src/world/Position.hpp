/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

#include "utility/Point3.hpp"

namespace mc {

class Position: public Point3<float> {
 public:
                        Position(float x, float y, float z);

    glm::vec3           GetVec() const;
    glm::mat4           CreateMatrix() const;
    void                Move(const Position& offset);
};


}   // namespace mc
