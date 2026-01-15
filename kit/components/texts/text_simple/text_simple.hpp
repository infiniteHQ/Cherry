#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// TextSimple
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_SIMPLE
#define CHERRY_KIT_TEXT_SIMPLE

namespace Cherry {
namespace Components {
class TextSimple : public Component {
public:
  TextSimple(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:text_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                           GetProperty("label").c_str());

    auto parent = Cherry::GetParent();
    if (parent) {
      CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                             parent->GetProperty("label").c_str());
    }
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TextSimple(const Identifier &identifier,
                             const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TextSimple>(identifier,
                                                                 label);
}

inline Component &TextSimple(const std::string &label) {
  return Cherry::Kit::TextSimple(
      Application::GenerateUniqueID(label + "TextSimple"), label);
}

#ifdef CHERRY_ENABLE_SCRIPTING
LUA_FUNC(DrawTextSimple) {
  const char *label = luaL_checkstring(L, 1);
  auto cmp = Cherry::Kit::TextSimple(label);

  lua_pushstring(L, cmp.GetIdentifier().string().c_str());
  return 1;
}

inline void RegisterTextSimple() {
  LUA_REGISTER_GLOBAL_FUNC("Cherry", DrawTextSimple);
}

#endif
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_SIMPLE
