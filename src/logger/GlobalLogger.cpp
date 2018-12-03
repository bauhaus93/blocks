/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "GlobalLogger.hpp"

namespace blocks {

std::unique_ptr<Logger> g_logger = nullptr;

void CreateGlobalLogger(std::ostream& output) {
    if (g_logger == nullptr) {
        g_logger = std::make_unique<Logger>(output, LogLevel::LOG_LEVEL);
        INFO("Created global logger");
    }
}

Logger& GetGlobalLogger() {
    if (g_logger != nullptr) {
        CreateGlobalLogger(std::cout);
    }
    return *g_logger;
}

bool GlobalLoggerActive() {
    return g_logger != nullptr;
}

void DestroyGlobalLogger() {
    if (g_logger != nullptr) {
        INFO("Destroying global logger");
        g_logger = nullptr;
    }
}

}   // namespace blocks
