#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TitleThree
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H3
#define CHERRY_KIT_TITLE_H3

namespace Cherry {
namespace Components {
class TitleThree : public Component {
public:
  TitleThree(const Cherry::Identifier &id, const std::string &label)
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
    CherryGUI::GetFont()->Scale *= 1.70; // TODO : property
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
inline Component &TitleThree(const Identifier &identifier,
                             const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TitleThree>(identifier,
                                                                 label);
}

inline Component &TitleThree(const std::string &label) {
  return Cherry::Kit::TitleThree(
      Application::GenerateUniqueID(label, "TitleThree"), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TITLE_H3
