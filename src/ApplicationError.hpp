/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace mc {

class ApplicationError: public std::runtime_error {

 private:

 public:

            ApplicationError(const std::string& error,
                             const std::string& function,
                             const std::string& description);



};

}   // namespace mc
