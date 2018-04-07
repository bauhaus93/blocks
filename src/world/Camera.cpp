/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"

namespace mc::world {

ShaderProgram LoadShader();

Camera::Camera(const Point3f& position_, const Point3f& rotation_):
    Entity(position_, rotation_),
    shader { LoadShader() },
    view { glm::lookAt(
                    CreateVec(position),
                    glm::vec3 { 0, 0, 0 },
                    glm::vec3 { 0, 0, 1 }) },
    projection { glm::perspective(
                            glm::radians(90.0f),
                            4.0f / 3.0f,
                            0.5f,
                            1000.0f) },
    frustum { view, projection } {
}

void Camera::LoadMVPMatrix(const glm::mat4& model) const {
    glm::mat4 mvp = projection * view * model;
    shader.SetMVPMatrix(mvp);
}

void Camera::UpdateView() {
    auto direction = CreateDirection(rotation);

    TRACE("camera direction: ", direction[0], "/", direction[1], "/", direction[2]);
    glm::vec3 posVec = CreateVec(position);
    view = glm::lookAt(
        posVec,
        posVec + direction,
        glm::vec3(0, 0, 1)      //we want z to be up
    );
    frustum.Update(view, projection);
}

void Camera::Move(const Point3f& offset) {
    Entity::Move(offset);
    UpdateView();
}

//TODO investigate why angle PI causes jumps in view
//for now, just don't set the view y angle to exact PI/0
void Camera::Rotate(const Point3f& offset) {
    constexpr float DOUBLE_PI = 2 * M_PI;
    constexpr float MIN_Y = 0.001f;
    constexpr float MAX_Y = M_PI - MIN_Y;

    rotation += offset;
    
    if (rotation[1] < MIN_Y) {
        rotation[1] = MIN_Y;
    } else if (rotation[1] > MAX_Y) {
        rotation[1] = MAX_Y;
    }

    for (uint8_t i = 0; i < 3; i++) {
        if (rotation[i] < 0) {
            rotation[i] += DOUBLE_PI;
        } else if (rotation[i] > DOUBLE_PI) {
            rotation[i] -= DOUBLE_PI;
        }
    }
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

}   // namespace mc::world
