/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <random>
#include <memory>

#include "SimplexNoise.hpp"

namespace mc::world::architect {

class Biome {
 public:
                            Biome(uint32_t seed);
                            Biome(Biome&& other);
    Biome&                  operator=(Biome&& other);
    void                    SetMinHeight(int32_t minHeight_);
    void                    SetMaxHeight(int32_t maxHeight_);
    void                    SetOctaves(uint32_t octaves_);
    void                    SetScale(double scale_);
    void                    SetRoughness(double roughness_);

    double                  GetValue(Point2i globalPosition) const;
    int32_t                 GetMinHeight() const { return minHeight; }
    int32_t                 GetMaxHeight() const { return maxHeight; }

 private:
    SimplexNoise            noise;
    int32_t                 minHeight;
    int32_t                 maxHeight;
    uint32_t                octaves;                     
    double                  scale;
    double                  roughness;
};


}   // namespace mc::world::architect