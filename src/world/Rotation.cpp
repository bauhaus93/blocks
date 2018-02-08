/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Rotation.hpp"

namespace mc {

Rotation::Rotation(float x, float y, float z):
    quaternion { glm::quat(glm::vec3(
                 glm::degrees(x),
                 glm::degrees(y),
                 glm::degrees(z))) } {
}

glm::mat4 Rotation::CreateMatrix() const {
    return glm::toMat4(quaternion);
}

void Rotation::Rotate(const Rotation& offset) {
    quaternion = offset.quaternion * quaternion;
}


}   // namespace mc
