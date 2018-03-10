/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "BoundingBox.hpp"

namespace mc::world {

BoundingBox::BoundingBox(Point3f center_, Point3f extent_):
    center { CreateVec(center_) },
    extent { CreateVec(extent_) } {
}


}   // namedspace mc::world