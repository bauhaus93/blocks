/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <cstdint>

#include "glad/glad.h"
#include "utility/Point2.hpp"

namespace mc::graphics {

class TextureAtlas {
 public:
                TextureAtlas(Point2u textureSize_, uint32_t layerCount_);
                ~TextureAtlas();
    void        MakeActive();
    uint32_t    AddTextureLayer(std::vector<uint8_t>& textureData);

 private:
    const Point2u                   textureSize;
    const uint32_t                  layerCount;
    uint32_t                        nextLayer;
    GLuint                          textureId;
};

}   // namespace mc::graphics