//
//  lua_helpers.hpp
//  Some helpers for the Lua scripting system of Cherry
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once
#include <string>

#ifdef CHERRY_ENABLE_SCRIPTING
#include <lib/lua/src/lua.hpp>

#define LUA_FUNC(name) static int l_##name(lua_State *L)

#define LUA_REGISTER(L, table, name) \
  lua_pushcfunction(L, l_##name);    \
  lua_setfield(L, -2, #name)

#define VXLUA_REGISTER_AS(L, func, lua_name) \
  lua_pushcfunction(L, l_##func);            \
  lua_setfield(L, -2, lua_name)

#define LUA_REGISTER_GLOBAL_FUNC(global, name)                      \
  do {                                                              \
    lua_State *L = Cherry::Script::GetScriptingEngine().GetState(); \
                                                                    \
    lua_getglobal(L, global);                                       \
    if (lua_isnil(L, -1)) {                                         \
      lua_pop(L, 1);                                                \
      lua_newtable(L);                                              \
      lua_setglobal(L, global);                                     \
      lua_getglobal(L, global);                                     \
    }                                                               \
                                                                    \
    lua_pushcfunction(L, l_##name);                                 \
    lua_setfield(L, -2, #name);                                     \
                                                                    \
    lua_pop(L, 1);                                                  \
  } while (0)

inline std::string lua_getstring(lua_State *L, int idx) {
  return luaL_checkstring(L, idx);
}

inline float lua_getfloat(lua_State *L, int idx) {
  return static_cast<float>(luaL_checknumber(L, idx));
}

inline int lua_getint(lua_State *L, int idx) {
  return static_cast<int>(luaL_checkinteger(L, idx));
}

inline bool lua_getbool(lua_State *L, int idx) {
  return lua_toboolean(L, idx) != 0;
}

inline int lua_error_msg(lua_State *L, const char *msg) {
  return luaL_error(L, "%s", msg);
}

inline void lua_push(lua_State *L, float val) {
  lua_pushnumber(L, val);
}

inline void lua_push(lua_State *L, int val) {
  lua_pushinteger(L, val);
}

inline void lua_push(lua_State *L, const std::string &val) {
  lua_pushstring(L, val.c_str());
}

inline void lua_push(lua_State *L, bool val) {
  lua_pushboolean(L, val);
}

static nlohmann::json lua_table_to_json(lua_State *L, int index) {
  nlohmann::json j = nlohmann::json::object();

  lua_pushnil(L);
  while (lua_next(L, index)) {
    std::string key = luaL_checkstring(L, -2);

    if (lua_isstring(L, -1))
      j[key] = std::string(lua_tostring(L, -1));
    else if (lua_isnumber(L, -1))
      j[key] = lua_tonumber(L, -1);
    else if (lua_isboolean(L, -1))
      j[key] = lua_toboolean(L, -1);
    else
      j[key] = nullptr;

    lua_pop(L, 1);
  }

  return j;
}

static void json_to_lua_table(lua_State *L, const nlohmann::json &j) {
  lua_newtable(L);

  for (auto &[key, value] : j.items()) {
    lua_pushstring(L, key.c_str());

    if (value.is_string())
      lua_pushstring(L, value.get<std::string>().c_str());
    else if (value.is_number())
      lua_pushnumber(L, value.get<double>());
    else if (value.is_boolean())
      lua_pushboolean(L, value.get<bool>());
    else
      lua_pushnil(L);

    lua_settable(L, -3);
  }
}

inline void CallLuaVoid(lua_State *L, int ref) {
  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return;
  }

  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    const char *err = lua_tostring(L, -1);
    fprintf(stderr, "[Lua Error] %s\n", err ? err : "unknown");
    lua_pop(L, 1);
  }
}
#endif  // CHERRY_ENABLE_SCRIPTING