/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Texture.hpp"

namespace mc {


Texture::Texture(const std::string& filePath):
    id { ReadBitmap(filePath) } {

}



}   // namespace mc
