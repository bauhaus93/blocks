/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Position.hpp"

namespace mc::world {

Position::Position(float x, float y, float z):
    Point3f(x, y, z) {
}

Position::Position(const Position& other):
    Point3f(static_cast<const Point3f&>(other)) {
}

glm::vec3 Position::GetVec() const {
    return glm::vec3((*this)[0], (*this)[1], (*this)[2]);
}

glm::mat4 Position::CreateMatrix() const {
    return glm::translate(glm::mat4(), GetVec());
}

void Position::Move(const Position& offset) {
    (*this) += offset;
}

}   // namespace mc::world
