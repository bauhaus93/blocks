/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>

#include "Texture.hpp"

namespace mc::graphics {

class TextureAtlas {
 public:
            TextureAtlas(const std::string& filePath, Point2f fieldSize_);
    void    MakeActive() const;
    Point2f GetFieldUV(Point2i fieldPos) const;

 private:
    const Texture   texture;
    const Point2f   fieldSize;

};

}   // namespace mc::graphics