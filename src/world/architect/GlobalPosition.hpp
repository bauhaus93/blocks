/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "utility/Point2.hpp"
#include "world/Size.hpp"

namespace mc::world::architect {

Point2i GetGlobalPosition(Point2i chunkPos, Point2i localPos);

}   // namespace mc::world::architect