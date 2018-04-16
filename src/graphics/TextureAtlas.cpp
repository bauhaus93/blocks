/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "TextureAtlas.hpp"

namespace mc::graphics {

TextureAtlas::TextureAtlas(Point2u textureSize_, uint32_t layerCount_):
    textureSize { textureSize_ },
    layerCount { layerCount_ },
    nextLayer { 0 },
    textureId { 0 } {
    
    uint32_t mipmaps = 1;
    uint32_t mipmapDivider = 2;
    while (textureSize[0] / mipmapDivider > 0 &&
           textureSize[1] / mipmapDivider > 0) {
        mipmaps++;
        mipmapDivider *= 2;
    }
    INFO("Creating TextureAtlas: size = ", textureSize, ", layers = ", layerCount, ", mipmaps = ", mipmaps);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipmaps, GL_RGB8, textureSize[0], textureSize[1], layerCount);
    Deactivate();

}

TextureAtlas::~TextureAtlas() {
    if (textureId != 0) {
        glDeleteTextures(1, &textureId);
    }
}

void TextureAtlas::Activate() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
}

void TextureAtlas::Deactivate() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

uint32_t TextureAtlas::AddTextureLayer(const Image& img) {
    assert(nextLayer < layerCount);
    assert(img.GetDepth() == 24);
    assert(img.GetSize() == textureSize);

    Activate();

    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,      
        0, 0, static_cast<GLint>(nextLayer),
        static_cast<GLint>(textureSize[0]), static_cast<GLint>(textureSize[1]), 1,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        img.GetData().data()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    Deactivate();
    return nextLayer++;
}

}   // namespace mc::graphics