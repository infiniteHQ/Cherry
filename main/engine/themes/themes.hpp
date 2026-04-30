#pragma once

#include "../../../src/core/color.hpp"
#include "../../../src/layer.hpp"
#include "../options.hpp"

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
} // namespace Cherry

#endif // CHERRY_THEME_H