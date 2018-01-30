/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"


namespace mc {

Camera::Camera():
    position { 4.0f, 4.0f, 4.0f },
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

void Camera::Rotate(float horizontal, float vertical) {

    glm::vec3 direction(
        cos(vertical) * sin(horizontal),
        sin(vertical),
        cos(vertical) * cos(horizontal)
    );

    glm::vec3 right(
        sin(horizontal - 3.14f / 2.0f),
        0,
        cos(horizontal - 3.14f / 2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    auto test = position + direction;
    INFO(test[0], "/", test[1], "/", test[2]);

    view = glm::lookAt(
        position,
        position + direction,
        up
    );
}

}   // namespace mc
