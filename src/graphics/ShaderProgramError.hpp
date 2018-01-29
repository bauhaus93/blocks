/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>

#include "OpenGLError.hpp"

namespace mc {

class ShaderProgramError: public OpenGLError {

 private:

 public:

        ShaderProgramError(const std::string& function, GLuint programId);
        ShaderProgramError(const std::string& function, const std::string& description);
};

}   // namespace mc
