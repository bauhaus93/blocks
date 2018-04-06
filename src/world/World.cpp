/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

World::World():
    camera { Point3f(4.0f, 4.0f, 0.0f), Point3f(0.0f, 0.0f, 0.0f) },
    architect { },
    grid { 10, architect } {
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
    grid.Draw(camera);
}

}   // namespace mc::world
