/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec4.hpp>

#include "Point.hpp"

namespace mc {

class Position: public Point {
 public:
                Position(float x, float y, float z);

 protected:

     void       UpdateMatrix();
};


}   // namespace mc
