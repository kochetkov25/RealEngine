#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace Resources {

/**
 * @brief Simple structured logging utility for model loading operations
 *
 * Provides different log levels and structured output for debugging
 * and production use. Can be extended to support file logging,
 * log rotation, and filtering in the future.
 */
class ModelLogger {
public:
  enum class LogLevel { Debug, Info, Warning, Error };

  /**
   * @brief Set the minimum log level (messages below this level are ignored)
   */
  static void setLogLevel(LogLevel level) noexcept { minLogLevel_ = level; }

  /**
   * @brief Get the current minimum log level
   */
  static LogLevel getLogLevel() noexcept { return minLogLevel_; }

  /**
   * @brief Log a debug message
   */
  template <typename... Args>
  static void debug(const std::string &context, Args &&...args) {
    log(LogLevel::Debug, context, std::forward<Args>(args)...);
  }

  /**
   * @brief Log an info message
   */
  template <typename... Args>
  static void info(const std::string &context, Args &&...args) {
    log(LogLevel::Info, context, std::forward<Args>(args)...);
  }

  /**
   * @brief Log a warning message
   */
  template <typename... Args>
  static void warning(const std::string &context, Args &&...args) {
    log(LogLevel::Warning, context, std::forward<Args>(args)...);
  }

  /**
   * @brief Log an error message
   */
  template <typename... Args>
  static void error(const std::string &context, Args &&...args) {
    log(LogLevel::Error, context, std::forward<Args>(args)...);
  }

private:
  template <typename... Args>
  static void log(LogLevel level, const std::string &context, Args &&...args) {
    if (level < minLogLevel_) {
      return;
    }

    std::ostringstream stream;
    stream << "[" << getCurrentTime() << "] "
           << "[" << levelToString(level) << "] "
           << "[" << context << "] ";

    // Use fold expression to concatenate arguments
    (stream << ... << std::forward<Args>(args));

    stream << std::endl;

    // Output to appropriate stream
    if (level >= LogLevel::Warning) {
      std::cerr << stream.str();
    } else {
      std::cout << stream.str();
    }
  }

  static std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
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

  static const char *levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::Debug:
      return "DEBUG";
    case LogLevel::Info:
      return "INFO ";
    case LogLevel::Warning:
      return "WARN ";
    case LogLevel::Error:
      return "ERROR";
    default:
      return "UNKNW";
    }
  }

  static LogLevel minLogLevel_;
};

// Initialize static member
inline ModelLogger::LogLevel ModelLogger::minLogLevel_ = LogLevel::Info;

} // namespace Resources
