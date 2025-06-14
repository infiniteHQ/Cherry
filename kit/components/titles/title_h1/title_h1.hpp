#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TitleOne
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H1
#define CHERRY_KIT_TITLE_H1

namespace Cherry {
namespace Components {
class TitleOne : public Component {
public:
  TitleOne(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#FFFFFFFF"); // TODO get the default theme

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    float oldsize = CherryGUI::GetFont()->Scale;
    CherryGUI::GetFont()->Scale *= 2.10;
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
inline Component &TitleOne(const Identifier &identifier,
                           const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TitleOne>(identifier,
                                                               label);
}

inline Component &TitleOne(const std::string &label) {
  return Cherry::Kit::TitleOne(Application::GenerateUniqueID(label), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TITLE_H1
