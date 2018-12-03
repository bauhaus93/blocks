/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "graphics/ShaderProgram.hpp"
#include "utility/Color.hpp"

namespace blocks {

class Fog {
 public:
    explicit    Fog(ShaderProgram& shader_);

    void        ModDensity(float factor);
    void        SetDensity(float value);
    void        SetColor(Color newColor);

 private:
     ShaderProgram&                 shader;
     float                          density;
     Color                          color;

     void       UpdateShader();
};


}   // namespace blocks
