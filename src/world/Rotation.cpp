/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Rotation.hpp"

namespace mc {

Rotation::Rotation(float x, float y, float z):
    x { x_ },
    y { y_ },
    z { z_ } {
}

void Rotation::UpdateMatrix() {
    constexpr vec3 rotateY { 0, 1, 0 };
    constexpr vec3 rotateX { 1, 0, 0 };
    matrix = glm::rotate(
}



}   // namespace mc
