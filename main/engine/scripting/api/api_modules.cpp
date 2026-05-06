//
//  api_modules.cpp
//  Scripting API for Cherry modules features : hooks and component factory for Cherry Lua
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//  This work is licensed under the terms of the MIT license.
//

#include "../scripting.hpp"

#ifdef CHERRY_ENABLE_SCRIPTING
#include <main/engine/app/app.hpp>
#include <main/engine/components/factory.hpp>
#include <main/engine/hooks/hooks.hpp>

#include "../lua_helpers.hpp"

namespace Cherry {
  namespace Script {

    static bool CallLuaBool(lua_State *L, int ref) {
      lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
      if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        const char *err = lua_tostring(L, -1);
        CH_ERROR_TAG("Lua", "condition callback error: %s", err ? err : "?");
        lua_pop(L, 1);
        return false;
      }
      bool result = lua_toboolean(L, -1) != 0;
      lua_pop(L, 1);
      return result;
    }

    static Props LuaTableToProps(lua_State *L, int tbl) {
      Props props;
      if (!lua_istable(L, tbl))
        return props;

      lua_pushnil(L);
      while (lua_next(L, tbl) != 0) {
        if (lua_isstring(L, -2)) {
          std::string key = lua_tostring(L, -2);
          if (lua_isstring(L, -1))
            props[key] = std::string(lua_tostring(L, -1));
          else if (lua_isnumber(L, -1))
            props[key] = (float)lua_tonumber(L, -1);
          else if (lua_isboolean(L, -1))
            props[key] = (bool)(lua_toboolean(L, -1) != 0);
        }
        lua_pop(L, 1);
      }
      return props;
    }

    LUA_FUNC(CreateHook) {
      if (lua_gettop(L) < 3) {
        return lua_error_msg(L, "CreateHook: expected (id, mode, callback [, condition])");
      }

      std::string id = lua_getstring(L, 1);
      std::string mode = lua_getstring(L, 2);

      if (!lua_isfunction(L, 3))
        return lua_error_msg(L, "CreateHook: argument 3 must be a function");

      ExecuteHookOn execOn = ExecuteHookOn::Frame;
      if (mode == "second")
        execOn = ExecuteHookOn::Second;

      lua_pushvalue(L, 3);
      int cbRef = luaL_ref(L, LUA_REGISTRYINDEX);

      int condRef = LUA_NOREF;
      if (lua_gettop(L) >= 4 && lua_isfunction(L, 4)) {
        lua_pushvalue(L, 4);
        condRef = luaL_ref(L, LUA_REGISTRYINDEX);
      }

      std::function<void(Hook *)> hookFn = [L, cbRef](Hook *self) {
        (void)self;
        CallLuaVoid(L, cbRef);
      };

      std::function<bool()> condFn;
      if (condRef != LUA_NOREF) {
        condFn = [L, condRef]() -> bool { return CallLuaBool(L, condRef); };
      } else {
        condFn = []() -> bool { return true; };
      }

      Cherry::CreateHook(id, execOn, hookFn, condFn);
      return 0;
    }

    LUA_FUNC(GetHookData) {
      std::string id = lua_getstring(L, 1);
      std::string key = lua_getstring(L, 2);
      std::string val = Cherry::GetHookData(Cherry::Identifier(id), key);
      lua_pushstring(L, val.c_str());
      return 1;
    }

    LUA_FUNC(SetHookData) {
      std::string id = lua_getstring(L, 1);
      std::string key = lua_getstring(L, 2);
      std::string val = lua_getstring(L, 3);

      auto hook = Cherry::GetHook(Cherry::Identifier(id));
      if (hook)
        hook->SetData(key, val);
      return 0;
    }
    LUA_FUNC(Use) {
      int argc = lua_gettop(L);
      std::string name;
      Props defaults;
      int fnIdx = -1;

      if (argc < 2)
        return lua_error_msg(L, "Use: expected (name, [defaults,] render_fn)");

      name = lua_getstring(L, 1);

      if (argc == 2) {
        if (!lua_isfunction(L, 2))
          return lua_error_msg(L, "Use: argument 2 must be a function");
        fnIdx = 2;
      } else {
        if (lua_istable(L, 2))
          defaults = LuaTableToProps(L, 2);
        if (!lua_isfunction(L, 3))
          return lua_error_msg(L, "Use: argument 3 must be a function");
        fnIdx = 3;
      }

      auto &registry = Cherry::ComponentRegistry::Instance();

      lua_pushvalue(L, fnIdx);
      int ref = luaL_ref(L, LUA_REGISTRYINDEX);

      if (registry.HasLua(name)) {
        registry.UpdateLua(name, L, ref);
      } else {
        registry.RegisterLua(name, L, ref);
      }

      Cherry::ComponentDefaultsRegistry::Instance().SetOnce(name, defaults);

      return 0;
    }

    LUA_FUNC(Draw) {
      std::string name = lua_getstring(L, 1);
      Props props;

      if (lua_gettop(L) >= 2 && lua_istable(L, 2))
        props = LuaTableToProps(L, 2);

      Component &comp = Cherry::Draw(name, props);
      lua_pushstring(L, comp.GetIdentifier().string().c_str());
      return 1;
    }

    LUA_FUNC(ClearHooks) {
      Cherry::ClearHooks();
      return 0;
    }

    LUA_FUNC(ClearComponentsRegistry) {
      Cherry::ClearComponentFactoryRegistry();
      return 0;
    }

    LUA_FUNC(ClearComponentsCache) {
      Cherry::ClearComponentFactoryCache();
      return 0;
    }

    // Dev only ! High costs on ressources if called every frames
    LUA_FUNC(RefreshComponents) {
      Cherry::ClearComponentFactoryCache();
      Cherry::ClearComponentFactoryRegistry();
      CherryApp.ClearComponentPoolStack();
      CherryApp.ClearComponentPool();
      return 0;
    }

    void RegisterHooksAndComponentsAPI(lua_State *L) {
      // Hooks
      LUA_REGISTER(L, -1, CreateHook);
      LUA_REGISTER(L, -1, GetHookData);
      LUA_REGISTER(L, -1, SetHookData);
      LUA_REGISTER(L, -1, ClearHooks);

      // Components
      LUA_REGISTER(L, -1, Use);
      LUA_REGISTER(L, -1, Draw);
      LUA_REGISTER(L, -1, ClearComponentsRegistry);
      LUA_REGISTER(L, -1, ClearComponentsCache);
      LUA_REGISTER(L, -1, RefreshComponents);
    }

  }  // namespace Script
}  // namespace Cherry
#endif  // CHERRY_ENABLE_SCRIPTING