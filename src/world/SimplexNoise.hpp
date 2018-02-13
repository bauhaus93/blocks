/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */
#pragma once

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>

class SimplexNoise{
 public:
                SimplexNoise();
    explicit    SimplexNoise(uint32_t seed_);
                ~SimplexNoise() = default;

    uint32_t    GetSeed() const;
    double      GetNoise(double x, double y) const;
    double      GetOctavedNoise(int x, int y, int octaves, double roughness, double scale) const;
    
 private:
    const uint32_t          seed;
    std::mt19937            rng;
    std::vector<uint8_t>    permutation;

    double              Fade(double t) const;
    double              Lerp(double t, double a, double b) const;
    double              Gradient(int hash, double x, double y, double z) const;


};
