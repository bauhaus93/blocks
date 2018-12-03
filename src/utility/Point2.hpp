/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "Point.hpp"

namespace blocks {

template<typename T>
using Point2 = Point<T, 2>;

typedef Point2<int8_t> Point2i8;
typedef Point2<uint8_t> Point2u8;
typedef Point2<int32_t> Point2i;
typedef Point2<uint32_t> Point2u;
typedef Point2<float> Point2f;

}       // namespace blocks
