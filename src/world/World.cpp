/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc {

World::World():
    camera { glm::vec3(4.0, 4.0, 4.0), glm::vec2(0.0, 0.0) },
    mesh { "cube.obj" },
    texture { "test.bmp" },
    cube { glm::vec3(0.0, 0.0, 0.0), mesh, texture } {
}

void World::Draw() const {
    cube.Draw(camera);
}


}   // namespace mc
