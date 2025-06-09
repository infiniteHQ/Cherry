#pragma once

#include "../../src/core/color.hpp"
#include "../../src/layer.hpp"

#include <mutex>
#include <string>
#include <unordered_map>

#ifndef CHERRY_THEME_H
#define CHERRY_THEME_H

namespace Cherry {
class Theme {
public:
  Theme();

  // Theme Properties
  std::string SetProperty(const std::string &key, const std::string &val);
  std::string GetProperty(const std::string &key);

  std::string m_ThemeID;

private:
  std::unordered_map<std::string, std::string> m_Properties;
};
} // namespace Cherry

#endif // CHERRY_THEME_H