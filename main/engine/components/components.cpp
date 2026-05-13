//
//  components.cpp
//  Sources for Cherry components (for UI objects and OOP GUIs)
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "components.hpp"

#include "../app/app.hpp"

namespace Cherry {

  Component::Component(const Cherry::Identifier &id) : m_Identifier(id) { };

  Component::Component() { };

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

  void Component::ClearData(const std::string &key) {
    m_Data.erase(key);
  }

  std::string Component::SetProperty(const std::string &key, const std::string &val) {
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

  void Component::RenderWrapper() {
    m_IsComponentRendered = true;
    // Pre process actions
    RefreshContextProperties();

    m_HaveParent = false;
    auto p = Cherry::GetParent();
    if (p) {
      SetParentIdentifier(p->GetIdentifier());
      SetParentType(p->GetType());
      m_HaveParent = true;
    }

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
    Application::Get().m_OnTimeProperties.clear();  // One time properties consumed.

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

  const RenderMode &Component::GetRenderMode() const {
    return m_RenderMode;
  }

  const bool &Component::GetIsComponentRendered() const {
    return m_IsComponentRendered;
  }

  void Component::SetIsComponentRendered(const bool &value) {
    m_IsComponentRendered = value;
  }

  void Component::SetRenderMode(RenderMode prop) {
    m_RenderMode = prop;
  }

  const std::unordered_map<std::string, std::string> &Component::GetPropertiesMap() const {
    return m_Properties;
  }

  const std::unordered_map<std::string, std::string> &Component::GetDataMap() const {
    return m_Data;
  }

  const std::unordered_map<std::string, std::string> &Component::GetContextPropertiesMap() const {
    return m_ContextProperties;
  }

  const std::unordered_map<std::string, std::string> &Component::GetContextDataMap() const {
    return m_ContextData;
  }

  const std::string &Component::GetType() const {
    return m_ComponentType;
  }

  void Component::SetType(const std::string &type) {
    m_ComponentType = type;
  }

  const std::string &Component::GetParentType() const {
    return m_ParentComponentType;
  }

  void Component::SetParentType(const std::string &type) {
    m_ParentComponentType = type;
  }

  void Component::SetIdentifier(const Cherry::Identifier &id) {
    m_Identifier = id;
  }

  void Component::SetParentIdentifier(const Identifier &id) {
    m_ParentIdentifier = id;
  }

  const Identifier &Component::GetIdentifier() const {
    return m_Identifier;
  }

  const bool &Component::HaveParent() const {
    return m_HaveParent;
  }

  const Identifier &Component::GetParentIdentifier() const {
    return m_ParentIdentifier;
  }
}  // namespace Cherry
