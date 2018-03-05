/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "glad/glad.h"

#include "Bitmap.hpp"


namespace mc {

class Texture {

 public:
    explicit    Texture(const std::string& filePath);
                ~Texture();
    GLuint      GetId() const { return id; }
    void        MakeActive() const;

 private:
    GLuint      id;

};


}   // namespace mc
