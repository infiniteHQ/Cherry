#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// KeyValComboString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_COMBO_SIMPLE
#define CHERRY_KIT_KEYVAL_COMBO_SIMPLE

namespace Cherry {
namespace Components {
class KeyValComboString : public Component {
public:
  KeyValComboString(const Cherry::Identifier &id, const std::string &label,
                    std::vector<std::string> *list,
                    int default_selected_index = 0)
      : Component(id), m_List(list) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:keyval_color_text");

    // Behavior
    SetProperty("default_index", std::to_string(default_selected_index));
    SetProperty("selected_index", std::to_string(default_selected_index));
    SetData("selected_string", (*m_List)[default_selected_index]);

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    auto parent = Cherry::GetParent();
    if (parent) {
      if (std::stoi(parent->GetData("renderedColumn")) == 0) {
        CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                   IM_COL32(100, 100, 100, 40));
        CherryGUI::AlignTextToFramePadding();
        CherryGUI::Indent(10.0f);
        CherryGUI::TextWrapped(GetProperty("label").c_str());
        CherryGUI::Unindent(10.0f);
      } else if (std::stoi(parent->GetData("renderedColumn")) == 1) {
        std::string identifier = GetIdentifier().string();
        std::string Label = "####" + GetProperty("label");

        if (!identifier.empty()) {
          Label += Label + identifier;
        }

        CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                   IM_COL32(100, 100, 100, 40));

        if (!m_List->empty()) {
          static ImGuiComboFlags flags = 0;

          int default_index = std::stoi(GetProperty("default_index"));
          int selected = std::stoi(GetProperty("selected_index"));

          if (default_index < 0 || default_index >= m_List->size())
            default_index = 0;

          std::string identifier = GetIdentifier().string();
          std::string Label = "####" + GetProperty("label");

          if (!identifier.empty()) {
            Label += Label + identifier;
          }

          const char *combo_preview_value = (*m_List)[selected].c_str();
          if (CherryGUI::BeginCombo(Label.c_str(), combo_preview_value,
                                    flags)) {
            for (int n = 0; n < m_List->size(); n++) {
              const bool is_selected = (selected == n);
              if (CherryGUI::Selectable((*m_List)[n].c_str(), is_selected)) {
                if (selected != n) {
                  selected = n;
                  SetProperty("selected_index", std::to_string(n));
                  SetData("selected_string", (*m_List)[n]);
                  // UpdateLastChangedTime();
                }
              }

              if (is_selected)
                CherryGUI::SetItemDefaultFocus();
            }
            CherryGUI::EndCombo();
          }
        } else {
          CherryGUI::Text("INVALID/EMPTY VALUE");
        }
      }
    }
  }

private:
  std::vector<std::string> *m_List;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValComboString(const Identifier &identifier,
                                    const std::string &label,
                                    std::vector<std::string> *list,
                                    int default_selected_index = 0) {
  return CherryApp.PushComponent<Cherry::Components::KeyValComboString>(
      identifier, label, list, default_selected_index);
}

inline Component &KeyValComboString(const std::string &label,
                                    std::vector<std::string> *list,
                                    int default_selected_index = 0) {
  return Cherry::Kit::KeyValComboString(
      Application::GenerateUniqueID(label, list, default_selected_index,
                                    "KeyValComboString"),
      label, list, default_selected_index);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_COMBO_SIMPLE
