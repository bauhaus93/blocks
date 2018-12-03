/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "world/Size.hpp"

namespace blocks {

Point2i GetGlobalPosition(Point2i chunkPos, Point2i8 localPos);
Point3i GetGlobalPosition(Point3i chunkPos, Point3i8 localPos);

}   // namespace blocks
