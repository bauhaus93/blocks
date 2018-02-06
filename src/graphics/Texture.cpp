/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Texture.hpp"

namespace mc {


Texture::Texture(const std::string& filePath):
    id { ReadBitmap(filePath) } {
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



}   // namespace mc
