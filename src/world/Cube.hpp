/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"

#include "Entity.hpp"
#include "Camera.hpp"

namespace mc {

class Cube: public Entity {

 public:

                Cube(glm::vec3 position_,
                     const Mesh& mesh_,
                     const Texture& texture_);
    void        Draw(const Camera& camera) const;

 private:

     const Mesh&      mesh;
     const Texture&   texture;

};



}   // namespace mc
