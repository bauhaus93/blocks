/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "logger/GlobalLogger.hpp"

namespace mc {

class Camera {

 public:

                    Camera();

    void            Rotate(float horizontal, float vertical);
    glm::mat4       CreateMVPMatrix(const glm::mat4& model);


 private:
     float          horizontalAngle;
     float          verticalAngle;
     glm::vec3      position;
     glm::mat4      view;
     glm::mat4      projection;

};

}   // namespace mc
