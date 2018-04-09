/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc::world {

World::World():
    atlas { "data/atlas.bmp", Point2f(32.0f) },
    camera { Point3f(0.0f, 0.0f, 400.0f), Point3f(0.0f, 0.0f, 0.0f) },
    architect { },
    grid { 40, architect, atlas } {
    INFO("Creating world");
    atlas.MakeActive();
    grid.GivePositionUpdate(camera.GetPosition());
}

World::~World() {
    INFO("Destroying world");
}

void World::Tick() {
    grid.GivePositionUpdate(camera.GetPosition());
    grid.Update();
}

void World::Draw() const {
    grid.Draw(camera);
}

}   // namespace mc::world
