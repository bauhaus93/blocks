/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "ApplicationError.hpp"

namespace mc {

ApplicationError::ApplicationError(const std::string& error,
                                   const std::string& function,
                                   const std::string& description):
    std::runtime_error(error + " @ " + function + ": " + description) {

}



}   // namespace mc
