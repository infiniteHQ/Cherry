#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextCenter
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_CENTER
#define CHERRY_KIT_TEXT_CENTER

namespace Cherry {
namespace Components {
class TextCenter : public Component {
public:
  TextCenter(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#FFFFFFFF");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    ImVec2 text_size = CherryGUI::CalcTextSize(GetProperty("label").c_str());

    ImVec2 child_size = CherryGUI::GetWindowSize();
    float posX = (child_size.x - text_size.x) * 0.5f;

    posX = posX > 0 ? posX : 0;

    CherryGUI::SetCursorPosX(posX);

    CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                           GetProperty("label").c_str());

    auto parent = Cherry::GetParent();
    if (parent) {
      ImVec2 text_size = CherryGUI::CalcTextSize(GetProperty("label").c_str());

      ImVec2 child_size = CherryGUI::GetWindowSize();
      float posX = (child_size.x - text_size.x) * 0.5f;

      posX = posX > 0 ? posX : 0;

      CherryGUI::SetCursorPosX(posX);

      CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                             GetProperty("label").c_str());
    }
  }
};
} // namespace Components

// End-User API
namespace Kit {

inline Component &TextCenter(const Identifier &identifier,
                             const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TextCenter>(identifier,
                                                                 label);
}

inline Component &TextCenter(const std::string &label) {
  return Cherry::Kit::TextCenter(Application::GenerateUniqueID(label), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_CENTER
