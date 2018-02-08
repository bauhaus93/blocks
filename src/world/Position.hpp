/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>


namespace mc {

class Position {
 public:
                        Position(float x, float y, float z);

    const glm::vec3&    GetVec() const;
    glm::mat4           CreateMatrix() const;
    void                Move(const Position& offset);
 private:
    glm::vec3    pos;
};


}   // namespace mc
