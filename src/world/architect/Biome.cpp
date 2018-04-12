/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Biome.hpp"

namespace mc::world::architect {

Biome::Biome(uint32_t seed):
    noise { seed },
    minHeight { 100 },
    maxHeight { 200 },
    octaves { 2 },
    scale { 0.0005 },
    roughness { 0.5 } {
}

Biome::Biome(Biome&& other):
    noise { std::move(other.noise) },
    minHeight { other.minHeight },
    maxHeight { other.maxHeight },
    octaves { other.octaves },
    scale { other.scale },
    roughness { other.roughness } {
}

Biome& Biome::operator=(Biome&& other) {
    noise = std::move(other.noise);
    minHeight = other.minHeight;
    maxHeight = other.maxHeight;
    octaves = other.octaves;
    scale = other.scale;
    roughness = other.roughness;
    return *this;
}

void Biome::SetMinHeight(int32_t minHeight_) {
    minHeight = minHeight_;
}

void Biome::SetMaxHeight(int32_t maxHeight_) {
    maxHeight = maxHeight_;
}

void Biome::SetOctaves(uint32_t octaves_) {
    octaves = octaves_;
}

void Biome::SetScale(double scale_) {
    scale = scale_;
}

void Biome::SetRoughness(double roughness_) {
    roughness = roughness_;
}

double Biome::GetValue(Point2i globalPosition) const {
    return noise.GetOctavedNoise(globalPosition, octaves, roughness, scale);
}

}   // namespace mc::world::architect