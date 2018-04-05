/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>

#include "utility/Point2.hpp"
#include "SimplexNoise.hpp"

namespace mc::world {

int32_t CalculateHeight(const Point2i& globalBlockPos, const SimplexNoise& heightNoise);

}   // namespace mc::world