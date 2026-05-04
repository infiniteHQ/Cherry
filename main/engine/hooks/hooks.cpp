//
//  hooks.cpp
//  Sources for Cherry hook
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "hooks.hpp"

namespace Cherry {

  Hook::Hook(
      const Identifier &id,
      const ExecuteHookOn &execute_on,
      const std::function<void(Hook *)> &hook,
      const std::function<bool()> &condition)
      : m_Identifier(id),
        m_ExecuteHookOn(execute_on),
        m_HookFunction(hook),
        m_ConditionEvaluation(condition) {
    //
  }

  void Hook::SetHook(const std::function<void(Hook *)> function) {
    m_HookFunction = function;
  }

  void Hook::SetCondition(const std::function<bool()> condition) {
    m_ConditionEvaluation = condition;
  }

  std::string Hook::SetData(const std::string &key, const std::string &val) {
    m_Data[key] = val;
    return val;
  }

  std::string Hook::SetData(const std::string &key, const char *val) {
    return SetData(key, std::string(val));
  }

  std::string Hook::SetData(const std::string &key, int val) {
    std::string strVal = std::to_string(val);
    m_Data[key] = strVal;
    return strVal;
  }

  std::string Hook::SetData(const std::string &key, float val) {
    std::string strVal = std::to_string(val) + "f";
    m_Data[key] = strVal;
    return strVal;
  }

  std::string Hook::SetData(const std::string &key, double val) {
    std::string strVal = std::to_string(val);
    m_Data[key] = strVal;
    return strVal;
  }

  std::string Hook::SetData(const std::string &key, bool val) {
    std::string strVal = val ? "true" : "false";
    m_Data[key] = strVal;
    return strVal;
  }

  std::string Hook::GetData(const std::string &key) {
    std::string value = "undefined";

    auto it = m_Data.find(key);
    if (it != m_Data.end()) {
      value = it->second;

    } else {
      value = "undefined";
    }

    return value;
  }

  void Hook::ClearData(const std::string &key) {
    m_Data.erase(key);
  }

  const std::unordered_map<std::string, std::string> &Hook::GetDataMap() const {
    return m_Data;
  }

  void Hook::SetIdentifier(const Cherry::Identifier &id) {
    m_Identifier = id;
  }

  const Identifier &Hook::GetIdentifier() const {
    return m_Identifier;
  }

  ExecuteHookOn Hook::GetExecuteOn() const {
    return m_ExecuteHookOn;
  }

  void Hook::Execute() {
    if (m_ConditionEvaluation && !m_ConditionEvaluation()) {
      return;
    }
    if (m_HookFunction) {
      m_HookFunction(this);
    }
  }
}  // namespace Cherry
