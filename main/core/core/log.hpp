#pragma once

#include <chrono>
#include <cstdarg>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace Cherry {

class Log {
public:
  enum class Level { Trace = 0, Info, Warn, Error, Fatal };

  static void SetLevel(Level level) { s_FilterLevel = level; }

  using LogCallback =
      std::function<void(Level level, const std::string &message)>;
  static void AddCallback(LogCallback cb) { s_Callbacks.push_back(cb); }

  static void Print(Level level, std::string_view tag, const char *fmt, ...) {
    if (level < s_FilterLevel)
      return;

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) %
              1000;
    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;

#ifdef _WIN32
    localtime_s(&local_tm, &time_t_now);
#else
    localtime_r(&time_t_now, &local_tm);
#endif

    char timeBuffer[25];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S",
                  &local_tm);

    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);
    int size = std::vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);

    std::string message;
    if (size > 0) {
      message.resize(size);
      std::vsnprintf(&message[0], size + 1, fmt, args);
    }
    va_end(args);

    const char *color = GetLevelColor(level);
    const char *reset = "\033[0m";
    std::ostream &out = (level >= Level::Error) ? std::cerr : std::cout;

    out << "[" << timeBuffer << "." << std::setfill('0') << std::setw(3)
        << ms.count() << "] " << color << "[" << LevelToString(level) << "]"
        << reset << " ";

    if (!tag.empty()) {
      out << "[" << tag << "] ";
    }

    out << message << std::endl;
    for (auto &cb : s_Callbacks) {
      cb(level, message);
    }
  }

private:
  inline static std::vector<LogCallback> s_Callbacks;

  static constexpr const char *LevelToString(Level level) {
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
    default:
      return "UNKNOWN";
    }
  }

  static constexpr const char *GetLevelColor(Level level) {
    switch (level) {
    case Level::Trace:
      return "\033[90m";
    case Level::Info:
      return "\033[32m";
    case Level::Warn:
      return "\033[33m";
    case Level::Error:
      return "\033[31m";
    case Level::Fatal:
      return "\033[41m\033[37m";
    default:
      return "\033[0m";
    }
  }

  inline static Level s_FilterLevel = Level::Trace;
};

} // namespace Cherry

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

#define CHI(...) CH_INFO(__VA_ARGS__)