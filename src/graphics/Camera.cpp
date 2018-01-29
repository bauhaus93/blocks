/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"


namespace mc {

Camera::Camera():
    position { 4.0f, 3.0f, 3.0f },
    view { glm::lookAt(
                        position,
                        glm::vec3 { 0, 0, 0 },
                        glm::vec3 { 0, 1, 0 }) },
    projection { glm::perspective(
                            glm::radians(45.0f),
                            4.0f / 3.0f,
                            0.1f,
                            100.0f)} {
}

glm::mat4 Camera::CreateMVPMatrix(const glm::mat4& model) {
    return projection * view * model;
}

}   // namespace mc
