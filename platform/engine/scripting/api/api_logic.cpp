
#include "../../drawing/drawing.hpp"
#include "../../imgui/wrappers/wrappers.hpp"
#include "../lua_helpers.hpp"
#include "../scripting.hpp"

#ifdef CHERRY_ENABLE_SCRIPTING

#include "../../components/components.hpp"
#include "../../identifier/identifier.hpp"

namespace Cherry {
namespace Script {

static Cherry::Component &Lua_GetComponentHelper(lua_State *L,
                                                 const std::string &id_str) {
  Cherry::Identifier id(id_str);
  return Cherry::Application::Get().GetComponent(id);
}
LUA_FUNC(SetComponentProperty) {
  std::string id_str = lua_getstring(L, 1);
  std::string key = lua_getstring(L, 2);
  std::string val = lua_getstring(L, 3);

  auto comp = Cherry::Application::GetComponentPtr(Cherry::Identifier(id_str));
  if (comp) {
    comp->SetProperty(key, val);
  }
  return 0;
}

LUA_FUNC(GetComponentProperty) {
  std::string id_str = lua_getstring(L, 1);
  std::string key = lua_getstring(L, 2);

  auto comp = Cherry::Application::GetComponentPtr(Cherry::Identifier(id_str));
  if (comp) {
    lua_pushstring(L, comp->GetProperty(key).c_str());
  } else {
    lua_pushstring(L, "");
  }
  return 1;
}

LUA_FUNC(SetComponentData) {
  std::string id_str = lua_getstring(L, 1);
  std::string key = lua_getstring(L, 2);
  std::string val = lua_getstring(L, 3);

  auto comp = Cherry::Application::GetComponentPtr(Cherry::Identifier(id_str));
  if (comp) {
    comp->SetData(key, val);
  }
  return 0;
}

LUA_FUNC(GetComponentData) {
  std::string id_str = lua_getstring(L, 1);
  std::string key = lua_getstring(L, 2);

  auto comp = Cherry::Application::GetComponentPtr(Cherry::Identifier(id_str));
  if (comp) {
    lua_pushstring(L, comp->GetData(key).c_str());
  } else {
    lua_pushstring(L, "");
  }
  return 1;
}

LUA_FUNC(GetCurrentComponentID) {
  auto &comp = CherryCurrentComponent;
  lua_pushstring(L, comp.GetIdentifier().string().c_str());
  return 1;
}

LUA_FUNC(SetCurrentComponentData) {
  std::string key = lua_getstring(L, 1);
  std::string val = lua_getstring(L, 2);

  auto &comp = CherryCurrentComponent;
  comp.SetData(key, val);
  return 0;
}

LUA_FUNC(SetCurrentComponentProperty) {
  std::string key = lua_getstring(L, 1);
  std::string val = lua_getstring(L, 2);

  auto &comp = CherryCurrentComponent;
  comp.SetProperty(key, val);
  return 0;
}

LUA_FUNC(GetCurrentComponentData) {
  std::string key = lua_getstring(L, 1);

  auto &comp = CherryCurrentComponent;
  lua_pushstring(L, comp.GetData(key).c_str());
  return 1;
}

LUA_FUNC(GetCurrentComponentProperty) {
  std::string key = lua_getstring(L, 1);

  auto &comp = CherryCurrentComponent;
  lua_pushstring(L, comp.GetProperty(key).c_str());

  return 1;
}

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

LUA_FUNC(IsMouseOverRect) {
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  float w = luaL_checknumber(L, 3);
  float h = luaL_checknumber(L, 4);

  ImVec2 min(x, y);
  ImVec2 max(x + w, y + h);

  bool hovered = ImGui::IsMouseHoveringRect(min, max, false) &&
                 !ImGui::IsAnyItemHovered() &&
                 !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

  lua_pushboolean(L, hovered);
  return 1;
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

LUA_FUNC(SetDrawCursorPos) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  CherryGUI::SetCursorPos(ImVec2(x, y));
  return 0;
}

LUA_FUNC(SetDrawCursorPosX) {
  float x = lua_getfloat(L, 1);
  CherryGUI::SetCursorPosX(x);
  return 0;
}

LUA_FUNC(SetDrawCursorPosY) {
  float y = lua_getfloat(L, 1);
  CherryGUI::SetCursorPosY(y);
  return 0;
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

LUA_FUNC(PushFont) {
  std::string font = static_cast<std::string>(lua_getstring(L, 1));

  Cherry::PushFont(font);

  return 0;
}

LUA_FUNC(PopFont) {
  Cherry::PopFont();

  return 0;
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

static std::string ResolveLuaPath(const std::string &path) {
  namespace fs = std::filesystem;
  try {
    if (fs::is_directory(path)) {
      fs::path mainFile = fs::path(path) / "main.lua";
      if (fs::exists(mainFile)) {
        return mainFile.string();
      }
    }
  } catch (...) {
    // failed
  }
  return path;
}

LUA_FUNC(Script) {
  int n = lua_gettop(L);
  std::string rawPath = static_cast<std::string>(lua_tostring(L, 1));
  std::string resolvedPath = ResolveLuaPath(Cherry::GetPath(rawPath));

  return ScriptingEngine::InternalRenderScript(resolvedPath, false, n - 1);
}

LUA_FUNC(FreshScript) {
  int n = lua_gettop(L);
  std::string rawPath = static_cast<std::string>(lua_tostring(L, 1));
  std::string resolvedPath = ResolveLuaPath(Cherry::GetPath(rawPath));

  return ScriptingEngine::InternalRenderScript(resolvedPath, true, n - 1);
}

LUA_FUNC(AbsoluteScript) {
  int n = lua_gettop(L);
  std::string rawPath = static_cast<std::string>(lua_tostring(L, 1));
  std::string resolvedPath = ResolveLuaPath(rawPath);

  return ScriptingEngine::InternalRenderScript(resolvedPath, false, n - 1);
}

LUA_FUNC(FreshAbsoluteScript) {
  int n = lua_gettop(L);
  std::string rawPath = static_cast<std::string>(lua_tostring(L, 1));
  std::string resolvedPath = ResolveLuaPath(rawPath);

  return ScriptingEngine::InternalRenderScript(resolvedPath, true, n - 1);
}

LUA_FUNC(BeginComponent) {
  std::string id = static_cast<std::string>(lua_getstring(L, 1));

  auto component = CherryApp.PushComponent<Cherry::Component>(
      Cherry::Identifier(id, Cherry::RenderMode::CreateOnly));

  CherryApp.SetCurrentComponent(&component);
  return 0;
}

LUA_FUNC(EndComponent) {
  CherryApp.ResetCurrentComponent();
  return 0;
}

LUA_FUNC(Close) {
  CherryApp.Close();
  return 0;
}

void RegisterLogicAPI(lua_State *L) {
  // Application
  LUA_REGISTER(L, -1, Close);

  // Execute lua (in lua)
  LUA_REGISTER(L, -1, FreshScript);
  LUA_REGISTER(L, -1, Script);
  LUA_REGISTER(L, -1, FreshAbsoluteScript);
  LUA_REGISTER(L, -1, AbsoluteScript);

  // Drawing Cursor pos
  LUA_REGISTER(L, -1, GetDrawCursorPos);
  LUA_REGISTER(L, -1, GetDrawCursorPosY);
  LUA_REGISTER(L, -1, GetDrawCursorPosX);
  LUA_REGISTER(L, -1, SetDrawCursorPos);
  LUA_REGISTER(L, -1, SetDrawCursorPosY);
  LUA_REGISTER(L, -1, SetDrawCursorPosX);

  // Components
  LUA_REGISTER(L, -1, BeginComponent);
  LUA_REGISTER(L, -1, EndComponent);

  // Fonts
  LUA_REGISTER(L, -1, PushFont);
  LUA_REGISTER(L, -1, PopFont);

  // Components
  LUA_REGISTER(L, -1, GetCurrentComponentID);
  LUA_REGISTER(L, -1, SetComponentProperty);
  LUA_REGISTER(L, -1, GetComponentProperty);
  LUA_REGISTER(L, -1, SetComponentData);
  LUA_REGISTER(L, -1, GetComponentData);
  LUA_REGISTER(L, -1, SetCurrentComponentProperty);
  LUA_REGISTER(L, -1, GetCurrentComponentProperty);
  LUA_REGISTER(L, -1, SetCurrentComponentData);
  LUA_REGISTER(L, -1, GetCurrentComponentData);

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
  LUA_REGISTER(L, -1, IsMouseOverRect);
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
