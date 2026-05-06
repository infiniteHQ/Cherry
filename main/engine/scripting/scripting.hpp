//
//  scripting.hpp
//  Headers for scripting engine of Cherry (in LUA)
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include <main/engine/app/app.hpp>
#ifdef CHERRY_ENABLE_SCRIPTING

#include <functional>
#include <string>
#include <vector>

#include "lua_helpers.hpp"

namespace Cherry {
  namespace Script {

    class ScriptingEngine {
     public:
      ScriptingEngine();
      ~ScriptingEngine();

      bool Execute(const std::string &code);
      int LoadFile(const std::string &path, int nargs = 0);
      void ReloadFile(const std::string &path);

      static int InternalRenderScript(const std::string &path, bool fresh, int nargs);

      std::string GetLastOutput() const;
      std::string GetLastError() const;
      const std::vector<std::string> &GetOutputHistory() const;
      void ClearOutput();

      void EnableHotReload(const std::string &scriptDir);
      void CheckForChanges();

      void RegisterCherryAPI();

      lua_State *GetState();

     private:
      lua_State *L;
      std::string m_LastError;
      std::vector<std::string> m_OutputHistory;

      bool m_HotReloadEnabled = false;
      std::string m_ScriptDir;
      std::unordered_map<std::string, long long> m_FileTimestamps;

      void CaptureOutput(const std::string &text);
      static int LuaPrint(lua_State *L);  // Override print()
    };

    // API features (Cherry::Script)
    CHERRY_API ScriptingEngine &GetScriptingEngine();
    CHERRY_API void RenderLuaScript(const std::string &lua_file_path);
    CHERRY_API void RenderLuaFreshScript(const std::string &lua_file_path);

    void RegisterDrawingAPI(lua_State *L);
    void RegisterLogicAPI(lua_State *L);
    void RegisterHooksAndComponentsAPI(lua_State *L);

  }  // namespace Script

  namespace Components {
    template<typename T>
    class RegistrableComponent : public Component {
     public:
      using Component::Component;

      static void Register(lua_State *L) {
        T::RegisterInternal(L);
      }
    };
  }  // namespace Components

}  // namespace Cherry

#else
namespace Cherry {
  namespace Script { }
}  // namespace Cherry
#endif  // CHERRY_ENABLE_SCRIPTING