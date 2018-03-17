/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "BoundingBox.hpp"

namespace mc::world {

BoundingBox::BoundingBox(Point3f min, Point3f max):
    center { CreateVec((min + max) / 2.0f) },
    extent { CreateVec((max - min) / 2.0f) } {
}


}   // namedspace mc::world