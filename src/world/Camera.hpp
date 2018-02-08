/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glad/glad.h>

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/ShaderProgram.hpp"

#include "Entity.hpp"
#include "Position.hpp"
#include "Rotation.hpp"

namespace mc {

class Camera: public Entity {

 public:
                    Camera(const Position& position_, const Rotation& rotation_);

    void            LoadMVPMatrix(const glm::mat4& model) const;
    void            Rotate(const Rotation& offset) override;

 private:
     ShaderProgram  shader;
     glm::mat4      view;
     glm::mat4      projection;

     glm::vec3      direction;
     glm::vec3      right;

};

}   // namespace mc
