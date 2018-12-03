/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#ifdef _MSC_VER
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <iostream>
#include <mutex>
#include <iomanip>
#include <ctime>

#include "LogLevel.hpp"

namespace blocks {

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

        void            WriteMessagePrefix(LogLevel msgLevel);
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
  WriteMessagePrefix(msgLevel);
  out << value;
  WriteAppend(args...);
}

template<typename T>
void Logger::Write(LogLevel msgLevel, const T& value) {
  WriteMessagePrefix(msgLevel);
  out << value << std::endl;
}

template<typename T, typename... Args>
void Logger::WriteAppend(const T& value, const Args&... args) {
    out << value;
    WriteAppend(args...);
}

template<typename T>
void Logger::WriteAppend(const T& value) {
    out << value << std::endl;
}

}   // namespace blocks
