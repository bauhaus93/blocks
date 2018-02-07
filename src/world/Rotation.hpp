/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


#include "Point.hpp"

namespace mc {

class Rotation: public Point {
 public:
        Rotation(float x, float y, float z);

protected:
    void        UpdateMatrix();

};


}   // namespace mc
