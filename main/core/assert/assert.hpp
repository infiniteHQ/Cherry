//
//  assert.hpp
//  Headers for assertion system of Cherry
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once
#include <cstdio>
#include <stdexcept>
#include <string>

#ifndef NDEBUG
#define CHERRY_ASSERT(condition, message)      \
  do {                                         \
    if (!(condition)) {                        \
      std::fprintf(                            \
          stderr,                              \
          "[Cherry] Assertion failed\n"        \
          "  Condition : %s\n"                 \
          "  Message   : %s\n"                 \
          "  Location  : %s:%d (%s)\n",        \
          #condition,                          \
          (message).c_str(),                   \
          __FILE__,                            \
          __LINE__,                            \
          __func__);                           \
      std::fflush(stderr);                     \
      throw ::Cherry::AssertionError(message); \
    }                                          \
  } while (0)
#else

#define CHERRY_ASSERT(condition, message)                           \
  do {                                                              \
    if (!(condition)) {                                             \
      ::Cherry::detail::AssertHandler(message, __FILE__, __LINE__); \
    }                                                               \
  } while (0)
#endif

namespace Cherry {

  struct AssertionError : std::runtime_error {
    explicit AssertionError(const std::string& msg) : std::runtime_error("[Cherry] " + msg) {
    }
  };

  namespace detail {
    inline void AssertHandler(const std::string& message, const char* file, int line) {
      std::fprintf(stderr, "[Cherry] Runtime assertion (release): %s  [%s:%d]\n", message.c_str(), file, line);
      std::fflush(stderr);
    }
  }  // namespace detail

}  // namespace Cherry