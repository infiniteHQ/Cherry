#include "themes.hpp"
#include "../app/app.hpp"

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

void Theme::SetName(const std::string &name) { m_ThemeID = name; }
const std::string Theme::GetName() const { return m_ThemeID; }

} // namespace Cherry
