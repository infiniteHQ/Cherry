#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// SeparatorText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_SEPARATOR_TEXT
#define CHERRY_KIT_SEPARATOR_TEXT

namespace Cherry {
namespace Components {
class SeparatorText : public Component {
public:
  SeparatorText(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#454545B2");

    // Informations
    SetProperty("label", label);
    SetProperty("text_scale", "0.8");
  }

  void Render() override {
    std::string label = GetProperty("label");
    float text_scale = std::stof(GetProperty("text_scale"));
    const float &ctx_text_scale = Application::GetCurrentRenderedWindow()
                                      ->m_Specifications.FontGlobalScale;

    ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
    CherryGUI::GetFont()->Scale *= text_scale;
    CherryGUI::PushFont(CherryGUI::GetFont());

    CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + 5.0f);

    CherryGUI::PushStyleColor(ImGuiCol_Text, grayColor);
    CherryGUI::Text(label.c_str());
    CherryGUI::PopStyleColor();

    CherryGUI::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
    CherryGUI::Separator();
    CherryGUI::PopStyleColor();

    CherryGUI::GetFont()->Scale = ctx_text_scale;
    CherryGUI::PopFont();
    CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + 2.0f);
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &SeparatorText(const Identifier &identifier,
                                const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::SeparatorText>(identifier,
                                                                    label);
}

inline Component &SeparatorText(const std::string &label) {
  return Cherry::Kit::SeparatorText(Application::GenerateUniqueID(label),
                                    label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_SEPARATOR_TEXT
