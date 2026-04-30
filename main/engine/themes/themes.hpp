//
//  themes.hpp
//  Headers for Cherry themes features
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include <main/core/color/color.hpp>
#include <main/engine/options.hpp>
#include <mutex>
#include <string>
#include <unordered_map>

#ifndef CHERRY_THEME_H
#define CHERRY_THEME_H

namespace Cherry {
  class CHERRY_API Theme {
   public:
    Theme();

    // Theme Properties
    std::string SetProperty(const std::string &key, const std::string &val);
    std::string GetProperty(const std::string &key);

    void SetName(const std::string &name);
    const std::string GetName() const;

   private:
    std::string m_ThemeID;
    std::unordered_map<std::string, std::string> m_Properties;
  };
}  // namespace Cherry

#endif  // CHERRY_THEME_H