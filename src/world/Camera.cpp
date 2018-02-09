/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"

namespace mc {

ShaderProgram LoadShader();

Camera::Camera(const Position& position_, const Rotation& rotation_):
    Entity(position_, rotation_),
    shader { LoadShader() },
    view { glm::lookAt(
                    position_.GetVec(),
                    glm::vec3 { 0, 0, 0 },
                    glm::vec3 { 0, 0, 1 }) },
    projection { glm::perspective(
                            glm::radians(45.0f),
                            4.0f / 3.0f,
                            0.1f,
                            100.0f) } {
}

void Camera::LoadMVPMatrix(const glm::mat4& model) const {
    glm::mat4 mvp = projection * view * model;
    shader.SetMVPMatrix(mvp);
}


void Camera::UpdateView() {
    auto direction = rotation.CreateDirection();

    TRACE("direction: ", direction[0], "/", direction[1], "/", direction[2]);
    view = glm::lookAt(
        position.GetVec(),
        position.GetVec() + direction,
        glm::vec3(0, 0, 1)      //we want z to be up
    );
}

void Camera::Move(const Position& offset) {
    Entity::Move(offset);
    UpdateView();
}

void Camera::Rotate(const Rotation& offset) {
    Entity::Rotate(offset);

    rotation.EnforceBoundary(
        Rotation(0.0f, 0.0f, 0.0f),
        Rotation(2.0f * M_PI, M_PI, 0.0f));

    UpdateView();
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
