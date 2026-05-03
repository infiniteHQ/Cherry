//
//  type_safety.hpp
//  Headers for safety measures while using cpp types on Cherry components or internal structure
//  where types are unsafe.
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

namespace Cherry {
  inline int SafeInt(const std::string& s, int def = 0) {
    if (s.empty())
      return def;
    try {
      return std::stoi(s);
    } catch (...) {
      return def;
    }
  }
  inline float SafeFloat(const std::string& s, float def = 0.f) {
    if (s.empty())
      return def;
    try {
      return std::stof(s);
    } catch (...) {
      return def;
    }
  }
  inline bool SafeBool(const std::string& s, bool def = false) {
    if (s == "true")
      return true;
    if (s == "false")
      return false;
    return def;
  }

}  // namespace Cherry