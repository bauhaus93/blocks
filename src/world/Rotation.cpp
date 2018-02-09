/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Rotation.hpp"

#include "logger/GlobalLogger.hpp"

namespace mc {

Rotation::Rotation(float x, float y, float z):
    angle { x, y, z } {
}

const glm::vec3& Rotation::GetVec() const {
    return angle;
}

glm::vec3 Rotation::CreateDirection() const {
    return glm::normalize(glm::vec3 { 
        sin(angle[1]) * cos(angle[0]),
        sin(angle[1]) * sin(angle[0]),
        cos(angle[1]) } );
}

void Rotation::EnforceBoundary(const Rotation& min,
                               const Rotation& max) {
    for (int i = 0; i < 3; i++) {
        if (angle[i] < min.angle[i])
            angle[i] = min.angle[i];
        else if (angle[i] > max.angle[i]) { 
            angle[i] = max.angle[i];
        }
    }
}

glm::mat4 Rotation::CreateMatrix() const {
    return glm::rotate(angle[0], glm::vec3(1, 0, 0)) *
           glm::rotate(angle[1], glm::vec3(0, 1, 0)) *
           glm::rotate(angle[2], glm::vec3(0, 0, 1));
}

void Rotation::Rotate(const Rotation& offset) {
    angle += offset.angle;
    for (int i = 0; i < 3; i++) {
        if (angle[i] >= 2 * M_PI) {
            angle[i] -= 2 * M_PI;
        }
        else if (angle[i] < 0) {
            angle[i] += 2 * M_PI;
        }
    }
}


}   // namespace mc
