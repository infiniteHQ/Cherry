#include "components.hpp"
#include "../app/app.hpp"

namespace Cherry {
Component::Component(const Cherry::Identifier &id) : m_Identifier(id) {};
Component::Component() {};

std::string Component::SetData(const std::string &key, const std::string &val) {
  m_Data[key] = val;
  return val;
}

std::string Component::SetData(const std::string &key, const char *val) {
  return SetData(key, std::string(val));
}

std::string Component::SetData(const std::string &key, int val) {
  std::string strVal = std::to_string(val);
  m_Data[key] = strVal;
  return strVal;
}

std::string Component::SetData(const std::string &key, float val) {
  std::string strVal = std::to_string(val) + "f";
  m_Data[key] = strVal;
  return strVal;
}

std::string Component::SetData(const std::string &key, double val) {
  std::string strVal = std::to_string(val);
  m_Data[key] = strVal;
  return strVal;
}

std::string Component::SetData(const std::string &key, bool val) {
  std::string strVal = val ? "true" : "false";
  m_Data[key] = strVal;
  return strVal;
}

std::string Component::GetData(const std::string &key) {
  std::string value = "undefined";

  auto ctxIt = m_ContextData.find(key);
  if (ctxIt != m_ContextData.end()) {
    value = ctxIt->second;

  } else {
    auto it = m_Data.find(key);
    if (it != m_Data.end()) {
      value = it->second;

    } else {
      value = "undefined";
    }
  }

  return value;
}

template <>
std::string Component::GetDataAs<std::string>(const std::string &key) {
  return GetData(key);
}

template <> int Component::GetDataAs<int>(const std::string &key) {
  return std::stoi(GetData(key));
}

template <> float Component::GetDataAs<float>(const std::string &key) {
  std::string val = GetData(key);
  if (!val.empty() && val.back() == 'f')
    val.pop_back();
  return std::stof(val);
}

template <> double Component::GetDataAs<double>(const std::string &key) {
  return std::stod(GetData(key));
}

template <> bool Component::GetDataAs<bool>(const std::string &key) {
  std::string val = GetData(key);
  std::transform(val.begin(), val.end(), val.begin(), ::tolower);
  return val == "true";
}

void Component::ClearData(const std::string &key) { m_Data.erase(key); }

std::string Component::SetProperty(const std::string &key,
                                   const std::string &val) {
  m_Properties[key] = val;
  return val;
}

std::string Component::SetProperty(const std::string &key, const char *val) {
  return SetProperty(key, std::string(val));
}

std::string Component::SetProperty(const std::string &key, float val) {
  std::string strVal = std::to_string(val) + "f";
  m_Properties[key] = strVal;
  return strVal;
}

std::string Component::SetProperty(const std::string &key, int val) {
  std::string strVal = std::to_string(val);
  m_Properties[key] = strVal;
  return strVal;
}

std::string Component::SetProperty(const std::string &key, double val) {
  std::string strVal = std::to_string(val);
  m_Properties[key] = strVal;
  return strVal;
}

std::string Component::SetProperty(const std::string &key, bool val) {
  std::cout << key << val << std::endl;
  std::string strVal = val ? "true" : "false";
  m_Properties[key] = strVal;
  return strVal;
}

std::string Component::GetProperty(const std::string &key) {
  const std::string prefix = "theme:";
  std::string value = "undefined";

  auto ctxIt = m_ContextProperties.find(key);
  if (ctxIt != m_ContextProperties.end()) {
    value = ctxIt->second;
  } else {
    auto it = m_Properties.find(key);
    if (it != m_Properties.end()) {
      value = it->second;

    } else {
      value = "undefined";
    }
  }

  if (value.rfind(prefix, 0) == 0) {
    std::string themeKey = value.substr(prefix.length());
    value = CherryApp.GetActiveThemeProperty(themeKey);
  }

  return value;
}

template <>
std::string Component::GetPropertyAs<std::string>(const std::string &key) {
  return GetProperty(key);
}

template <> int Component::GetPropertyAs<int>(const std::string &key) {
  return std::stoi(GetProperty(key));
}

template <> float Component::GetPropertyAs<float>(const std::string &key) {
  std::string val = GetProperty(key);
  if (!val.empty() && val.back() == 'f')
    val.pop_back();
  return std::stof(val);
}

template <> double Component::GetPropertyAs<double>(const std::string &key) {
  return std::stod(GetProperty(key));
}

template <> bool Component::GetPropertyAs<bool>(const std::string &key) {
  std::string val = GetProperty(key);
  std::transform(val.begin(), val.end(), val.begin(), ::tolower);
  return val == "true";
}

void Component::RenderWrapper() {
  m_IsComponentRendered = true;
  // Pre process actions
  RefreshContextProperties();

  // Render
  CherryApp.SetCurrentComponent(this);
  this->Render();
  CherryApp.ResetCurrentComponent();

  // Post process actions
  CherryApp.SetLastComponent(this);
  CherryApp.ResetNextComponent();
}

void Component::ClearProperty(const std::string &key) {
  m_Properties.erase(key);
}

void Component::RefreshContextProperties() {
  m_ContextProperties.clear();

  // Insert all one time props
  for (const auto &pair : Application::Get().m_OnTimeProperties) {
    m_ContextProperties.emplace(pair);
  }
  Application::Get()
      .m_OnTimeProperties.clear(); // One time properties consumed.

  // Insert all permanent props
  for (const auto &pair : Application::Get().m_PermanentProperties) {
    m_ContextProperties.emplace(pair);
  }

  // Insert all permanent props
  for (const auto &pair : CherryNextComponent.m_Properties) {
    m_ContextProperties.emplace(pair);
  }

  for (const auto &pair : CherryNextComponent.m_Data) {
    m_ContextData.emplace(pair);
  }
}

const std::string &Component::GetType() const { return m_ComponentType; }

void Component::SetType(const std::string &type) { m_ComponentType = type; }

void Component::SetIdentifier(const Cherry::Identifier &id) {
  m_Identifier = id;
}

const Identifier &Component::GetIdentifier() const { return m_Identifier; }

} // namespace Cherry
