/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "graphics/ShaderProgram.hpp"
#include "utility/Color.hpp"

namespace mc::world {

class Fog {
 public:
            Fog(ShaderProgram& shader);

            ModDensity(float value);
            SetDensity(float value);
            SetColor(Color newColor);

 private:
     Shader&    shader;
     float      density;
     Color      color;
};


}   // namespace mc::world
