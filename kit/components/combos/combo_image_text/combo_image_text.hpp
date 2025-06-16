#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ComboImageText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT__IMAGE_TEXT
#define CHERRY_KIT__IMAGE_TEXT

namespace Cherry {
namespace Components {
class ComboImageText : public Component {
public:
  ComboImageText(const Cherry::Identifier &id, const std::string &label,
                 const std::vector<std::pair<std::string, std::string>> &list,
                 int default_index)
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

    // Data & User-level informations
    SetData("lastClicked", "never");
    SetData("isClicked", "false");
  }

  void Render() override {
    int selected = std::stoi(GetProperty("selected"));
    int default_index = std::stoi(GetProperty("default_index"));
    static ImGuiComboFlags flags = 0;

    if (default_index < 0 || default_index >= m_List.size())
      default_index = 0;
    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!identifier.empty()) {
      Label += Label + "####" + identifier;
    }

    if (CherryGUI::BeginCombo(Label.c_str(), [&]() {
          ImTextureID texture =
              Application::Get().GetCurrentRenderedWindow()->get_texture(
                  m_List[selected].second);
          CherryGUI::Image(texture, ImVec2(15, 15));
          CherryGUI::SameLine();
          CherryGUI::Text(m_List[selected].first.c_str());
        })) {
      for (int n = 0; n < m_List.size(); n++) {
        const bool is_selected = (selected == n);

        if (CherryGUI::Selectable(
                std::string("####" + identifier + std::to_string(n)).c_str(),
                is_selected)) {
          if (selected != n) {
            SetProperty("selected", std::to_string(n));
            selected = n;
            // UpdateLastChangedTime();
          }
        }

        CherryGUI::SameLine();

        ImTextureID texture =
            Application::Get().GetCurrentRenderedWindow()->get_texture(
                m_List[n].second);
        CherryGUI::Image(texture, ImVec2(15, 15));
        CherryGUI::SameLine();
        CherryGUI::Text(m_List[n].first.c_str());

        if (is_selected)
          CherryGUI::SetItemDefaultFocus();
      }
      CherryGUI::EndCombo();
    }
  }

private:
  std::vector<std::pair<std::string, std::string>> m_List;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &
ComboImageText(const Identifier &identifier, const std::string &label,
               const std::vector<std::pair<std::string, std::string>> &list,
               int default_index = 0) {
  return CherryApp.PushComponent<Cherry::Components::ComboImageText>(
      identifier, label, list, default_index);
}

inline Component &
ComboImageText(const std::string &label,
               const std::vector<std::pair<std::string, std::string>> &list,
               int default_index = 0) {
  return Cherry::Kit::ComboImageText(
      Application::GenerateUniqueID(label, list, default_index,
                                    "ComboImageText"),
      label, list, default_index);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT__IMAGE_TEXT
