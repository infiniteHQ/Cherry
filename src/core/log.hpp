#pragma once

#include <array>
#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

namespace Cherry {

class Log {
public:
  enum class Level { Trace = 0, Info, Warn, Error, Fatal };

  static void SetLevel(Level level) { s_FilterLevel = level; }

  template <typename... Args>
  static void Print(Level level, std::string_view tag, std::string_view fmt,
                    Args &&...args) {
    if (level < s_FilterLevel)
      return;

    try {
      auto now = std::chrono::system_clock::now();
      auto time = std::chrono::current_zone()->to_local(now);

      // Format the user message first
      std::string message = std::vformat(fmt, std::make_format_args(args...));
      std::string logLine;

      if (tag.empty()) {
        logLine = std::format("[{:%T}] [{}] {}\n", time, LevelToString(level),
                              message);
      } else {
        logLine = std::format("[{:%T}] [{}] [{}] {}\n", time,
                              LevelToString(level), tag, message);
      }

      if (level >= Level::Error) {
        std::cerr << logLine;
      } else {
        std::cout << logLine;
      }
    } catch (const std::exception &e) {
      std::cerr << "Logging failed: " << e.what() << std::endl;
    }
  }

private:
  static const char *LevelToString(Level level) {
    switch (level) {
    case Level::Trace:
      return "TRACE";
    case Level::Info:
      return "INFO";
    case Level::Warn:
      return "WARN";
    case Level::Error:
      return "ERROR";
    case Level::Fatal:
      return "FATAL";
    }
    return "UNKNOWN";
  }

  inline static Level s_FilterLevel = Level::Trace;
};
} // namespace Cherry

// Simple logs
#define CH_TRACE(...)                                                          \
  ::Cherry::Log::Print(::Cherry::Log::Level::Trace, "", __VA_ARGS__)
#define CH_INFO(...)                                                           \
  ::Cherry::Log::Print(::Cherry::Log::Level::Info, "", __VA_ARGS__)
#define CH_WARN(...)                                                           \
  ::Cherry::Log::Print(::Cherry::Log::Level::Warn, "", __VA_ARGS__)
#define CH_ERROR(...)                                                          \
  ::Cherry::Log::Print(::Cherry::Log::Level::Error, "", __VA_ARGS__)
#define CH_FATAL(...)                                                          \
  ::Cherry::Log::Print(::Cherry::Log::Level::Fatal, "", __VA_ARGS__)

// Tagged logs
#define CH_TRACE_TAG(tag, ...)                                                 \
  ::Cherry::Log::Print(::Cherry::Log::Level::Trace, tag, __VA_ARGS__)
#define CH_INFO_TAG(tag, ...)                                                  \
  ::Cherry::Log::Print(::Cherry::Log::Level::Info, tag, __VA_ARGS__)
#define CH_WARN_TAG(tag, ...)                                                  \
  ::Cherry::Log::Print(::Cherry::Log::Level::Warn, tag, __VA_ARGS__)
#define CH_ERROR_TAG(tag, ...)                                                 \
  ::Cherry::Log::Print(::Cherry::Log::Level::Error, tag, __VA_ARGS__)
#define CH_FATAL_TAG(tag, ...)                                                 \
  ::Cherry::Log::Print(::Cherry::Log::Level::Fatal, tag, __VA_ARGS__)