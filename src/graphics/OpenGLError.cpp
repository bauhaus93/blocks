/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "OpenGLError.hpp"


namespace mc::graphics {

static std::string GetErrorString();

OpenGLError::OpenGLError(const std::string& error,
                         const std::string& function):
    ApplicationError(error, function, GetErrorString()) {

}

OpenGLError::OpenGLError(const std::string& error,
                         const std::string& function,
                         const std::string& description):
    ApplicationError(error, function, description) {
}

static std::string GetErrorString() {
    switch(glGetError()) {
    case GL_NO_ERROR:           return "GL_NO_ERROR";
    case GL_INVALID_ENUM:       return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:      return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:  return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:  return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:      return "GL_OUT_OF_MEMORY";
    default:                    return "UNKNOWN CASE";
    }
}

}   // namespace mc::graphics
