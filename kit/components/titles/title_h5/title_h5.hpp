#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// TitleFive
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H5
#define CHERRY_KIT_TITLE_H5

namespace Cherry {
namespace Components {
class TitleFive : public Component {
public:
  TitleFive(const Cherry::Identifier &id, const std::string &label)
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
    CherryGUI::GetFont()->Scale *= 1.30; // TODO : property
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
inline Component &TitleFive(const Identifier &identifier,
                            const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TitleFive>(identifier,
                                                                label);
}

inline Component &TitleFive(const std::string &label) {
  return Cherry::Kit::TitleFive(
      Application::GenerateUniqueID(label, "TitleFive"), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TITLE_H5
