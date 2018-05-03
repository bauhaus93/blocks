/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <sstream>

#include <glad/glad.h>

#include "application/ApplicationError.hpp"

namespace mc::graphics {

class OpenGLError: public ApplicationError {

 private:

 public:

            OpenGLError(const std::string& error,
                        const std::string& function);

            OpenGLError(const std::string& error,
                        const std::string& function,
                        const std::string& description);

};

}   // namespace mc::graphics
