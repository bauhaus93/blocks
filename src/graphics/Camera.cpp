/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"


namespace mc {

Camera::Camera():
    horizontalAngle { 0.0f },   //TODO: align this with actual view
    verticalAngle { 0.0f },     // or remove lookat and make by angles
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

    horizontalAngle += horizontal;
    verticalAngle += vertical;

    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    glm::vec3 right(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    view = glm::lookAt(
        position,
        position + direction,
        up
    );
}

}   // namespace mc
