/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "TextureAtlas.hpp"

namespace mc::graphics {

TextureAtlas::TextureAtlas(const std::string& filePath, Point2f fieldSize_):
    texture { filePath },
    fieldSize { fieldSize_ } {
    INFO("Created texture atlas size = ", texture.GetSize());
}

void TextureAtlas::MakeActive() const {
    texture.MakeActive();
}

Point2f TextureAtlas::GetFieldUV(Point2i fieldPos, uint8_t corner) const {
    assert(corner < 4);
    Point2f fieldPerc = fieldSize / Point2f(texture.GetSize());
    Point2f uv = Point2f(fieldPos) * fieldPerc;
    switch (corner) {
        case 0:                                     break;
        case 1: uv += Point2f(0.0f, fieldPerc[1]);   break;
        case 2: uv += Point2f(fieldPerc[0], 0.0f);   break;
        case 3: uv += fieldPerc;                     break;
        default:    assert(0);
    }
    return uv;
}

}   // namespace mc::graphics