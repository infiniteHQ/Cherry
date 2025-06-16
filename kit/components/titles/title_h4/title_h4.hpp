#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TitleFour
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H4
#define CHERRY_KIT_TITLE_H4

namespace Cherry {
namespace Components {
class TitleFour : public Component {
public:
  TitleFour(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:title_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    float oldsize = CherryGUI::GetFont()->Scale;
    CherryGUI::GetFont()->Scale *= 1.50; // TODO : property
    CherryGUI::PushFont(CherryGUI::GetFont());

    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

    CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                           GetProperty("label").c_str());

    CherryGUI::PopStyleVar();

    CherryGUI::GetFont()->Scale = oldsize;
    CherryGUI::PopFont();
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TitleFour(const Identifier &identifier,
                            const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TitleFour>(identifier,
                                                                label);
}

inline Component &TitleFour(const std::string &label) {
  return Cherry::Kit::TitleFour(
      Application::GenerateUniqueID(label, "TitleFour"), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TITLE_H4
