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
                    glm::vec3 { 0, 1, 0 }) },
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

void Camera::Rotate(const Rotation& offset) {
    Entity::Rotate(offset);

    //rotation.EnforceBoundary(
    //    Rotation(-M_PI / 2.0f, 0.0f, 0.0f),
    //    Rotation(M_PI / 2.0f, 2.0f * M_PI, 0.0f));

    auto rot = rotation.GetVec();

    glm::vec3 direction { cos(rot[1]) * sin(rot[0]),
                          sin(rot[1]),
                          cos(rot[1]) * cos(rot[0]) };

    glm::vec3 right { sin(rot[0] - M_PI / 2.0f),
                      0,
                      cos(rot[0] - M_PI/ 2.0f) };

    glm::vec3 up = glm::cross(right, direction);

    view = glm::lookAt(
        position.GetVec(),
        position.GetVec() + direction,
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
