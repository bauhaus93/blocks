/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

World::World(Point3i chunkSize, Point3f blockSize):
    camera { Point3f(4.0f, 4.0f, 0.0f), Point3f(0.0f, 0.0f, 0.0f) },
    block { "cube.obj" },
    grid { 3, chunkSize, blockSize } {
    INFO("Creating world");
    grid.SetCenter(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::Tick() {
    grid.SetCenter(camera.GetPosition());
    grid.UpdateChunks();
}

void World::Draw() const {
    DrawBlocks();
}

void World::DrawBlocks() const {
    block.Load();
    grid.DrawBlocks(camera, block);
    block.Unload();
}

}   // namespace mc::world
