/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>
#include <memory>

#include "Logger.hpp"
#include "LogLevel.hpp"

#ifdef NDEBUG
#define LOG_LEVEL INFO
#else
#define LOG_LEVEL DEBUG
#endif

namespace mc::log {

void CreateGlobalLogger(std::ostream& output);
Logger& GetGlobalLogger();
bool GlobalLoggerActive();
void DestroyGlobalLogger();

#if LOG_LEVEL == TRACE
#define TRACE(args...) mc::log::GetGlobalLogger().Trace(args)
#define DEBUG(args...) mc::log::GetGlobalLogger().Debug(args)
#define INFO(args...) mc::log::GetGlobalLogger().Info(args)
#define ERROR(args...) mc::log::GetGlobalLogger().Error(args)
#define WARN(args...) mc::log::GetGlobalLogger().Warn(args)
#elif LOG_LEVEL == DEBUG
#define TRACE(args...)
#define DEBUG(args...) mc::log::GetGlobalLogger().Debug(args)
#define INFO(args...) mc::log::GetGlobalLogger().Info(args)
#define ERROR(args...) mc::log::GetGlobalLogger().Error(args)
#define WARN(args...) mc::log::GetGlobalLogger().Warn(args)
#elif LOG_LEVEL == INFO
#define TRACE(args...)
#define DEBUG(args...)
#define INFO(args...) mc::log::GetGlobalLogger().Info(args)
#define ERROR(args...) mc::log::GetGlobalLogger().Error(args)
#define WARN(args...) mc::log::GetGlobalLogger().Warn(args)
#elif LOG_LEVEL == WARN
#define TRACE(args...)
#define DEBUG(args...)
#define INFO(args...)
#define ERROR(args...) mc::log::GetGlobalLogger().Error(args)
#define WARN(args...) mc::log::GetGlobalLogger().Warn(args)
#elif LOG_LEVEL == ERROR
#define TRACE(args...)
#define DEBUG(args...)
#define INFO(args...)
#define ERROR(args...)
#define WARN(args...) mc::log::GetGlobalLogger().Warn(args)
#endif

}   // namespace mc::log
