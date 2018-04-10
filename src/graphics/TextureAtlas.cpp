/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "TextureAtlas.hpp"

namespace mc::graphics {

TextureAtlas::TextureAtlas(Point2u textureSize_, uint32_t layerCount_):
    textureSize { textureSize_ },
    layerCount { layerCount_ },
    nextLayer { 0 },
    textureId { 0 } {

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, textureSize[0], textureSize[1], layerCount);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

TextureAtlas::~TextureAtlas() {
    if (textureId != 0) {
        glDeleteTextures(1, &textureId);
    }
}

void TextureAtlas::MakeActive() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
}

uint32_t TextureAtlas::AddTextureLayer(std::vector<uint8_t>& textureData) {
    assert(nextLayer < layerCount);
    //INFO(textureData.size(), " vs", textureSize[0] * textureSize[1] * 3);
    assert(textureData.size() == textureSize[0] * textureSize[1] * 3);

    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,      
        0, 0, static_cast<GLint>(nextLayer),
        static_cast<GLint>(textureSize[0]), static_cast<GLint>(textureSize[1]), 1,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        textureData.data()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_NEAREST);
    
    return nextLayer++;
}

}   // namespace mc::graphics