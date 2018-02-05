/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Object.hpp"
#include "Camera.hpp"

namespace mc {

class Player: public Object {

 public:

                    Player(glm::vec3 position_, glm::vec2 rotation_);

     void           AlignCameraToPlayer(Camera& camera);

 private:


};

}   // namespace mc
