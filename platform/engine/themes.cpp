#include "themes.hpp"
#include "app.hpp"

namespace Cherry {
Theme::Theme() {};

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

} // namespace Cherry
