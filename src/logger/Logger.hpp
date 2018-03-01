/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <iostream>
#include <mutex>
#include <iomanip>
#include <ctime>

#include "LogLevel.hpp"

namespace mc::log {

class Logger {
 public:
                        Logger(std::ostream& out_, LogLevel logLevel_);

        template<typename... Args>
        void            Trace(const Args&... args);
        template<typename... Args>
        void            Debug(const Args&... args);
        template<typename... Args>
        void            Info(const Args&... args);
        template<typename... Args>
        void            Warn(const Args&... args);
        template<typename... Args>
        void            Error(const Args&... args);

 private:
        std::ostream&   out;
        LogLevel        logLevel;
        std::mutex      mutex;

        template<typename T, typename... Args>
        void            Write(LogLevel msgLevel, const T& value, const Args&... args);
        template<typename T>
        void            Write(LogLevel msgLevel, const T& value);

        template<typename T, typename... Args>
        void            WriteAppend(const T& value, const Args&... args);
        template<typename T>
        void            WriteAppend(const T& value);
};

const char* GetLogLevelString(LogLevel logLevel);
int         GetLogLevelIndex(LogLevel logLevel);

template<typename... Args>
void Logger::Trace(const Args&... args) {
    if (GetLogLevelIndex(LogLevel::TRACE) >= GetLogLevelIndex(logLevel)) {
        mutex.lock();
        Write(LogLevel::TRACE, args...);
        mutex.unlock();
    }
}

template<typename... Args>
void Logger::Debug(const Args&... args) {
    if (GetLogLevelIndex(LogLevel::DEBUG) >= GetLogLevelIndex(logLevel)) {
        mutex.lock();
        Write(LogLevel::DEBUG, args...);
        mutex.unlock();
    }
}

template<typename... Args>
void Logger::Info(const Args&... args) {
    if (GetLogLevelIndex(LogLevel::INFO) >= GetLogLevelIndex(logLevel)) {
        mutex.lock();
        Write(LogLevel::INFO, args...);
        mutex.unlock();
    }
}

template<typename... Args>
void Logger::Warn(const Args&... args) {
    if (GetLogLevelIndex(LogLevel::WARN) >= GetLogLevelIndex(logLevel)) {
        mutex.lock();
        Write(LogLevel::WARN, args...);
        mutex.unlock();
    }
}

template<typename... Args>
void Logger::Error(const Args&... args) {
    if (GetLogLevelIndex(LogLevel::ERROR) >= GetLogLevelIndex(logLevel)) {
        mutex.lock();
        Write(LogLevel::ERROR, args...);
        mutex.unlock();
    }
}

template<typename T, typename... Args>
void Logger::Write(LogLevel msgLevel, const T& value, const Args&... args) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    out << std::put_time(&tm, "[%T] ")
        << GetLogLevelString(msgLevel) << " - " << value;
    WriteAppend(args...);
}

template<typename T>
void Logger::Write(LogLevel msgLevel, const T& value) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    out << std::put_time(&tm, "[%T] ")
        << GetLogLevelString(msgLevel) << " - " << value << "\n";
}

template<typename T, typename... Args>
void Logger::WriteAppend(const T& value, const Args&... args) {
    out << value;
    WriteAppend(args...);
}

template<typename T>
void Logger::WriteAppend(const T& value) {
    out << value << "\n";

}


}   // namespace mc::log
