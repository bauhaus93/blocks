/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Fog.hpp"

namespace blocks {

Fog::Fog(ShaderProgram& shader_):
    shader { shader_ },
    density { 0.0015 },
    color { 0.7f, 0.7f, 0.7f } {
    UpdateShader();
}

void Fog::ModDensity(float factor) {
    density *= factor;
    if (density > 1.0f) {
        density = 1.0f;
    } else if (density < 0.0f) {
        density = 0.0f;
    }
    UpdateShader();
}

void Fog::SetDensity(float value) {
    density = value;
    if (density > 1.0f) {
        density = 1.0f;
    } else if (density < 0.0f) {
        density = 0.0f;
    }
    UpdateShader();
}

void Fog::SetColor(Color newColor) {
    color = newColor;
    UpdateShader();
}

void Fog::UpdateShader() {
    shader.Activate();
    shader.SetFogDensity(density);
    shader.SetFogColor(color);
    shader.Deactivate();
}

}   // namespace blocks
