/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Position.hpp"

namespace mc {

Position::Position(float x, float y, float z):
    Point3(x, y, z) {
}

glm::vec3 Position::GetVec() const {
    return glm::vec3(GetX(), GetY(), GetZ());
}

glm::mat4 Position::CreateMatrix() const {
    return glm::translate(glm::mat4(), GetVec());
}

void Position::Move(const Position& offset) {
    (*this) += offset;
}

}   // namespace mc
