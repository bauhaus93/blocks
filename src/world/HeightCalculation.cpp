/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "HeightCalculation.hpp"

namespace mc::world {

int32_t CalculateHeight(const Point2i& globalBlockPos, const SimplexNoise& heightNoise) {
    constexpr double MIN_HEIGHT = 1.0;
    constexpr double HEIGHT_VARIATION = 10.0;
    double normalizedNoise = (1.0 + heightNoise.GetOctavedNoise(
        globalBlockPos[0],
        globalBlockPos[1],
        6, 0.1, 0.025)) / 2.0;
    return static_cast<int32_t>(MIN_HEIGHT + HEIGHT_VARIATION * normalizedNoise);
}

}   // namespace mc::world