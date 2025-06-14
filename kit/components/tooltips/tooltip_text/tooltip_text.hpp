#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

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
    SetProperty("color_border", "#454545B2");
    SetProperty("color_border_hovered", "#454545B2");
    SetProperty("color_border_clicked", "#454545B2");
    SetProperty("color_bg", "#242424FF");
    SetProperty("color_bg_hovered", "#343434FF");
    SetProperty("color_bg_clicked", "#444444FF");

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
      Application::GenerateUniqueID(label, description), label, description);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_TOOLTIP_TEXT
