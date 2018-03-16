/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

World::World():
    camera { Point3f(4.0f, 4.0f, 0.0f), Point3f(0.0f, 0.0f, 0.0f) },
    block { "data/cube.obj" },
    grid { 3 },
    dummyText { "data/grass.bmp" },
    dummyBlock { Point3f(0.0f), dummyText, 0} {
    INFO("Creating world");
    grid.SetCenter(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::Tick() {
    grid.SetCenter(camera.GetPosition());
    grid.UpdateChunks();
    auto frustum = camera.GetFrustum();
    BoundingBox bb(Point3f(1.0f), Point3f(1.0f));
    auto intersection = frustum.Intersects(bb);
    switch (intersection) {
        case Intersection::INSIDE: INFO("INSIDE"); break;
        case Intersection::OUTSIDE: INFO("OUTSIDE"); break;
        case Intersection::PARTIAL: INFO("PARTIAL"); break;
        default:    assert(0);
    }
}

void World::Draw() const {
    DrawBlocks();
}

void World::DrawBlocks() const {
    block.Load();
    dummyBlock.Draw(camera, block);
    //grid.DrawBlocks(camera, block);
    block.Unload();
}

}   // namespace mc::world
