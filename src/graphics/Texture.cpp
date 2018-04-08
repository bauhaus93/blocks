/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Texture.hpp"

namespace mc::graphics {

Texture::Texture(const std::string& filePath):
    id { 0 },
    size { Point2i(0) } {
    BitmapResult result { ReadBitmap(filePath) };
    id = result.id;
    size = result.size;
}

Texture::Texture(Texture&& other):
    id { other.id },
    size { other.size } {
    other.id = 0;
}

Texture& Texture::operator=(Texture&& other) {
    id = other.id;
    size = other.size;
    other.id = 0;
    return *this;
}

Texture::~Texture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
}

void Texture::MakeActive() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

}   // namespace mc::graphics
