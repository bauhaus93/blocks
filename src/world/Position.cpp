/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Position.hpp"

namespace mc {

Position::Position(float x, float y, float z):
    pos { x, y, z } {
}

const glm::vec3& Position::GetVec() const {
    return pos;
}

glm::mat4 Position::CreateMatrix() const {
    return glm::translate(glm::mat4(), pos);
}

void Position::Move(const Position& offset) {
    pos += offset.pos;
}

}   // namespace mc
