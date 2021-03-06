/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

#include "OpenGLError.hpp"

namespace blocks {

class ShaderError: public OpenGLError {

 private:

 public:

        ShaderError(const std::string& function, GLuint shaderId);



};

}   // namespace blocks
