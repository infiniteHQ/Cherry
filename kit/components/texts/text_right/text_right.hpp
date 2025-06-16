#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextRight
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_RIGHT
#define CHERRY_KIT_TEXT_RIGHT

namespace Cherry {
namespace Components {
class TextRight : public Component {
public:
  TextRight(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:text_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    std::string text = GetProperty("label").c_str();
    float windowWidth = CherryGUI::GetWindowSize().x;
    float textWidth = CherryGUI::CalcTextSize(text.c_str()).x;

    CherryGUI::SetCursorPosX(windowWidth - textWidth);

    CherryGUI::TextWrapped(text.c_str());
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TextRight(const Identifier &identifier,
                            const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TextRight>(identifier,
                                                                label);
}

inline Component &TextRight(const std::string &label) {
  return Cherry::Kit::TextRight(
      Application::GenerateUniqueID(label, "TextRight"), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_RIGHT
