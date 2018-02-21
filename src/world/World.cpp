/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

World::World(Point2i chunkSize, Point3f blockSize):
    camera { Position(4.0, 4.0, 0), Rotation(0, 0, 0) },
    block { "cube.obj" },
    grid { 2, chunkSize, blockSize } {
    INFO("Creating world");
    grid.SetCenter(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::Tick() {

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
