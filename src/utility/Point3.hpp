/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>

#include "Point.hpp"

namespace mc {

template<typename T>
using Point3 = Point<T, 3>;

typedef Point3<int32_t> Point3i;
typedef Point3<uint32_t> Point3u;
typedef Point3<float> Point3f;

}       // namespace mc
