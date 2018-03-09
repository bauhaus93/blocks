/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace mc::world {



class Frustum {
 public:
                    Frustum(const glm::mat4& view, const glm::mat4& projection);

 private:
    std::array<glm::vec4, 6>    plane;

};


}   // namespace mc::world
