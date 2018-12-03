/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>

#include "logger/GlobalLogger.hpp"
#include "utility/Point3.hpp"

namespace blocks {

class Volume {
 public:
                    Volume(Point3f min_, Point3f max_);
    bool            Contains(Point3f point) const;
 private:
     Point3f        min;
     Point3f        max;
};


}   // namespace blocks
