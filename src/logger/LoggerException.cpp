/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "LoggerException.hpp"

namespace mc::log {

LoggerException::LoggerException(
    const std::string& function,
    const std::string& description):
    std::runtime_error("LoggerException @ " + function + ": " + description){
}

}   // namespace mc::log
