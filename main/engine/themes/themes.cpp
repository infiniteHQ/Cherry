//
//  themes.cpp
//  Sources for Cherry themes features
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "themes.hpp"

#include <main/engine/app/app.hpp>

namespace Cherry {
  Theme::Theme() { };

  std::string Theme::SetProperty(const std::string &key, const std::string &val) {
    m_Properties[key] = val;
    return val;
  }

  std::string Theme::GetProperty(const std::string &key) {
    auto it = m_Properties.find(key);
    if (it != m_Properties.end()) {
      return it->second;
    } else {
      return "undefined";
    }
  }

  void Theme::SetName(const std::string &name) {
    m_ThemeID = name;
  }
  const std::string Theme::GetName() const {
    return m_ThemeID;
  }

}  // namespace Cherry
