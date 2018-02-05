/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Camera.hpp"
#include "Player.hpp"

namespace mc {

class World {

 public:

                World();

 private:

     Player     player;
     Camera     camera;



};

}   // namespace mc
