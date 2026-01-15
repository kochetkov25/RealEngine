#include "Logger.h"

#include <algorithm>
#include <filesystem>

namespace Core {

// Initialize static members
Logger::LogLevel Logger::minLogLevel_ = Logger::LogLevel::Info;
std::mutex Logger::logMutex_;

void Logger::setLogLevel(LogLevel level) noexcept {
  std::lock_guard<std::mutex> lock(logMutex_);
  minLogLevel_ = level;
}

Logger::LogLevel Logger::getLogLevel() noexcept {
  std::lock_guard<std::mutex> lock(logMutex_);
  return minLogLevel_;
}

std::string Logger::getCurrentTime() {
  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);
  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;

  std::ostringstream stream;

#ifdef _WIN32
  struct tm timeInfo;
  localtime_s(&timeInfo, &time);
  stream << std::put_time(&timeInfo, "%H:%M:%S");
#else
  struct tm timeInfo;
  localtime_r(&time, &timeInfo);
  stream << std::put_time(&timeInfo, "%H:%M:%S");
#endif

  stream << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return stream.str();
}

const char *Logger::levelToString(LogLevel level) noexcept {
  switch (level) {
  case LogLevel::Trace:
    return "TRACE";
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO ";
  case LogLevel::Warning:
    return "WARN ";
  case LogLevel::Error:
    return "ERROR";
  case LogLevel::Critical:
    return "CRIT ";
  default:
    return "UNKNW";
  }
}

std::string Logger::extractFilename(const std::string &path) noexcept {
  try {
#ifdef _WIN32
    const size_t pos = path.find_last_of("\\/");
#else
    const size_t pos = path.find_last_of('/');
#endif
    if (pos != std::string::npos) {
      return path.substr(pos + 1);
    }
    return path;
  } catch (...) {
    // Fallback on exception
    return path;
  }
}

} // namespace Core
