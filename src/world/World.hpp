/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Bitmap.hpp"
#include "graphics/TextureAtlas.hpp"
#include "chunk/Grid.hpp"

#include "Camera.hpp"
#include "Architect.hpp"
#include "ProtoBlock.hpp"

namespace mc::world {

extern const Point2u TEXTURE_SIZE;
extern const uint32_t ATLAS_DEPTH;

class World {
 public:

                World();
                ~World();

    void        LoadAtlas();
    
    Camera&     GetCamera() { return camera; }

    void        Tick();
    void        Draw() const;

 private:
    const graphics::TextureAtlas    atlas;
    std::map<BlockType, ProtoBlock> protoblocks;
    Camera                          camera;
    Architect                       architect;
    chunk::Grid                     grid;
};

}   // namespace mc::world::world
