/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"

#include "Camera.hpp"
#include "Position.hpp"
#include "Rotation.hpp"
#include "Chunk.hpp"

namespace mc {

class World {

 public:

                World();

    Camera&     GetCamera() { return camera; }

    void        Tick();
    void        Draw() const;

 private:

    Camera      camera;
    Mesh        mesh;
    Texture     texture;
    Chunk       chunk;



};

}   // namespace mc
