/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Rotation.hpp"

namespace mc::world {

Rotation::Rotation(float x, float y, float z):
     Point3(x, y, z) {
}

Rotation::Rotation(const Rotation& other):
    Point3(other) {
}

glm::vec3 Rotation::GetVec() const {
    return glm::vec3(GetX(), GetY(), GetZ());
}

glm::vec3 Rotation::CreateDirection() const {
    return glm::normalize(glm::vec3 {
        sin(GetY()) * cos(GetX()),
        sin(GetY()) * sin(GetX()),
        cos(GetY()) });
}

glm::mat4 Rotation::CreateMatrix() const {
    return glm::rotate(GetX(), glm::vec3(1, 0, 0)) *
           glm::rotate(GetY(), glm::vec3(0, 1, 0)) *
           glm::rotate(GetZ(), glm::vec3(0, 0, 1));
}

void Rotation::Rotate(const Rotation& offset) {
    (*this) += offset;
    for (int i = 0; i < 3; i++) {
        if ((*this)[i] > 2 * M_PI) {
            (*this)[i] -= 2 * M_PI;
        }
        else if ((*this)[i] < 0) {
            (*this)[i] += 2 * M_PI;
        }
    }
}

void Rotation::Rotate(const Rotation& offset,
                      const Rotation& min,
                      const Rotation& max) {
    (*this) += offset;
    for (int i = 0; i < 3; i++) {
        if ((*this)[i] < min[i]) {
            (*this)[i] = min[i];
        } else if ((*this)[i] > max[i]) {
            (*this)[i] = max[i];
        }

        if ((*this)[i] > 2 * M_PI) {
            (*this)[i] -= 2 * M_PI;
        } else if ((*this)[i] < 0) {
            (*this)[i] += 2 * M_PI;
        }
    }
    //INFO("angles: ", glm::degrees(angle[0]), "/", glm::degrees(angle[1]), "/", glm::degrees(angle[2]));
}


}   // namespace mc::world
