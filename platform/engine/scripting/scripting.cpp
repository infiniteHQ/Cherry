#include "scripting.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef CHERRY_ENABLE_SCRIPTING

namespace Cherry {
namespace Script {

namespace fs = std::filesystem;

static long long GetFileModTime(const std::string &path) {
  if (!fs::exists(path))
    return 0;
  auto time = fs::last_write_time(path);
  return std::chrono::duration_cast<std::chrono::seconds>(
             time.time_since_epoch())
      .count();
}

ScriptingEngine &GetScriptingEngine() {
  static ScriptingEngine instance;
  return instance;
}

ScriptingEngine::ScriptingEngine() {
  L = luaL_newstate();
  luaL_openlibs(L);

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, ScriptingEngine::LuaPrint, 1);
  lua_setglobal(L, "print");

  RegisterCherryAPI();
}

ScriptingEngine::~ScriptingEngine() {
  if (L)
    lua_close(L);
}

bool ScriptingEngine::Execute(const std::string &code) {
  if (luaL_dostring(L, code.c_str()) != LUA_OK) {
    m_LastError = lua_tostring(L, -1);
    CaptureOutput("[Error] " + m_LastError);
    lua_pop(L, 1);
    return false;
  }
  return true;
}

bool ScriptingEngine::LoadFile(const std::string &path) {
  if (luaL_loadfile(L, path.c_str()) != LUA_OK) {
    m_LastError = lua_tostring(L, -1);
    CaptureOutput("[Error] " + m_LastError);
    lua_pop(L, 1);
    return false;
  }

  if (m_HotReloadEnabled) {
    m_FileTimestamps[path] = GetFileModTime(path);
  }

  if (lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK) {
    m_LastError = lua_tostring(L, -1);
    CaptureOutput("[Runtime Error] " + m_LastError);
    lua_pop(L, 1);
    return false;
  }
  return true;
}

void ScriptingEngine::ReloadFile(const std::string &path) {
  CaptureOutput("[System] Reloading: " + path);
  LoadFile(path);
}

void ScriptingEngine::CaptureOutput(const std::string &text) {
  m_OutputHistory.push_back(text);
  if (m_OutputHistory.size() > 500)
    m_OutputHistory.erase(m_OutputHistory.begin());
}

int ScriptingEngine::LuaPrint(lua_State *L) {
  ScriptingEngine *engine =
      (ScriptingEngine *)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_gettop(L);
  std::string out = "";

  for (int i = 1; i <= n; i++) {
    if (lua_isstring(L, i)) {
      out += lua_tostring(L, i);
    } else if (lua_isboolean(L, i)) {
      out += lua_toboolean(L, i) ? "true" : "false";
    } else {
      out += lua_typename(L, lua_type(L, i));
    }
    if (i < n)
      out += "\t";
  }

  engine->CaptureOutput(out);
  return 0;
}

void ScriptingEngine::EnableHotReload(const std::string &scriptDir) {
  m_HotReloadEnabled = true;
  m_ScriptDir = scriptDir;
}

void ScriptingEngine::CheckForChanges() {
  if (!m_HotReloadEnabled)
    return;

  for (auto &[path, lastTime] : m_FileTimestamps) {
    auto currentTime = GetFileModTime(path);
    if (currentTime > lastTime) {
      m_FileTimestamps[path] = currentTime;
      ReloadFile(path);
    }
  }
}

std::string ScriptingEngine::GetLastOutput() const {
  return m_OutputHistory.empty() ? "" : m_OutputHistory.back();
}

std::string ScriptingEngine::GetLastError() const { return m_LastError; }

const std::vector<std::string> &ScriptingEngine::GetOutputHistory() const {
  return m_OutputHistory;
}

void ScriptingEngine::ClearOutput() { m_OutputHistory.clear(); }

static void InternalRenderScript(const std::string &path, bool fresh) {
  auto &engine = GetScriptingEngine();

  static std::unordered_map<std::string, bool> s_InitializedFiles;

  if (fresh) {
    if (!s_InitializedFiles[path]) {
      std::filesystem::path p(path);
      engine.EnableHotReload(p.parent_path().string());
      engine.LoadFile(path);
      s_InitializedFiles[path] = true;
    }
    engine.CheckForChanges();
  } else {
    if (!s_InitializedFiles[path]) {
      engine.LoadFile(path);
      s_InitializedFiles[path] = true;
    }
  }

  engine.LoadFile(path);
}

void RenderLuaScript(const std::string &lua_file_path) {
  InternalRenderScript(lua_file_path, false);
}

void RenderLuaFreshScript(const std::string &lua_file_path) {
  InternalRenderScript(lua_file_path, true);
}

void ScriptingEngine::RegisterCherryAPI() {
  lua_newtable(L);

  // Register apis
  RegisterDrawingAPI(L);

  lua_setglobal(L, "Cherry"); // Lib global
}
} // namespace Script
} // namespace Cherry

#endif