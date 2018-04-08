/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include "utility/Point2.hpp"
#include "glad/glad.h"
#include "Bitmap.hpp"

namespace mc::graphics {

class Texture {

 public:
    explicit    Texture(const std::string& filePath);
                ~Texture();
                Texture(Texture&& other);
    Texture&    operator=(Texture&& other);
    GLuint      GetId() const { return id; }
    Point2i     GetSize() const { return size; }
    void        MakeActive() const;

 private:
    GLuint      id;
    Point2i     size;

};


}   // namespace mc::graphics
