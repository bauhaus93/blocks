/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "graphics/ShaderProgram.hpp"
#include "utility/Color.hpp"

namespace mc::world {

class Fog {
 public:
    explicit    Fog(graphics::ShaderProgram& shader_);

    void        ModDensity(float factor);
    void        SetDensity(float value);
    void        SetColor(Color newColor);

 private:
     graphics::ShaderProgram&       shader;
     float                          density;
     Color                          color;

     void       UpdateShader();
};


}   // namespace mc::world
