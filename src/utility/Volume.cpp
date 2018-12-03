/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "utility/Volume.hpp"

namespace blocks {

Volume::Volume(Point3f min_, Point3f max_):
    min { min_ },
    max { max_ } {

}

bool Volume::Contains(Point3f point) const {
    return point >= min && point <= max;
}



}   // namespace blocks
