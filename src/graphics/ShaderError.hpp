/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <vector>

#include "glad/Glad.h"

#include "OpenGLError.hpp"

namespace mc {

class ShaderError: public OpenGLError {

 private:

 public:

        ShaderError(const std::string& function, GLuint shaderId);



};

}   // namespace mc
