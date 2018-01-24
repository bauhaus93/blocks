/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "GlobalLogger.hpp"

namespace mc::log {

std::unique_ptr<Logger> g_logger = nullptr;

void CreateGlobalLogger(std::ostream& output) {
    if (g_logger == nullptr) {
        g_logger = std::make_unique<Logger>(output, LogLevel::LOG_LEVEL);
    } else {
        throw LoggerException(
            __FUNCTION__,
            "Global logger already existing");
    }
}

Logger& GetGlobalLogger() {
    if (g_logger != nullptr) {
        return *g_logger;
    }
    throw LoggerException(__FUNCTION__,
                          "No global logger existing");
}

void DestroyGlobalLogger() {
    if (g_logger != nullptr) {
        g_logger = nullptr;
    } else {
        throw LoggerException(__FUNCTION__,
                              "Global logger not existing");
    }
}

}   // namespace mc::log
