/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace mc {

class Rotation {
 public:
                Rotation(float x, float y, float z);
    glm::mat4   CreateMatrix() const;

    void        Rotate(const Rotation& offset);
 private:
    glm::quat   quaternion;
};


}   // namespace mc
