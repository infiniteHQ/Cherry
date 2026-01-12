
#include "../../drawing/drawing.hpp"
#include "../../imgui/wrappers/wrappers.hpp"
#include "../lua_helpers.hpp"
#include "../scripting.hpp"

#ifdef CHERRY_ENABLE_SCRIPTING

namespace Cherry {
namespace Script {

LUA_FUNC(GetCursorPos) {
  float x = CherryGUI::GetCursorPosX();
  float y = CherryGUI::GetCursorPosY();

  lua_pushnumber(L, x);
  lua_pushnumber(L, y);

  return 2;
}

LUA_FUNC(GetCursorPosY) {
  float y = CherryGUI::GetCursorPosY();

  lua_pushnumber(L, y);

  return 1;
}

LUA_FUNC(GetCursorPosX) {
  float x = CherryGUI::GetCursorPosX();

  lua_pushnumber(L, x);

  return 1;
}

LUA_FUNC(GetWindowSize) {
  ImVec2 winsize = CherryGUI::GetWindowSize();

  lua_pushnumber(L, winsize.x);
  lua_pushnumber(L, winsize.y);

  return 2;
}

LUA_FUNC(GetWindowSizeX) {
  float x = CherryGUI::GetWindowSize().x;

  lua_pushnumber(L, x);

  return 1;
}

LUA_FUNC(GetWindowSizeY) {
  float y = CherryGUI::GetWindowSize().y;

  lua_pushnumber(L, y);

  return 1;
}

LUA_FUNC(GetWindowPos) {
  ImVec2 winpos = CherryGUI::GetWindowPos();

  lua_pushnumber(L, winpos.x);
  lua_pushnumber(L, winpos.y);

  return 2;
}

LUA_FUNC(GetWindowPosX) {
  float x = CherryGUI::GetWindowPos().x;

  lua_pushnumber(L, x);

  return 1;
}

LUA_FUNC(GetWindowPosY) {
  float y = CherryGUI::GetWindowPos().y;

  lua_pushnumber(L, y);

  return 1;
}

LUA_FUNC(GetPath) {
  std::string relative_path = static_cast<std::string>(lua_getstring(L, 1));

  std::string path = Cherry::GetPath(relative_path);

  lua_pushstring(L, path.c_str());

  return 1;
}

#ifdef CHERRY_ENABLE_AUDIO
LUA_FUNC(PlaySound) {
  std::string audio_file = static_cast<std::string>(lua_getstring(L, 1));

  CherryApp.PlaySound(audio_file);

  return 0;
}
#endif

LUA_FUNC(GetLocale) {
  std::string locale = static_cast<std::string>(lua_getstring(L, 1));

  std::string result = Cherry::GetLocale(locale);

  lua_pushstring(L, result.c_str());

  return 1;
}

LUA_FUNC(SetTheme) {
  std::string theme_name = static_cast<std::string>(lua_getstring(L, 1));

  CherryApp.SetTheme(theme_name);

  return 0;
}

LUA_FUNC(PushTheme) {
  std::string theme_name = static_cast<std::string>(lua_getstring(L, 1));

  CherryApp.PushTheme(theme_name);

  return 0;
}

LUA_FUNC(PopTheme) {
  CherryApp.PopTheme();
  return 0;
}

LUA_FUNC(GetThemeProperty) {
  std::string theme = static_cast<std::string>(lua_getstring(L, 1));
  std::string key = static_cast<std::string>(lua_getstring(L, 2));

  std::string result = CherryApp.GetThemeProperty(theme, key);

  lua_pushstring(L, result.c_str());

  return 1;
}

LUA_FUNC(GetActiveThemeProperty) {
  std::string key = static_cast<std::string>(lua_getstring(L, 1));

  std::string result = CherryApp.GetActiveThemeProperty(key);

  lua_pushstring(L, result.c_str());

  return 1;
}

LUA_FUNC(Log) {
  std::string message = static_cast<std::string>(lua_getstring(L, 1));

  Cherry::Log::Print(Cherry::Log::Level::Info,
                     "",   // tag
                     "%s", // format
                     message.c_str());

  return 0;
}

LUA_FUNC(LogInfo) {
  std::string message = static_cast<std::string>(lua_getstring(L, 1));

  Cherry::Log::Print(Cherry::Log::Level::Info,
                     "",   // tag
                     "%s", // format
                     message.c_str());

  return 0;
}
LUA_FUNC(LogWarn) {
  std::string message = static_cast<std::string>(lua_getstring(L, 1));

  Cherry::Log::Print(Cherry::Log::Level::Warn,
                     "",   // tag
                     "%s", // format
                     message.c_str());

  return 0;
}
LUA_FUNC(LogError) {
  std::string message = static_cast<std::string>(lua_getstring(L, 1));

  Cherry::Log::Print(Cherry::Log::Level::Error,
                     "",   // tag
                     "%s", // format
                     message.c_str());

  return 0;
}

LUA_FUNC(LogFatal) {
  std::string message = static_cast<std::string>(lua_getstring(L, 1));

  Cherry::Log::Print(Cherry::Log::Level::Fatal,
                     "",   // tag
                     "%s", // format
                     message.c_str());

  return 0;
}

LUA_FUNC(LogTrace) {
  std::string message = static_cast<std::string>(lua_getstring(L, 1));

  Cherry::Log::Print(Cherry::Log::Level::Trace,
                     "",   // tag
                     "%s", // format
                     message.c_str());

  return 0;
}
// IsRightClickedOnArea
// IsKeyPressed
// IsKeyPressedOnArea

void RegisterLogicAPI(lua_State *L) {
  // Cursor pos
  LUA_REGISTER(L, -1, GetCursorPos);
  LUA_REGISTER(L, -1, GetCursorPosY);
  LUA_REGISTER(L, -1, GetCursorPosX);

  // Window size
  LUA_REGISTER(L, -1, GetWindowSize);
  LUA_REGISTER(L, -1, GetWindowSizeX);
  LUA_REGISTER(L, -1, GetWindowSizeY);

  // Window pos
  LUA_REGISTER(L, -1, GetWindowPos);
  LUA_REGISTER(L, -1, GetWindowPosX);
  LUA_REGISTER(L, -1, GetWindowPosY);

  // Paths
  LUA_REGISTER(L, -1, GetPath);

  // Locales
  LUA_REGISTER(L, -1, GetLocale);

  // Themes
  LUA_REGISTER(L, -1, PushTheme);
  LUA_REGISTER(L, -1, PopTheme);
  LUA_REGISTER(L, -1, SetTheme);
  LUA_REGISTER(L, -1, GetThemeProperty);
  LUA_REGISTER(L, -1, GetActiveThemeProperty);

  // Logs
  LUA_REGISTER(L, -1, Log);
  LUA_REGISTER(L, -1, LogInfo);
  LUA_REGISTER(L, -1, LogWarn);
  LUA_REGISTER(L, -1, LogError);
  LUA_REGISTER(L, -1, LogFatal);
  LUA_REGISTER(L, -1, LogTrace);

  // Audio
#ifdef CHERRY_ENABLE_AUDIO
  LUA_REGISTER(L, -1, PlaySound);
#endif
}

} // namespace Script
} // namespace Cherry
#endif // CHERRY_ENABLE_SCRIPTING
