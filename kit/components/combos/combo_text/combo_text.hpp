#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// ComboText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_COMBO_TEXT
#define CHERRY_KIT_COMBO_TEXT

namespace Cherry {
namespace Components {
class ComboText : public Component {
public:
  ComboText(const Cherry::Identifier &id, const std::string &label,
            std::vector<std::string> *list, int default_index)
      : Component(id), m_List(list) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "theme:combo_color_border");
    SetProperty("color_border_hovered", "theme:combo_color_border_hovered");
    SetProperty("color_border_clicked", "theme:combo_color_border_clicked");
    SetProperty("color_bg", "theme:combo_color_bg");
    SetProperty("color_bg_hovered", "theme:combo_color_bg_hovered");
    SetProperty("color_bg_clicked", "theme:combo_color_bg_clicked");

    // Images
    SetProperty("default_index", std::to_string(default_index));
    SetProperty("selected", std::to_string(default_index));

    // Informations
    SetProperty("label", label);

    // Size
    SetProperty("size_x", "50");
    SetProperty("padding_x", "7");
    SetProperty("padding_y", "7");

    // Data & User-level informations
    SetData("lastClicked", "never");
    SetData("isClicked", "false");
  }

  void Render() override {
    static ImGuiComboFlags flags = 0;

    int selected = std::stoi(GetProperty("selected"));
    int default_index = std::stoi(GetProperty("default_index"));
    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                            ImVec2(std::stoi(GetProperty("padding_x")),
                                   std::stoi(GetProperty("padding_y"))));

    float sizeX = std::stof(GetProperty("size_x"));

    if (default_index < 0 || default_index >= m_List->size())
      default_index = 0;

    const char *combo_preview_value = (*m_List)[selected].c_str();

    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    CherryGUI::SetNextItemWidth(sizeX);
    if (CherryGUI::BeginCombo(Label.c_str(), combo_preview_value, flags)) {
      for (int n = 0; n < (*m_List).size(); n++) {
        const bool is_selected = (selected == n);
        if (CherryGUI::Selectable((*m_List)[n].c_str(), is_selected)) {
          if (selected != n) {
            SetProperty("selected", std::to_string(n));
            // UpdateLastChangedTime();
          }
        }

        if (is_selected)
          CherryGUI::SetItemDefaultFocus();
      }
      CherryGUI::EndCombo();
    }
    CherryGUI::PopStyleVar();
  }

private:
  std::vector<std::string> *m_List;
};
} // namespace Components

// End-User API
namespace Kit {

inline Component &ComboText(const Identifier &identifier,
                            const std::string &label,
                            std::vector<std::string> *list,
                            int default_index = 0) {
  return CherryApp.PushComponent<Cherry::Components::ComboText>(
      identifier, label, list, default_index);
}

inline Component &ComboText(const std::string &label,
                            std::vector<std::string> *list,
                            int default_index = 0) {
  return Cherry::Kit::ComboText(
      Application::GenerateUniqueID(label, list, default_index, "ComboText"),
      label, list, default_index);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_COMBO_TEXT
