/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Intersection.hpp"
#include "BoundingBox.hpp"



namespace mc::world {

class Frustum {
 public:
                    Frustum(const glm::mat4& view, const glm::mat4& projection);

    Intersection    Intersects(const BoundingBox& box) const;

 private:
    std::array<glm::vec4, 6>    plane;

};


}   // namespace mc::world
