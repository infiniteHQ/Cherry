#pragma once
#include "../../../cherry.hpp"
#include <map>

namespace Cherry {
namespace Components {

class CalculatorButton : public Component {
public:
  CalculatorButton(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    SetIdentifier(id);
    SetProperty("color_text", "theme:text_color_text");
    SetProperty("label", label);
  }

  void Render() override {
    Cherry::Script::RenderLuaFreshScript(
        Cherry::GetPath("resources/components/button/button.lua"));
  }
};

} // namespace Components

namespace Kit {

inline Component &CalculatorButton(const Cherry::Identifier &identifier,
                                   const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::CalculatorButton>(
      identifier, label);
}

inline Component &CalculatorButton(const std::string &label) {
  static std::map<std::string, Cherry::Identifier> buttonIDs;

  if (buttonIDs.find(label) == buttonIDs.end())
    buttonIDs[label] = Application::GenerateUniqueID(label);

  return Cherry::Kit::CalculatorButton(buttonIDs[label], label);
}

#ifdef CHERRY_ENABLE_SCRIPTING

LUA_FUNC(DrawCalculatorButton) {
  const char *label = luaL_checkstring(L, 1);
  auto &cmp = Cherry::Kit::CalculatorButton(CherryID(label), label);

  lua_pushstring(L, cmp.GetIdentifier().string().c_str());
  return 1;
}

inline void RegisterCalculatorButton() {
  LUA_REGISTER_GLOBAL_FUNC("Cherry", DrawCalculatorButton);
}

#endif

} // namespace Kit
} // namespace Cherry
