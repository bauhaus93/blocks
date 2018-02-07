/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"

namespace mc {

ShaderProgram LoadShader();

Camera::Camera(const Position& position_, const Rotation& rotation_):
    Entity(position_, rotation_),
    shader { LoadShader() },
    view { glm::lookAt(
                    position_.CreateGLMVec(),
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

void Camera::Rotate(const Rotation& offset) {
    Entity::Rotate(offset);

    direction = glm::vec3(
        cos(rotation.GetX()) * sin(rotation.GetY()),
        sin(rotation.GetX()),
        cos(rotation.GetX()) * sin(rotation.GetY())
    );

    right = glm::vec3(
        sin(rotation.GetY() - 3.14f / 2.0f),
        0,
        cos(rotation.GetY() - 3.14f / 2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    glm::vec3 posVec = position.CreateGLMVec();

    view = glm::lookAt(
        posVec,
        posVec + direction,
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
