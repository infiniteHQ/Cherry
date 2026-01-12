
#include "../../drawing/drawing.hpp"
#include "../../imgui/wrappers/wrappers.hpp"
#include "../lua_helpers.hpp"
#include "../scripting.hpp"

#ifdef CHERRY_ENABLE_SCRIPTING

namespace Cherry {
namespace Script {

LUA_FUNC(DrawRect) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::RectWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h), color);

  return 0;
}

LUA_FUNC(DrawRectFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::RectScreen(Cherry::Vec2(x, y), Cherry::Vec2(w, h), color);

  return 0;
}

LUA_FUNC(DrawText) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  std::string text = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::TextWindow(text, Cherry::Vec2(x, y), size, color);

  return 0;
}

void RegisterDrawingAPI(lua_State *L) {
  LUA_REGISTER(L, -1, DrawRect);
  LUA_REGISTER(L, -1, DrawRectFixed);
  LUA_REGISTER(L, -1, DrawText);
}

} // namespace Script
} // namespace Cherry
#endif // CHERRY_ENABLE_SCRIPTING
