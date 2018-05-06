/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <array>


namespace mc {

class Color {
 public:
                Color(float r, float g, float b);
    float       GetRed() const { return rgb[0]; }
    float       GetGreen() const { return rgb[1]; }
    float       GetBlue() const { return rgb[2]; }
 private:
    std::array<float, 3>  rgb;
};


}   // namespace mc
