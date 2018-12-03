/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <cstdint>

#include "glad/glad.h"
#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Image.hpp"

namespace blocks {

class TextureAtlas {
 public:
                TextureAtlas(Point2u textureSize_, uint32_t layerCount_);
                ~TextureAtlas();
    void        Activate();
    void        Deactivate();
    uint32_t    AddTextureLayer(const Image& img);
    Point2u     GetTextureSize() const { return textureSize; }

 private:
    const Point2u                   textureSize;
    const uint32_t                  layerCount;
    uint32_t                        nextLayer;
    GLuint                          textureId;
};

}   // namespace blocks
