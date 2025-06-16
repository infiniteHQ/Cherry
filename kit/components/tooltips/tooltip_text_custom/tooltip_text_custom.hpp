#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TooltipTextCustom
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TOOLTIP_TEXT_CUSTOM
#define CHERRY_KIT_TOOLTIP_TEXT_CUSTOM

namespace Cherry {
namespace Components {
class TooltipTextCustom : public Component {
public:
  TooltipTextCustom(const Cherry::Identifier &id, const std::string &label,
                    const std::function<void()> &render_callback)
      : Component(id), m_RenderCallback(render_callback) {
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
  }

  void Render() override {
    CherryGUI::TextDisabled(GetProperty("label").c_str());
    if (CherryGUI::IsItemHovered()) {
      CherryGUI::BeginTooltip();
      CherryGUI::PushTextWrapPos(CherryGUI::GetFontSize() * 35.0f);
      if (m_RenderCallback) {
        m_RenderCallback();
      }
      CherryGUI::PopTextWrapPos();
      CherryGUI::EndTooltip();
    }
  }

private:
  std::function<void()> m_RenderCallback;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &
TooltipTextCustom(const Identifier &identifier, const std::string &label,
                  const std::function<void()> &render_callback) {
  return CherryApp.PushComponent<Cherry::Components::TooltipTextCustom>(
      identifier, label, render_callback);
}

inline Component &
TooltipTextCustom(const std::string &label,
                  const std::function<void()> &render_callback) {
  return Cherry::Kit::TooltipTextCustom(
      Application::GenerateUniqueID(label, render_callback,
                                    "TooltipTextCustom"),
      label, render_callback);
}
} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_TOOLTIP_TEXT_CUSTOM
