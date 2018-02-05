/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <string>

#include "ApplicationError.hpp"

namespace mc {

class FileNotFoundError: public ApplicationError {

 private:

 public:

            FileNotFoundError(const std::string& function,
                              const std::string& filename);



};

}   // namespace mc
