/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"


namespace mc::world {

typedef std::vector<uint8_t> Vec8u;

class SimplexNoise {
 public:
                SimplexNoise();
    explicit    SimplexNoise(uint32_t seed_);
                SimplexNoise(const SimplexNoise& other) = delete;
                SimplexNoise(SimplexNoise&& other) = delete;
                ~SimplexNoise() = default;

    uint32_t    GetSeed() const;

    double      GetOctavedNoise(Point2i pos, int octaves, double roughness, double scale) const;

 private:    
    const uint32_t  seed;
    std::mt19937    rng;
    Vec8u           permutation;

    double          GetNoise(double x, double y) const;
    double          Fade(double t) const;
    double          Lerp(double t, double a, double b) const;
    double          Gradient(int hash, double x, double y, double z) const;


};

}   // namespace mc::world::chunk
