/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "logger/GlobalLogger.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"

#include "Camera.hpp"
#include "Position.hpp"
#include "Rotation.hpp"
#include "Chunk.hpp"

namespace mc {

typedef std::map<Point2<int32_t>, Chunk>    ChunkMap;

class World {

 public:

                World(Point2<int32_t> chunkSize_, Point3<float> blockSize_, int32_t activeChunkDistance_);
                ~World();
    Camera&     GetCamera() { return camera; }

    void        Tick();
    void        Draw() const;

 private:

    Point2<int32_t> GetChunkPos(const Entity& entity);

    Camera      	camera;
    SimplexNoise	noise;
    Mesh        	mesh;
    Texture     	texture;
    Point2<int32_t> chunkSize;
    Point3<float>   blockSize;
    Point2<int32_t> centerChunkPos;
    int32_t         activeChunkDistance;
    ChunkMap        activeChunks;
};

}   // namespace mc
