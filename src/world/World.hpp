/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <map>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "utility/Bitmap.hpp"
#include "utility/Image.hpp"
#include "graphics/TextureAtlas.hpp"
#include "chunk/Grid.hpp"
#include "architect/Architect.hpp"

#include "Camera.hpp"
#include "Fog.hpp"
#include "BlockManager.hpp"

namespace blocks {

class World {
 public:

                World();
                ~World();
    void        SetDrawDistance(int32_t chunkDrawDistance);

    Camera&     GetCamera() { return camera; }
    Fog&        GetFog() { return fog; }
    int32_t     GetChunkDrawDistance() const { return grid.GetDrawDistance(); }

    void        Tick();
    void        Draw();

 private:
    ShaderProgram   shader;
    TextureAtlas    atlas;
    Camera          camera;
    Fog             fog;
    BlockManager    blockManager;
    Architect       architect;
    Grid            grid;

    void            LoadBlocks();
};

}   // namespace blocks
