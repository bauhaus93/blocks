/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "graphics/TextureAtlas.hpp"
#include "chunk/Grid.hpp"

#include "Camera.hpp"
#include "Architect.hpp"

namespace mc::world {

class World {
    static const Point2f    ATLAS_FIELD_SIZE;
 public:

                World();
                ~World();
    Camera&     GetCamera() { return camera; }

    void        Tick();
    void        Draw() const;

 private:
    const graphics::TextureAtlas    atlas;
    Camera                          camera;
    Architect                       architect;
    chunk::Grid                     grid;
};

}   // namespace mc::world::world
