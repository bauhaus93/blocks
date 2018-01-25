/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>
#include <sstream>

#include <glad/glad.h>

#include "ApplicationError.hpp"

namespace mc {

class OpenGLError: public ApplicationError {

 private:

 public:

            OpenGLError(const std::string& error,
                        const std::string& function);



};

}   // namespace mc
