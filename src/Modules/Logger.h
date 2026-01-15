#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>

// C++20 source_location support with C++17 fallback
#if __cplusplus >= 202002L && defined(__cpp_lib_source_location)
#define REALENGINE_LOGGER_USE_SOURCE_LOCATION
#include <source_location>
#endif

namespace Core {

class Logger {
public:
  /**
   * @brief Log levels in order of severity (lowest to highest)
   */
  enum class LogLevel : int {
    Trace = 0,   // Detailed diagnostic information (most verbose)
    Debug = 1,   // Debugging information
    Info = 2,    // Informational messages (default)
    Warning = 3, // Warning messages
    Error = 4,   // Error messages
    Critical = 5 // Critical/fatal errors (highest severity)
  };

  /**
   * @brief Set the minimum log level (messages below this level are ignored)
   * @param level Minimum log level to output
   *
   * In release builds, this also affects conditional compilation - messages
   * below the configured level are completely removed from the binary.
   */
  static void setLogLevel(LogLevel level) noexcept;

  /**
   * @brief Get the current minimum log level
   */
  [[nodiscard]] static LogLevel getLogLevel() noexcept;

  /**
   * @brief Log a trace-level message
   *
   * Used for extremely detailed diagnostic information.
   */
  template <typename... Args>
  static void trace(const std::string &context, Args &&...args) {
#if defined(REALENGINE_LOGGER_USE_SOURCE_LOCATION)
    log(LogLevel::Trace, context, std::source_location::current(),
        std::forward<Args>(args)...);
#else
    log(LogLevel::Trace, context, "", 0, std::forward<Args>(args)...);
#endif
  }

  /**
   * @brief Log a debug-level message
   *
   * Used for debugging information during development.
   */
  template <typename... Args>
  static void debug(const std::string &context, Args &&...args) {
#if defined(REALENGINE_LOGGER_USE_SOURCE_LOCATION)
    log(LogLevel::Debug, context, std::source_location::current(),
        std::forward<Args>(args)...);
#else
    log(LogLevel::Debug, context, "", 0, std::forward<Args>(args)...);
#endif
  }

  /**
   * @brief Log an info-level message
   *
   * Used for general informational messages (default log level).
   */
  template <typename... Args>
  static void info(const std::string &context, Args &&...args) {
#if defined(REALENGINE_LOGGER_USE_SOURCE_LOCATION)
    log(LogLevel::Info, context, std::source_location::current(),
        std::forward<Args>(args)...);
#else
    log(LogLevel::Info, context, "", 0, std::forward<Args>(args)...);
#endif
  }

  /**
   * @brief Log a warning-level message
   *
   * Used for warning messages that don't prevent operation.
   */
  template <typename... Args>
  static void warning(const std::string &context, Args &&...args) {
#if defined(REALENGINE_LOGGER_USE_SOURCE_LOCATION)
    log(LogLevel::Warning, context, std::source_location::current(),
        std::forward<Args>(args)...);
#else
    log(LogLevel::Warning, context, "", 0, std::forward<Args>(args)...);
#endif
  }

  /**
   * @brief Log an error-level message
   *
   * Used for error conditions that may affect functionality.
   */
  template <typename... Args>
  static void error(const std::string &context, Args &&...args) {
#if defined(REALENGINE_LOGGER_USE_SOURCE_LOCATION)
    log(LogLevel::Error, context, std::source_location::current(),
        std::forward<Args>(args)...);
#else
    log(LogLevel::Error, context, "", 0, std::forward<Args>(args)...);
#endif
  }

  /**
   * @brief Log a critical-level message
   *
   * Used for critical/fatal errors that may cause termination.
   */
  template <typename... Args>
  static void critical(const std::string &context, Args &&...args) {
#if defined(REALENGINE_LOGGER_USE_SOURCE_LOCATION)
    log(LogLevel::Critical, context, std::source_location::current(),
        std::forward<Args>(args)...);
#else
    log(LogLevel::Critical, context, "", 0, std::forward<Args>(args)...);
#endif
  }

  // Convenience macros for C++17 compatibility (when source_location not
  // available)
#ifndef REALENGINE_LOGGER_USE_SOURCE_LOCATION
  template <typename... Args>
  static void trace(const std::string &context, const char *file, int line,
                    Args &&...args) {
    log(LogLevel::Trace, context, file, line, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void debug(const std::string &context, const char *file, int line,
                    Args &&...args) {
    log(LogLevel::Debug, context, file, line, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void info(const std::string &context, const char *file, int line,
                   Args &&...args) {
    log(LogLevel::Info, context, file, line, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void warning(const std::string &context, const char *file, int line,
                      Args &&...args) {
    log(LogLevel::Warning, context, file, line, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void error(const std::string &context, const char *file, int line,
                    Args &&...args) {
    log(LogLevel::Error, context, file, line, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void critical(const std::string &context, const char *file, int line,
                       Args &&...args) {
    log(LogLevel::Critical, context, file, line, std::forward<Args>(args)...);
  }
#endif

private:
  // Thread-safe logging implementation
#if defined(REALENGINE_LOGGER_USE_SOURCE_LOCATION)
  template <typename... Args>
  static void log(LogLevel level, const std::string &context,
                  const std::source_location &location, Args &&...args) {
    // Early return if below minimum log level (optimized by compiler in
    // release)
    if (level < minLogLevel_) {
      return;
    }

    std::lock_guard<std::mutex> lock(logMutex_);

    // Double-check after acquiring lock (for thread safety)
    if (level < minLogLevel_) {
      return;
    }

    std::ostringstream stream;
    stream << "[" << getCurrentTime() << "] "
           << "[" << levelToString(level) << "] "
           << "[" << context << "] "
           << "[" << extractFilename(location.file_name()) << ":"
           << location.line() << "] ";

    // Use fold expression to concatenate arguments (C++17)
    (stream << ... << std::forward<Args>(args));

    stream << std::endl;

    // Output to appropriate stream based on severity
    if (level >= LogLevel::Warning) {
      std::cerr << stream.str();
    } else {
      std::cout << stream.str();
    }
  }
#else
  template <typename... Args>
  static void log(LogLevel level, const std::string &context, const char *file,
                  int line, Args &&...args) {
    // Early return if below minimum log level (optimized by compiler in
    // release)
    if (level < minLogLevel_) {
      return;
    }

    std::lock_guard<std::mutex> lock(logMutex_);

    // Double-check after acquiring lock (for thread safety)
    if (level < minLogLevel_) {
      return;
    }

    std::ostringstream stream;
    stream << "[" << getCurrentTime() << "] "
           << "[" << levelToString(level) << "] "
           << "[" << context << "] ";

    // Include file:line if provided (C++17 fallback)
    if (file && file[0] != '\0') {
      stream << "[" << extractFilename(file) << ":" << line << "] ";
    }

    // Use fold expression to concatenate arguments (C++17)
    (stream << ... << std::forward<Args>(args));

    stream << std::endl;

    // Output to appropriate stream based on severity
    if (level >= LogLevel::Warning) {
      std::cerr << stream.str();
    } else {
      std::cout << stream.str();
    }
  }
#endif

  // Format current timestamp with millisecond precision
  static std::string getCurrentTime();

  // Convert log level to string representation
  [[nodiscard]] static const char *levelToString(LogLevel level) noexcept;

  // Extract filename from full path (cross-platform)
  [[nodiscard]] static std::string
  extractFilename(const std::string &path) noexcept;

  // Static members
  static LogLevel minLogLevel_;
  static std::mutex logMutex_;
};

// Macro helpers for C++17 compatibility
#ifndef REALENGINE_LOGGER_USE_SOURCE_LOCATION
#define LOG_TRACE(context, ...)                                                \
  Core::Logger::trace(context, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(context, ...)                                                \
  Core::Logger::debug(context, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(context, ...)                                                 \
  Core::Logger::info(context, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(context, ...)                                              \
  Core::Logger::warning(context, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(context, ...)                                                \
  Core::Logger::error(context, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_CRITICAL(context, ...)                                             \
  Core::Logger::critical(context, __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_TRACE(context, ...) Core::Logger::trace(context, __VA_ARGS__)
#define LOG_DEBUG(context, ...) Core::Logger::debug(context, __VA_ARGS__)
#define LOG_INFO(context, ...) Core::Logger::info(context, __VA_ARGS__)
#define LOG_WARNING(context, ...) Core::Logger::warning(context, __VA_ARGS__)
#define LOG_ERROR(context, ...) Core::Logger::error(context, __VA_ARGS__)
#define LOG_CRITICAL(context, ...) Core::Logger::critical(context, __VA_ARGS__)
#endif

} // namespace Core
