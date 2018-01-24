/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>

#include "LogLevel.hpp"

namespace mc::log {

class Logger {
 public:
                        Logger(std::ostream& out_, LogLevel logLevel_);

        template<typename... Args>
        void            Trace(Args... args);
        template<typename... Args>
        void            Debug(Args... args);
        template<typename... Args>
        void            Info(Args... args);
        template<typename... Args>
        void            Warn(Args... args);
        template<typename... Args>
        void            Error(Args... args);

 private:
        std::ostream&   out;
        LogLevel        logLevel;

        template<typename T, typename... Args>
        void            Write(LogLevel msgLevel, T value, Args... args);
        template<typename T>
        void            Write(LogLevel msgLevel, T value);

        template<typename T, typename... Args>
        void            WriteAppend(T value, Args... args);
        template<typename T>
        void            WriteAppend(T value);
};

const char* GetLogLevelString(LogLevel logLevel);
int         GetLogLevelIndex(LogLevel logLevel);

template<typename... Args>
void Logger::Trace(Args... args) {
    if (GetLogLevelIndex(LogLevel::TRACE) >= GetLogLevelIndex(logLevel))
        Write(LogLevel::TRACE, args...);
}

template<typename... Args>
void Logger::Debug(Args... args) {
    if (GetLogLevelIndex(LogLevel::DEBUG) >= GetLogLevelIndex(logLevel))
        Write(LogLevel::DEBUG, args...);
}

template<typename... Args>
void Logger::Info(Args... args) {
    if (GetLogLevelIndex(LogLevel::INFO) >= GetLogLevelIndex(logLevel))
        Write(LogLevel::INFO, args...);
}

template<typename... Args>
void Logger::Warn(Args... args) {
    if (GetLogLevelIndex(LogLevel::WARN) >= GetLogLevelIndex(logLevel))
        Write(LogLevel::WARN, args...);
}

template<typename... Args>
void Logger::Error(Args... args) {
    if (GetLogLevelIndex(LogLevel::ERROR) >= GetLogLevelIndex(logLevel))
        Write(LogLevel::ERROR, args...);
}

template<typename T, typename... Args>
void Logger::Write(LogLevel msgLevel, T value, Args... args) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    out << std::put_time(&tm, "[%T] ")
        << GetLogLevelString(msgLevel) << " - " << value;
    WriteAppend(args...);
}

template<typename T>
void Logger::Write(LogLevel msgLevel, T value) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    out << std::put_time(&tm, "[%T] ")
        << GetLogLevelString(msgLevel) << " - " << value << "\n";
}

template<typename T, typename... Args>
void Logger::WriteAppend(T value, Args... args) {
    out << value;
    WriteAppend(args...);
}

template<typename T>
void Logger::WriteAppend(T value) {
    out << value << "\n";
}


}   // namespace mc::log
