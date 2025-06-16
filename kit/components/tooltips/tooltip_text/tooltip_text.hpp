#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// TooltipText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TOOLTIP_TEXT
#define CHERRY_KIT_TOOLTIP_TEXT

namespace Cherry {
namespace Components {
class TooltipText : public Component {
public:
  TooltipText(const Cherry::Identifier &id, const std::string &label,
              const std::string &description)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "theme:tooltip_color_border");
    SetProperty("color_border_hovered", "theme:tooltip_color_border_hovered");
    SetProperty("color_border_clicked", "theme:tooltip_color_border_clicked");
    SetProperty("color_bg", "theme:tooltip_color_bg");
    SetProperty("color_bg_hovered", "theme:tooltip_color_bg_hovered");
    SetProperty("color_bg_clicked", "theme:tooltip_color_bg_clicked");

    // Informations
    SetProperty("label", label);
    SetProperty("description", description);
  }

  void Render() override {
    CherryGUI::TextDisabled(GetProperty("label").c_str());
    if (CherryGUI::IsItemHovered()) {
      CherryGUI::BeginTooltip();
      CherryGUI::PushTextWrapPos(CherryGUI::GetFontSize() * 35.0f);
      CherryGUI::TextUnformatted(GetProperty("description").c_str());
      CherryGUI::PopTextWrapPos();
      CherryGUI::EndTooltip();
    }
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TooltipText(const Identifier &identifier,
                              const std::string &label,
                              const std::string &description) {
  return CherryApp.PushComponent<Cherry::Components::TooltipText>(
      identifier, label, description);
}

inline Component &TooltipText(const std::string &label,
                              const std::string &description) {
  return Cherry::Kit::TooltipText(
      Application::GenerateUniqueID(label, description, "TooltipText"), label,
      description);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_TOOLTIP_TEXT
