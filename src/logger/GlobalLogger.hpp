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

namespace blocks {

void CreateGlobalLogger(std::ostream& output);
Logger& GetGlobalLogger();
bool GlobalLoggerActive();
void DestroyGlobalLogger();

#if LOG_LEVEL == TRACE
#define TRACE(...) blocks::GetGlobalLogger().Trace(__VA_ARGS__)
#define DEBUG(...) blocks::GetGlobalLogger().Debug(__VA_ARGS__)
#define INFO(...) blocks::GetGlobalLogger().Info(__VA_ARGS__)
#define WARN(...) blocks::GetGlobalLogger().Warn(__VA_ARGS__)
#define ERROR(...) blocks::GetGlobalLogger().Error(__VA_ARGS__)
#elif LOG_LEVEL == DEBUG
#define TRACE(...) ((void)0)
#define DEBUG(...) blocks::GetGlobalLogger().Debug(__VA_ARGS__)
#define INFO(...) blocks::GetGlobalLogger().Info(__VA_ARGS__)
#define WARN(...) blocks::GetGlobalLogger().Warn(__VA_ARGS__)
#define ERROR(...) blocks::GetGlobalLogger().Error(__VA_ARGS__)
#elif LOG_LEVEL == INFO
#define TRACE(...) ((void)0)
#define DEBUG(...) ((void)0)
#define INFO(...) blocks::GetGlobalLogger().Info(__VA_ARGS__)
#define WARN(...) blocks::GetGlobalLogger().Warn(__VA_ARGS__)
#define ERROR(...) blocks::GetGlobalLogger().Error(__VA_ARGS__)
#elif LOG_LEVEL == WARN
#define TRACE(...) ((void)0)
#define DEBUG(...) ((void)0)
#define INFO(...) ((void)0)
#define WARN(...) blocks::GetGlobalLogger().Warn(__VA_ARGS__)
#define ERROR(...) blocks::GetGlobalLogger().Error(__VA_ARGS__)
#elif LOG_LEVEL == ERROR
#define TRACE(...) ((void)0)
#define DEBUG(...) ((void)0)
#define INFO(...) ((void)0)
#define WARN(...) ((void)0)
#define ERROR(...) blocks::GetGlobalLogger().Error(__VA_ARGS__)
#endif

}   // namespace blocks
