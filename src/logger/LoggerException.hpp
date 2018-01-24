/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <stdexcept>
#include <string>

namespace mc::log {

class LoggerException : public std::runtime_error {
 public:
    LoggerException(const std::string& function,
                    const std::string& description);
};

}  // namespace mc::log
