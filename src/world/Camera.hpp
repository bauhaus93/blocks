/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cmath>

#include "graphics/glad/Glad.h"

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/ShaderProgram.hpp"

#include "utility/Point3.hpp"
#include "utility/PointTransformation.hpp"
#include "Frustum.hpp"
#include "Entity.hpp"

namespace mc::world {

class Camera: public Entity {

 public:
                    Camera(const Point3f& position_, const Point3f& rotation_);

    void            SetFOV(float fovDegree);
    void            SetAspectRatio(float ratio);
    void            SetNear(float near_);
    void            SetFar(float far_);

    void            ModFOV(float degree);

    void            LoadMVPMatrix(const glm::mat4& model) const;
    void            Move(const Point3f& offset) override;
    void            Rotate(const Point3f& offset) override;
    const Frustum&  GetFrustum() const { return frustum; }

 private:

    void            UpdateView();
    void            UpdateProjection();

    float           fov;
    float           aspectRatio;
    float           near;
    float           far;

    graphics::ShaderProgram  shader;
    glm::mat4      view;
    glm::mat4      projection;
    Frustum        frustum;
};

}   // namespace mc::world
