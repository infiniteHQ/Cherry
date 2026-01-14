
#include "../../drawing/drawing.hpp"
#include "../../imgui/wrappers/wrappers.hpp"
#include "../lua_helpers.hpp"
#include "../scripting.hpp"

#ifdef CHERRY_ENABLE_SCRIPTING

namespace Cherry {
namespace Script {

LUA_FUNC(IsMouseClickedOnPos) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  int btn = lua_getint(L, 5);
  bool repeat = (lua_gettop(L) >= 6) ? lua_toboolean(L, 6) : false;

  ImVec2 mousePos = ImGui::GetMousePos();
  ImVec2 winPos = ImGui::GetWindowPos();

  bool inside = (mousePos.x >= winPos.x + x && mousePos.x <= winPos.x + x + w &&
                 mousePos.y >= winPos.y + y && mousePos.y <= winPos.y + y + h);

  bool result = inside && CherryApp.IsMouseClicked(btn, repeat);
  lua_pushboolean(L, result);
  return 1;
}

LUA_FUNC(IsMouseDoubleClickedOnPos) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  int btn = lua_getint(L, 5);

  ImVec2 mousePos = ImGui::GetMousePos();
  ImVec2 winPos = ImGui::GetWindowPos();

  bool inside = (mousePos.x >= winPos.x + x && mousePos.x <= winPos.x + x + w &&
                 mousePos.y >= winPos.y + y && mousePos.y <= winPos.y + y + h);

  bool result = inside && CherryApp.IsMouseDoubleClicked(btn);
  lua_pushboolean(L, result);
  return 1;
}

LUA_FUNC(IsKeyPressedOnPos) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);

  if (!lua_isstring(L, 5)) {
    lua_pushboolean(L, false);
    return 1;
  }

  std::string keyStr = lua_tostring(L, 5);
  auto cherryKey = Cherry::Application::StringToCherryKey(keyStr);

  ImVec2 mousePos = ImGui::GetMousePos();
  ImVec2 winPos = ImGui::GetWindowPos();

  bool inside = (mousePos.x >= winPos.x + x && mousePos.x <= winPos.x + x + w &&
                 mousePos.y >= winPos.y + y && mousePos.y <= winPos.y + y + h);

  if (inside && cherryKey.has_value()) {
    lua_pushboolean(L, CherryApp.IsKeyPressed(cherryKey.value()));
  } else {
    lua_pushboolean(L, false);
  }
  return 1;
}

LUA_FUNC(IsMouseClicked) {
  int btn = lua_getint(L, 1);

  bool repeat = (lua_gettop(L) >= 2) ? lua_toboolean(L, 2) : false;

  bool pressed = CherryApp.IsMouseClicked(btn, repeat);

  lua_pushboolean(L, pressed);
  return 1;
}

LUA_FUNC(IsMouseDoubleClicked) {
  int btn = lua_getint(L, 1);

  bool pressed = CherryApp.IsMouseDoubleClicked(btn);

  return 1;
}

LUA_FUNC(GetMouseScreenPos) {
  float x = CherryGUI::GetScreenMousePos().x;
  float y = CherryGUI::GetScreenMousePos().y;

  lua_pushnumber(L, x);
  lua_pushnumber(L, y);

  return 2;
}

LUA_FUNC(GetMouseScreenPosY) {
  float y = CherryGUI::GetScreenMousePos().y;

  lua_pushnumber(L, y);

  return 1;
}

LUA_FUNC(GetMouseScreenPosX) {
  float x = CherryGUI::GetScreenMousePos().x;

  lua_pushnumber(L, x);

  return 1;
}

LUA_FUNC(GetMousePos) {
  float x = CherryGUI::GetWindowMousePos().x;
  float y = CherryGUI::GetWindowMousePos().y;

  lua_pushnumber(L, x);
  lua_pushnumber(L, y);

  return 2;
}

LUA_FUNC(GetMousePosY) {
  float y = CherryGUI::GetWindowMousePos().y;

  lua_pushnumber(L, y);

  return 1;
}

LUA_FUNC(GetMousePosX) {
  float x = CherryGUI::GetWindowMousePos().x;

  lua_pushnumber(L, x);

  return 1;
}

LUA_FUNC(GetDrawCursorPos) {
  float x = CherryGUI::GetCursorPosX();
  float y = CherryGUI::GetCursorPosY();

  lua_pushnumber(L, x);
  lua_pushnumber(L, y);

  return 2;
}

LUA_FUNC(GetDrawCursorPosY) {
  float y = CherryGUI::GetCursorPosY();

  lua_pushnumber(L, y);

  return 1;
}

LUA_FUNC(GetDrawCursorPosX) {
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

LUA_FUNC(IsKeyPressed) {
  if (!lua_isstring(L, 1)) {
    lua_pushboolean(L, false);
    return 1;
  }

  std::string keyStr = lua_tostring(L, 1);
  auto cherryKey = Cherry::Application::StringToCherryKey(keyStr);

  if (cherryKey.has_value()) {
    bool pressed = CherryApp.IsKeyPressed(cherryKey.value());
    lua_pushboolean(L, pressed);
  } else {
    lua_pushboolean(L, false);
  }

  return 1;
}

// IsRightClickedOnArea
// IsKeyPressed
// IsKeyPressedOnArea

void RegisterLogicAPI(lua_State *L) {
  // Drawing Cursor pos
  LUA_REGISTER(L, -1, GetDrawCursorPos);
  LUA_REGISTER(L, -1, GetDrawCursorPosY);
  LUA_REGISTER(L, -1, GetDrawCursorPosX);

  // Mouse pos
  LUA_REGISTER(L, -1, GetMousePos);
  LUA_REGISTER(L, -1, GetMousePosY);
  LUA_REGISTER(L, -1, GetMousePosX);
  LUA_REGISTER(L, -1, GetMouseScreenPos);
  LUA_REGISTER(L, -1, GetMouseScreenPosY);
  LUA_REGISTER(L, -1, GetMouseScreenPosX);

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

  // Keys
  LUA_REGISTER(L, -1, IsKeyPressed);
  LUA_REGISTER(L, -1, IsMouseClicked);
  LUA_REGISTER(L, -1, IsMouseDoubleClicked);
  LUA_REGISTER(L, -1, IsMouseClickedOnPos);
  LUA_REGISTER(L, -1, IsMouseDoubleClickedOnPos);
  LUA_REGISTER(L, -1, IsKeyPressedOnPos);

  // Audio
#ifdef CHERRY_ENABLE_AUDIO
  LUA_REGISTER(L, -1, PlaySound);
#endif
}

} // namespace Script
} // namespace Cherry
#endif // CHERRY_ENABLE_SCRIPTING
