#pragma once
#include "../../../cherry.hpp"

namespace Cherry {
namespace Components {
class CalculatorScreen : public Component {
public:
  CalculatorScreen(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:text_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    Cherry::Script::RenderLuaFreshScript(
        Cherry::GetPath("resources/components/screen/screen.lua"));
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &CalculatorScreen(const Identifier &identifier,
                                   const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::CalculatorScreen>(
      identifier, label);
}

inline Component &CalculatorScreen(const std::string &label) {
  return Cherry::Kit::CalculatorScreen(
      Application::GenerateUniqueID(label + "CalculatorScreen"), label);
}

#ifdef CHERRY_ENABLE_SCRIPTING
LUA_FUNC(DrawCalculatorScreen) {
  const char *val = luaL_checkstring(L, 1);

  auto cmp = Cherry::Kit::CalculatorScreen(CherryID("CalculatorScreen"), val);

  lua_pushstring(L, cmp.GetIdentifier().string().c_str());
  return 1;
}

inline void RegisterCalculatorScreen() {
  LUA_REGISTER_GLOBAL_FUNC("Cherry", DrawCalculatorScreen);
}
#endif

} // namespace Kit

} // namespace Cherry
