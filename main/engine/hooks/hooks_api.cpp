//
//  hooks_api.cpp
//  Sources for Cherry hook api and end user functions.
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "hooks.hpp"

namespace Cherry {

  void CreateHook(
      const std::string &id,
      ExecuteHookOn execute_on,
      const std::function<void(Hook *)> &hook,
      const std::function<bool()> &condition) {
    if (hooks.find(CherryID(id)) != hooks.end())
      return;
    hooks[CherryID(id)] = std::make_shared<Hook>(CherryID(id), execute_on, hook, condition);
  }

  std::shared_ptr<Hook> GetHook(const Identifier &id) {
    auto it = hooks.find(id);
    if (it != hooks.end())
      return it->second;
    return nullptr;
  }

  std::string GetHookData(const Identifier &id, const std::string &key) {
    auto hook = GetHook(id);
    if (!hook)
      return "";
    return hook->GetData(key);
  }

  void DeleteHook(const Identifier &id) {
    hooks.erase(id);
  }

  void RenderHooks() {
    static std::unordered_map<std::string, std::chrono::steady_clock::time_point> last_exec;
    auto now = std::chrono::steady_clock::now();

    for (auto &[id, hook] : hooks) {
      if (!hook)
        continue;

      switch (hook->GetExecuteOn()) {
        case ExecuteHookOn::Frame: {
          hook->Execute();
          break;
        }

        case ExecuteHookOn::Second: {
          const std::string &key = id.string();
          auto it = last_exec.find(key);

          if (it == last_exec.end()) {
            last_exec[key] = now;
            hook->Execute();
          } else {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second).count();
            if (elapsed >= 1000) {
              it->second = now;
              hook->Execute();
            }
          }
          break;
        }
      }
    }
  }

}  // namespace Cherry