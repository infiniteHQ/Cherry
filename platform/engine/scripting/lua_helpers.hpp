#pragma once
#include "../../../lib/lua/src/lua.hpp"
#include <string>

#define LUA_FUNC(name) static int l_##name(lua_State *L)

#define LUA_REGISTER(L, table, name)                                           \
  lua_pushcfunction(L, l_##name);                                              \
  lua_setfield(L, -2, #name)

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

inline void lua_push(lua_State *L, float val) { lua_pushnumber(L, val); }
inline void lua_push(lua_State *L, int val) { lua_pushinteger(L, val); }
inline void lua_push(lua_State *L, const std::string &val) {
  lua_pushstring(L, val.c_str());
}
inline void lua_push(lua_State *L, bool val) { lua_pushboolean(L, val); }