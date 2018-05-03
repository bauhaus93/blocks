/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Fog.hpp"

namespace mc::world {

Fog::Fog():
    density { 0.0015 },
    color { 0.8f, 0.8f, 0.8f } {
}


            ModDensity(float value);
            SetDensity(float value);
            SetColor(Color newColor);

 private:
     float      density;
     Color      color;
};


}   // namespace mc::world
