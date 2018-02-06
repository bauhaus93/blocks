/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"

namespace mc {

ShaderProgram LoadShader();

Camera::Camera(glm::vec3 position_, glm::vec2 rotation_):
    Entity(position_, rotation_),
    shader { LoadShader() },
    view { glm::lookAt(
                    position_,
                    glm::vec3 { 0, 0, 0 },
                    glm::vec3 { 0, 1, 0 }) },
    projection { glm::perspective(
                            glm::radians(45.0f),
                            4.0f / 3.0f,
                            0.1f,
                            100.0f) } {
}

void Camera::LoadMVPMatrix(const glm::mat4& model) const {
    const glm::mat4 mvp = projection * view * model;
    shader.SetMVPMatrix(mvp);
}

//offset[0] -> horizontal
//offset[1] -> vertical
void Camera::Rotate(glm::vec2 offset) {
    Entity::Rotate(offset);

    direction = glm::vec3(
        cos(offset[1]) * sin(offset[0]),
        sin(offset[1]),
        cos(offset[1]) * cos(offset[0])
    );

    right = glm::vec3(
        sin(offset[0] - 3.14f / 2.0f),
        0,
        cos(offset[0] - 3.14f / 2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    view = glm::lookAt(
        position,
        position + direction,
        up
    );
}

ShaderProgram LoadShader() {
    ShaderProgram program;
    program.AddVertexShader("shader/VertexShader.glsl");
    program.AddFragmentShader("shader/FragmentShader.glsl");
    program.Link();
    program.Use();
    return program;
}

}   // namespace mc
