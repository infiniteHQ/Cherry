//
//  components.hpp
//  Headers for Cherry components (for UI objects OOP GUIs)
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

#ifndef CHERRY_COMPONENT_H
#define CHERRY_COMPONENT_H

#include <main/engine/identifier/identifier.hpp>

namespace Cherry {

  class CHERRY_API Component {
   public:
    Component(const Identifier &id);
    Component();

    // Main setters
    void SetType(const std::string &type);
    void SetIdentifier(const Identifier &id);
    void SetRenderMode(RenderMode prop);
    void SetIsComponentRendered(const bool &value);

    // Main getters
    const Identifier &GetIdentifier() const;
    const std::string &GetType() const;
    const std::unordered_map<std::string, std::string> &GetPropertiesMap() const;
    const std::unordered_map<std::string, std::string> &GetDataMap() const;
    const std::unordered_map<std::string, std::string> &GetContextPropertiesMap() const;
    const std::unordered_map<std::string, std::string> &GetContextDataMap() const;
    const RenderMode &GetRenderMode() const;
    const bool &GetIsComponentRendered() const;

    // Utils
    void ClearProperty(const std::string &key);
    void ClearData(const std::string &key);

    // Properties
    std::string SetProperty(const std::string &key, const std::string &val);
    std::string SetProperty(const std::string &key, const char *val);
    std::string SetProperty(const std::string &key, float val);
    std::string SetProperty(const std::string &key, int val);
    std::string SetProperty(const std::string &key, double val);
    std::string SetProperty(const std::string &key, bool val);
    std::string GetProperty(const std::string &key);
    template<typename T>
    T GetPropertyAs(const std::string &key);

    // Datas
    std::string SetData(const std::string &key, const std::string &val);
    std::string SetData(const std::string &key, const char *val);
    std::string SetData(const std::string &key, int val);
    std::string SetData(const std::string &key, float val);
    std::string SetData(const std::string &key, double val);
    std::string SetData(const std::string &key, bool val);
    std::string GetData(const std::string &key);
    template<typename T>
    T GetDataAs(const std::string &key);

    void RefreshContextProperties();
    void RenderWrapper();

    // Overrideable functions
    virtual void Render() { };
    virtual void Refresh() { };

   private:
    // Identification
    Identifier m_Identifier;
    RenderMode m_RenderMode = RenderMode::None;

    // Optionnal, some parent components can ask for a specific
    // type (like NodeArea and Nodes), this type member can help
    // to introduce a type safe component management
    std::string m_ComponentType = "undefined";

    // States
    bool m_IsComponentRendered = false;

    // Properties
    // Cpy of props registered in the cherry context
    // (from permanent or ontime), first choice
    std::unordered_map<std::string, std::string> m_ContextProperties;
    // Cpy of props registered in the component, second choice
    std::unordered_map<std::string, std::string> m_Properties;
    std::unordered_map<std::string, std::string> m_ContextData;
    std::unordered_map<std::string, std::string> m_Data;
    std::unordered_map<std::string, std::string> m_CachedProperties;
  };

  // Templates utils for data and properties of components
  template<>
  inline std::string Component::GetDataAs<std::string>(const std::string &key) {
    return GetData(key);
  }

  template<>
  inline int Component::GetDataAs<int>(const std::string &key) {
    return std::stoi(GetData(key));
  }

  template<>
  inline float Component::GetDataAs<float>(const std::string &key) {
    std::string val = GetData(key);
    if (!val.empty() && val.back() == 'f')
      val.pop_back();
    return std::stof(val);
  }

  template<>
  inline double Component::GetDataAs<double>(const std::string &key) {
    return std::stod(GetData(key));
  }

  template<>
  inline bool Component::GetDataAs<bool>(const std::string &key) {
    std::string val = GetData(key);
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    return val == "true";
  }

  template<>
  inline std::string Component::GetPropertyAs<std::string>(const std::string &key) {
    return GetProperty(key);
  }

  template<>
  inline int Component::GetPropertyAs<int>(const std::string &key) {
    return std::stoi(GetProperty(key));
  }

  template<>
  inline float Component::GetPropertyAs<float>(const std::string &key) {
    std::string val = GetProperty(key);
    if (!val.empty() && val.back() == 'f')
      val.pop_back();
    return std::stof(val);
  }

  template<>
  inline double Component::GetPropertyAs<double>(const std::string &key) {
    return std::stod(GetProperty(key));
  }

  template<>
  inline bool Component::GetPropertyAs<bool>(const std::string &key) {
    std::string val = GetProperty(key);
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    return val == "true";
  }

}  // namespace Cherry

#endif  // CHERRY_COMPONENT_H