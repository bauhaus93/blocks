/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

World::World(Point2i chunkSize, Point3f blockSize):
    camera { Position(4.0, 4.0, 0), Rotation(0, 0, 0) },
    block { "cube.obj" },
    grid { 1, chunkSize, blockSize } {
    INFO("Creating world");
    grid.SetCenter(camera.GetPosition());

    Octree<int32_t> test(Point3i(0, 0, 0),
                         Point3i(64, 64, 64));
    test.QueueElement(Point3i(30, 50, 60));
    test.QueueElement(Point3i(10, 10, 33));
    test.QueueElement(Point3i(20, 30, 40));
    test.QueueElement(Point3i(30, 10, 62));
    test.QueueElement(Point3i(10, 10, 10));
    test.QueueElement(Point3i(1, 1, 1));
    test.QueueElement(Point3i(0, 0, 0));
    test.InsertQueuedElements();
    INFO(test);
}

World::~World() {
    INFO("Destroying world");
}

void World::Tick() {
    grid.SetCenter(camera.GetPosition());
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
