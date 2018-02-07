/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Position.hpp"

namespace mc {

Position::Position(float x_, float y_, float z_):
    Point(x_, y_, z_) {
}

void Position::UpdateMatrix() {
    matrix = glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

}   // namespace mc
