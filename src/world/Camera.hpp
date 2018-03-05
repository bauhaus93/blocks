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
#include "Entity.hpp"

namespace mc::world {

class Camera: public Entity {

 public:
                    Camera(const Point3f& position_, const Point3f& rotation_);

    void            LoadMVPMatrix(const glm::mat4& model) const;
    void            Move(const Point3f& offset) override;
    void            Rotate(const Point3f& offset) override;

 private:

    void            UpdateView();

     ShaderProgram  shader;
     glm::mat4      view;
     glm::mat4      projection;
};

}   // namespace mc::world
