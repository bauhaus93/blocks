/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Camera.hpp"

namespace blocks {

Camera::Camera(ShaderProgram& shader_):
    Entity(),
    fov { 75.0f },
    aspectRatio { 4.0f / 3.0f },
    near { 0.5f },
    far { 200.0f },
    shader { shader_ },
    view { glm::lookAt(
            CreateVec(position),
            glm::vec3 { 0, 0, 0 },
            glm::vec3 { 0, 0, 1 })
         },
    projection { glm::perspective(
                    glm::radians(fov),
                    aspectRatio,
                    near,
                    far)
               } {
}

void Camera::SetFOV(float fovDegree) {
    fov = fovDegree;
    UpdateProjection();
}
void Camera::SetAspectRatio(float ratio) {
    aspectRatio = ratio;
    UpdateProjection();
}
void Camera::SetNear(float near_) {
    near = near_;
    UpdateProjection();
}
void Camera::SetFar(float far_) {
    far = far_;
    UpdateProjection();
}

void Camera::ModFOV(float degree) {
    fov = std::min(179.0f, std::max(1.0f, fov + degree));
    UpdateProjection();
}

void Camera::LoadMVPMatrix(const glm::mat4& model) const {
    glm::mat4 mvp = projection * view * model;
    shader.SetMVPMatrix(mvp);
}

void Camera::UpdateView() {
    auto direction = CreateDirection(rotation);

    glm::vec3 posVec = CreateVec(position);
    view = glm::lookAt(
        posVec,
        posVec + direction,
        glm::vec3(0, 0, 1)      //we want z to be up
    );
}

void Camera::UpdateProjection() {
    projection = glm::perspective(
        glm::radians(fov),
        aspectRatio,
        near,
        far
    );
    INFO("Updated projection: fov = ", fov, ", aspect = ", aspectRatio, ", near = ", near, ", far = ", far);
}

void Camera::SetPosition(const Point3f& newPosition) {
    Entity::SetPosition(newPosition);
    UpdateView();
}

void Camera::SetRotation(const Point3f& newRotation) {
    constexpr float DOUBLE_PI = 2 * M_PI;
    constexpr float MIN_Y = 0.001f;
    constexpr float MAX_Y = M_PI - MIN_Y;

    rotation = newRotation;

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



}   // namespace blocks
