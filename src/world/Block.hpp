/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <glm/vec3.hpp>

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "utility/Point3.hpp"
#include "logger/GlobalLogger.hpp"

#include "Entity.hpp"
#include "Camera.hpp"

namespace mc::world {

class Block: public Entity {

 public:
    static constexpr float   SIZE = 2.0f;

                Block(const Point3f& position_,
                      const Texture& texture_,
                      uint8_t neighbourCount_);
                Block(const Block& other);
    void        Draw(const Camera& camera, const Mesh& mesh) const;
    //bool        IsHidden() const { return neighbourCount == 6; }
    void        IncreaseNeighbourCount(uint8_t amount);
    void        DecreaseNeighbourCount(uint8_t amount);
    uint8_t     GetNeighbourCount() const { return neighbourCount; }

 private:
     const Texture&   texture;
     uint8_t          neighbourCount;

};



}   // namespace mc::world
