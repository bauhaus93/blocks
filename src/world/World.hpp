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
#include "utility/Image.hpp"
#include "graphics/TextureAtlas.hpp"
#include "chunk/Grid.hpp"
#include "architect/Architect.hpp"

#include "Camera.hpp"
#include "ProtoBlock.hpp"

namespace mc::world {

extern const Point2u TEXTURE_SIZE;
extern const uint32_t ATLAS_DEPTH;

class World {
 public:

                World();
                ~World();
    void        SetDrawDistance(int32_t chunkDrawDistance);

    Camera&     GetCamera() { return camera; }
    int32_t     GetChunkDrawDistance() const { return grid.GetDrawDistance(); }

    void        Tick();
    void        Draw() const;

 private:
    graphics::TextureAtlas          atlas;
    std::map<BlockType, ProtoBlock> protoblocks;
    Camera                          camera;
    architect::Architect            architect;
    chunk::Grid                     grid;

    void                            LoadProtoBlocks();
};

}   // namespace mc::world::world
