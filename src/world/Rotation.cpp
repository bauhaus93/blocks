/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Rotation.hpp"

namespace mc {

Rotation::Rotation(float x, float y, float z):
    angle { x, y, z } {
}

const glm::vec3& Rotation::GetVec() const {
    return angle;
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
    return glm::rotate(angle[2], glm::vec3(0, 0, 1)) *
           glm::rotate(angle[1], glm::vec3(0, 1, 0)) *
           glm::rotate(angle[0], glm::vec3(1, 0, 0)) *
           glm::mat4(1.0f);
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
