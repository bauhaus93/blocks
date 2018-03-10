/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "utility/Point3.hpp"
#include "utility/PointTransformation.hpp"
#include "Intersection.hpp"

namespace mc::world {

class BoundingBox {
 public:
                    BoundingBox(Point3f center_, Point3f extent_);
    
    const glm::vec3&  GetCenter() const { return center; }
    const glm::vec3&  GetExtent() const { return extent; }

 private:
    glm::vec3     center;
    glm::vec3     extent;

};

}   // namedspace mc::world