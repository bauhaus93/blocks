/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "PointTransformation.hpp"

namespace blocks {


glm::mat4 CreateTranslationMatrix(const glm::vec3& point) {
    return glm::translate(glm::mat4(), point);
}


}   // namespace blocks