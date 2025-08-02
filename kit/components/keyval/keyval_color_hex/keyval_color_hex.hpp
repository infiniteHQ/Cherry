#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// KeyValColorHex
//

#ifndef CHERRY_KIT_KEYVAL_COLOR_HEX
#define CHERRY_KIT_KEYVAL_COLOR_HEX

namespace Cherry {
namespace Components {
class KeyValColorHex : public Component {
public:
  KeyValColorHex(const Cherry::Identifier &id, const std::string &label,
                 std::string *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:keyval_color_text");
    SetProperty("size_x", "30");
    SetProperty("padding_x", "7");
    SetProperty("padding_y", "7");
    SetProperty("enter_return", "false");
    SetProperty("focus_on_appear", "false");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    auto parent = Cherry::GetParent();
    if (!parent || !m_Value) {
      return;
    }

    if (std::stoi(parent->GetData("renderedColumn")) == 0) {
      CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                 IM_COL32(100, 100, 100, 40));
      CherryGUI::AlignTextToFramePadding();
      CherryGUI::Indent(10.0f);
      CherryGUI::TextWrapped(GetProperty("label").c_str());
      CherryGUI::Unindent(10.0f);
    } else if (std::stoi(parent->GetData("renderedColumn")) == 1) {
      std::string identifier = GetIdentifier().string();
      std::string popupID = "ColorPickerPopup##" + identifier;

      ImU32 currentColor = HexToImU32(*m_Value);
      ImVec4 colorForButton = ImGui::ColorConvertU32ToFloat4(currentColor);

      if (ImGui::ColorButton(
              ("##ColorPreview" + identifier).c_str(), colorForButton,
              ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop)) {
        ImGui::OpenPopup(popupID.c_str());
      }

      ImGui::SameLine();
      char buffer[10];
      std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));
      buffer[sizeof(buffer) - 1] = '\0';

      ImGui::SetNextItemWidth(-FLT_MIN);
      if (ImGui::InputText(("##" + identifier).c_str(), buffer, sizeof(buffer),
                           ImGuiInputTextFlags_CharsHexadecimal |
                               ImGuiInputTextFlags_CharsUppercase)) {
        *m_Value = std::string(buffer);
      }

      if (ImGui::BeginPopup(popupID.c_str())) {
        static ImU32 tempColor;

        if (ImGui::IsPopupOpen(popupID.c_str(), ImGuiPopupFlags_AnyPopupId)) {
          tempColor = currentColor;
        }

        ImVec4 tempColorVec4 = ImGui::ColorConvertU32ToFloat4(tempColor);

        ImGuiColorEditFlags picker_flags =
            ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB |
            ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex;

        if (ImGui::ColorPicker4("##picker", (float *)&tempColorVec4,
                                picker_flags)) {
          tempColor = ImGui::ColorConvertFloat4ToU32(tempColorVec4);
        }

        ImGui::Separator();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(0.3f, 0.5f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4(0.1f, 0.3f, 0.7f, 1.0f));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
          *m_Value = ImU32ToHex(tempColor);
          ImGui::CloseCurrentPopup();
        }

        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
          ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
      }
    }
  }

private:
  std::string *m_Value;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValColorHex(const Identifier &identifier,
                                 const std::string &label, std::string *value) {
  return CherryApp.PushComponent<Cherry::Components::KeyValColorHex>(
      identifier, label, value);
}

inline Component &KeyValColorHex(const std::string &label, std::string *value) {
  return Cherry::Kit::KeyValColorHex(
      Application::GenerateUniqueID(label, value, "KeyValColorHex"), label,
      value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_COLOR_HEX
