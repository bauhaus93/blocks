/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Rotation.hpp"

namespace mc {

Rotation::Rotation(float x_, float y_, float z_):
    Point(x_, y_, z_) {
}

void Rotation::UpdateMatrix() {
    glm::vec3 eulerAngles(glm::degrees(x),
                          glm::degrees(y),
                          glm::degrees(z));
    matrix = glm::toMat4(glm::quat(eulerAngles));
}



}   // namespace mc
