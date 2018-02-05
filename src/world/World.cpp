/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "World.hpp"

namespace mc {

class World {

World::World():
    player { glm::vec3(4.0, 4.0, 4.0), glm::vec2(0.0, 0.0) },
    camera { player } {

}


}   // namespace mc
