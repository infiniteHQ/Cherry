#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// TitleSix
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H6
#define CHERRY_KIT_TITLE_H6

namespace Cherry {
namespace Components {
class TitleSix : public Component {
public:
  TitleSix(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:title_color_text");

    SetProperty("font_scale", "1.1");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    cached_text_color = HexToRGBA(GetProperty("color_text"));
    cached_label = GetProperty("label");
    font_scale = std::stof(GetProperty("font_scale"));

    float old_font_size = CherryGUI::GetFont()->Scale;
    CherryGUI::GetFont()->Scale *= font_scale;
    CherryGUI::PushFont(CherryGUI::GetFont());

    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
    CherryGUI::TextColored(cached_text_color, cached_label.c_str());
    CherryGUI::PopStyleVar();

    CherryGUI::GetFont()->Scale = old_font_size;
    CherryGUI::PopFont();
  }

private:
  ImVec4 cached_text_color;
  std::string cached_label;
  float font_scale;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TitleSix(const Identifier &identifier,
                           const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TitleSix>(identifier,
                                                               label);
}

inline Component &TitleSix(const std::string &label) {
  return Cherry::Kit::TitleSix(Application::GenerateUniqueID(label, "TitleSix"),
                               label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TITLE_H6
