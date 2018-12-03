/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cmath>

#include "glad/glad.h"

#include "glm/trigonometric.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "logger/GlobalLogger.hpp"
#include "graphics/ShaderProgram.hpp"

#include "utility/Point3.hpp"
#include "utility/PointTransformation.hpp"
#include "Entity.hpp"

namespace blocks {

class Camera: public Entity {

 public:
    explicit        Camera(ShaderProgram& shader_);

    void            SetFOV(float fovDegree);
    void            SetAspectRatio(float ratio);
    void            SetNear(float near_);
    void            SetFar(float far_);
    float           GetNear() const { return near; }

    void            ModFOV(float degree);

    void            LoadMVPMatrix(const glm::mat4& model) const;
    void            SetPosition(const Point3f& newPosition) override;
    void            SetRotation(const Point3f& newRotation) override;
    void            Move(const Point3f& offset) override;
    void            Rotate(const Point3f& offset) override;

 private:

    void                        UpdateView();
    void                        UpdateProjection();

    float                       fov;
    float                       aspectRatio;
    float                       near;
    float                       far;

    ShaderProgram&              shader;
    glm::mat4                   view;
    glm::mat4                   projection;
};

}   // namespace blocks
