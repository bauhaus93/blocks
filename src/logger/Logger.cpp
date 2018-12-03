/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Logger.hpp"

namespace blocks {

const char* GetLogLevelString(LogLevel logLevel) {
    switch (logLevel) {
    case LogLevel::TRACE:   return "TRACE";
    case LogLevel::DEBUG:   return "DEBUG";
    case LogLevel::INFO:    return "INFO";
    case LogLevel::WARN:    return "WARN";
    case LogLevel::ERROR:   return "ERROR";
    default:                return "UNREACHABLE";
    }
}

int GetLogLevelIndex(LogLevel logLevel) {
    switch (logLevel) {
    case LogLevel::TRACE:   return 0;
    case LogLevel::DEBUG:   return 1;
    case LogLevel::INFO:    return 2;
    case LogLevel::WARN:    return 3;
    case LogLevel::ERROR:   return 4;
    default:                return -1;
    }
}

Logger::Logger(std::ostream& out_, LogLevel logLevel_) :
    out { out_ },
    logLevel { logLevel_ },
    mutex { } {
}

void Logger::WriteMessagePrefix(LogLevel msgLevel) {
  std::time_t t = std::time(nullptr);
  #ifdef _MSC_VER
  std::tm tm;
  if (localtime_s(&tm, &t) == 0) {
  #else
  std::tm* tm = localtime(&t);
  if (tm != nullptr) {
  #endif
      out << std::put_time(tm, "[%T] ");
  }
  else {
    out << "[NO_TIME] ";
  }
  switch(msgLevel) {
      case LogLevel::TRACE:
      case LogLevel::DEBUG:
      case LogLevel::INFO:  out << "\033[1;32m";    break;
      case LogLevel::WARN:  out << "\033[1;93m";    break;
      case LogLevel::ERROR: out << "\033[1;31m";    break;
      default:  break;
  }
  out << GetLogLevelString(msgLevel) << "\033[0m - ";
}

}   // namespace blocks
