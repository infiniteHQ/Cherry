//
//  hooks.hpp
//  Headers for Cherry hook system
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once
#include <main/core/color/color.hpp>
#include <main/core/logs/log.hpp>
#include <main/engine/identifier/identifier.hpp>
#include <main/engine/options.hpp>
#include <unordered_map>

#ifndef HOOKS_HPP
#define HOOKS_HPP

#include "hooks_helpers.hpp"

namespace Cherry {

  class Hook {
   public:
    Hook(
        const Identifier &id,
        const ExecuteHookOn &execute_on,
        const std::function<void(Hook *)> &hook,
        const std::function<bool()> &condition);

    const Identifier &GetIdentifier() const;
    void SetIdentifier(const Identifier &id);

    void SetHook(const std::function<void(Hook *)> function);
    void SetCondition(const std::function<bool()> condition);

    void Execute();

    // Datas
    std::string SetData(const std::string &key, const std::string &val);
    std::string SetData(const std::string &key, const char *val);
    std::string SetData(const std::string &key, int val);
    std::string SetData(const std::string &key, float val);
    std::string SetData(const std::string &key, double val);
    std::string SetData(const std::string &key, bool val);
    void ClearData(const std::string &key);
    std::string GetData(const std::string &key);
    const std::unordered_map<std::string, std::string> &GetDataMap() const;
    template<typename T>
    T GetDataAs(const std::string &key);
    ExecuteHookOn GetExecuteOn() const;

   private:
    Identifier m_Identifier;
    std::function<bool()> m_ConditionEvaluation;
    std::function<void(Hook *)> m_HookFunction;
    ExecuteHookOn m_ExecuteHookOn;
    std::unordered_map<std::string, std::string> m_Data;
  };

  // Templates utils for data and properties of hooks
  template<>
  inline std::string Hook::GetDataAs<std::string>(const std::string &key) {
    return GetData(key);
  }

  template<>
  inline int Hook::GetDataAs<int>(const std::string &key) {
    return std::stoi(GetData(key));
  }

  template<>
  inline float Hook::GetDataAs<float>(const std::string &key) {
    std::string val = GetData(key);
    if (!val.empty() && val.back() == 'f')
      val.pop_back();
    return std::stof(val);
  }

  template<>
  inline double Hook::GetDataAs<double>(const std::string &key) {
    return std::stod(GetData(key));
  }

  template<>
  inline bool Hook::GetDataAs<bool>(const std::string &key) {
    std::string val = GetData(key);
    std::transform(val.begin(), val.end(), val.begin(), ::tolower);
    return val == "true";
  }

  static std::unordered_map<Identifier, std::shared_ptr<Hook>> hooks;
  void CreateHook(
      const std::string &id,
      ExecuteHookOn execute_on,
      const std::function<void(Hook *)> &hook,
      const std::function<bool()> &condition = nullptr);

  std::shared_ptr<Hook> GetHook(const Identifier &id);

  std::string GetHookData(const Identifier &id, const std::string &key);
  void DeleteHook(const Identifier &id);

  void RenderHooks();  // Called each frames

#define CherryHook(code)     [](Cherry::Hook *self) { code }
#define CherryHookCond(code) []() { code }
#define CherryHookFrame      Cherry::ExecuteHookOn::Frame
#define CherryHookSecond     Cherry::ExecuteHookOn::Second

}  // namespace Cherry

#endif  // HOOKS_HPP