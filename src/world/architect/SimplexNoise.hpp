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

namespace mc::world::architect {

typedef std::vector<uint8_t> Vec8u;

class SimplexNoise {
 public:
                    SimplexNoise();
    explicit        SimplexNoise(uint32_t seed_);
                    SimplexNoise(SimplexNoise&& other) = default;
    SimplexNoise&   operator=(SimplexNoise&& other) = default;
                    ~SimplexNoise() = default;

    void            SetOctaves(int octaves_);
    void            SetRoughness(double roughness_);
    void            SetScale(double scale_);
    void            SetMin(double min_);
    void            SetMax(double max_);

    uint32_t        GetSeed() const;

    double      GetNoise(Point2i pos) const;

 private:    
    uint32_t        seed;
    std::mt19937    rng;
    int             octaves;
    double          roughness;
    double          scale;
    double          min;
    double          max;
    Vec8u           permutation;


    double          GetRawNoise(double x, double y) const;
    double          Fade(double t) const;
    double          Lerp(double t, double a, double b) const;
    double          Gradient(int hash, double x, double y, double z) const;


};

}   // namespace mc::world::architect
