/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "FileNotFoundError.hpp"

namespace mc {

FileNotFoundError::FileNotFoundError(const std::string& function,
                                     const std::string& filename):
    ApplicationError("FileNotFound", function, "File \"" + filename + "\" not existing") {

}



}   // namespace mc
