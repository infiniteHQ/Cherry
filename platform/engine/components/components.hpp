#pragma once

#include "../options.hpp"
#include "../../../src/core/color.hpp"
#include "../../../src/layer.hpp"

#include <mutex>
#include <string>
#include <unordered_map>

#ifndef CHERRY_COMPONENT_H
#define CHERRY_COMPONENT_H

#include "../identifier/identifier.hpp"

namespace Cherry {

class CHERRY_API Component {
public:
  Component(const Identifier &id);
  Component();

  // Properties
  std::string SetProperty(const std::string &key, const std::string &val);
  std::string SetProperty(const std::string &key, const char *val);
  std::string SetProperty(const std::string &key, float val);
  std::string SetProperty(const std::string &key, int val);
  std::string SetProperty(const std::string &key, double val);
  std::string SetProperty(const std::string &key, bool val);
  std::string GetProperty(const std::string &key);
  template <typename T> T GetPropertyAs(const std::string &key);

  // Datas
  std::string SetData(const std::string &key, const std::string &val);
  std::string SetData(const std::string &key, const char *val);
  std::string SetData(const std::string &key, int val);
  std::string SetData(const std::string &key, float val);
  std::string SetData(const std::string &key, double val);
  std::string SetData(const std::string &key, bool val);
  std::string GetData(const std::string &key);
  template <typename T> T GetDataAs(const std::string &key);

  void ClearProperty(const std::string &key);
  void ClearData(const std::string &key);

  void RefreshContextProperties();

  // Identifier
  void SetIdentifier(const Identifier &id);
  const Identifier &GetIdentifier() const;

  const std::string &GetType() const;
  void SetType(const std::string &type);

  void RenderWrapper();

  RenderMode GetRenderMode() { return m_RenderMode; }

  void SetRenderMode(RenderMode prop) { m_RenderMode = prop; }

  virtual void Render() {};
  virtual void Refresh() {};

  bool m_IsComponentRendered = false;

private:
  Identifier m_Identifier;
  RenderMode m_RenderMode = RenderMode::None;

  std::string m_ComponentType =
      "undefined"; // Optionnal, some parent components can ask for a specific
                   // type (like NodeArea and Nodes), this type member can help
                   // to introduce a type safe component management
  std::unordered_map<std::string, std::string>
      m_ContextProperties; // Cpy of props registered in the cherry context
                           // (from permanent or ontime), first choice
  std::unordered_map<std::string, std::string>
      m_Properties; // Cpy of props registered in the component, second choice
  std::unordered_map<std::string, std::string> m_ContextData;
  std::unordered_map<std::string, std::string> m_Data;

  std::unordered_map<std::string, std::string> m_CachedProperties;
};


template <>
inline std::string Component::GetDataAs<std::string>(const std::string &key) {
    return GetData(key);
}

template <>
inline int Component::GetDataAs<int>(const std::string &key) {
    return std::stoi(GetData(key));
}

template <>
inline float Component::GetDataAs<float>(const std::string &key) {
    std::string val = GetData(key);
    if (!val.empty() && val.back() == 'f')
        val.pop_back();
    return std::stof(val);
}

template <>
inline double Component::GetDataAs<double>(const std::string &key) {
    return std::stod(GetData(key));
}

template <>
inline bool Component::GetDataAs<bool>(const std::string &key) {
    std::string val = GetData(key);
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    return val == "true";
}

template <>
inline std::string Component::GetPropertyAs<std::string>(const std::string &key) {
  return GetProperty(key);
}

template <> inline int Component::GetPropertyAs<int>(const std::string &key) {
  return std::stoi(GetProperty(key));
}

template <> inline float Component::GetPropertyAs<float>(const std::string &key) {
  std::string val = GetProperty(key);
  if (!val.empty() && val.back() == 'f')
    val.pop_back();
  return std::stof(val);
}

template <> inline double Component::GetPropertyAs<double>(const std::string &key) {
  return std::stod(GetProperty(key));
}

template <> inline bool Component::GetPropertyAs<bool>(const std::string &key) {
  std::string val = GetProperty(key);
  std::transform(val.begin(), val.end(), val.begin(), ::tolower);
  return val == "true";
}
} // namespace Cherry

#endif // CHERRY_COMPONENT_H