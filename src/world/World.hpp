/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"

#include "Camera.hpp"
#include "Cube.hpp"


namespace mc {

class World {

 public:

                World();

    Camera&     GetCamera() { return camera; }
    void        Draw() const;

 private:

     Camera     camera;
     Mesh       mesh;
     Texture    texture;
     Cube       cube;



};

}   // namespace mc
