/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "TextureAtlas.hpp"

namespace mc::graphics {

TextureAtlas::TextureAtlas(const std::string& filePath, Point2f fieldSize_):
    texture { filePath },
    fieldSize { fieldSize_ } {
}

void TextureAtlas::MakeActive() const {
    texture.MakeActive();
}

Point2f TextureAtlas::GetFieldUV(Point2i fieldPos) const {
    return Point2f(fieldPos) / Point2f(texture.GetSize());
}

}   // namespace mc::graphics