/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "chunk/Grid.hpp"

#include "Camera.hpp"
#include "octree/Octree.hpp"

namespace mc::world {

class World {

 public:

                World(Point3i chunkSize, Point3f blockSize);
                ~World();
    Camera&     GetCamera() { return camera; }

    void        Tick();
    void        Draw() const;

 private:
    void        DrawBlocks() const;

    Camera      camera;
    Mesh        block;
    chunk::Grid grid;
};

}   // namespace mc::world::world
