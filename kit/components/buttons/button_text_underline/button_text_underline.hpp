#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ButtonTextUnderline
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_UNDERLINE
#define CHERRY_KIT_BUTTON_UNDERLINE

namespace Cherry {
namespace Components {
class ButtonTextUnderline : public Component {
public:
  ButtonTextUnderline(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_underline", "#242424FF");
    SetProperty("color_underline_hovered", "#343434FF");
    SetProperty("color_underline_pressed", "#444444FF");
    SetProperty("color_text", "#BCBCBCFF");
    SetProperty("color_text_hovered", "#FFFFFFFF");
    SetProperty("color_text_pressed", "#FFFFFFFF");

    // Sizes
    SetProperty("margin_y", "2");
    SetProperty("size_y", "0");
    SetProperty("size_x", "0");

    // Params
    SetProperty("disabled", "false");
    SetProperty("disable_time", "false");

    // Informations
    SetProperty("label", label);

    // Data & User-level informations
    SetData("isClicked", "false");
    SetData("isPressed", "false");
    SetData("isHovered", "false");
    SetData("isActivated", "false");
    SetData("lastClicked", "never");
    SetData("lastPressed", "never");
    SetData("lastHovered", "never");
    SetData("lastActivated", "never");
  }

  void Render() override {
    int i = 0;

    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                            ImVec2(0, std::stof(GetProperty("margin_y"))));
    CherryGUI::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                            ImVec2(0, std::stof(GetProperty("margin_y"))));

    CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA("#00000000"));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA("#00000000"));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA("#00000000"));
    CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA("#00000000"));
    CherryGUI::PushStyleColor(ImGuiCol_Text,
                              HexToRGBA(GetProperty("color_text")));

    bool isClicked = CherryGUI::Button(
        GetProperty("label").c_str(), ImVec2(std::stoi(GetProperty("size_x")),
                                             std::stoi(GetProperty("size_y"))));

    if (CherryGUI::IsItemHovered()) {
      SetData("isHovered", "true");
    }

    if (CherryGUI::IsItemActivated()) {
      SetData("isActivated", "true");
    }

    if (CherryGUI::IsItemActive()) {
      SetData("isPressed", "true");
    }

    if (isClicked) {
      SetData("isClicked", "true");
    }

    if (CherryGUI::IsItemHovered()) {
      CherryGUI::PushStyleColor(ImGuiCol_Text,
                                HexToRGBA(GetProperty("color_text")));
      i++;
    }
    if (CherryGUI::IsItemActive()) {
      CherryGUI::PushStyleColor(ImGuiCol_Text,
                                HexToRGBA(GetProperty("color_text")));
      i++;
    }

    if (CherryGUI::IsItemHovered()) {
      ImVec2 pos = CherryGUI::GetItemRectMin();
      ImVec2 rect_size = CherryGUI::GetItemRectSize();
      ImDrawList *draw_list = CherryGUI::GetWindowDrawList();
      ImVec4 underline_color = CherryGUI::IsItemActive()
                                   ? HexToRGBA(GetProperty("color_text"))
                                   : HexToRGBA(GetProperty("color_text"));
      draw_list->AddLine(ImVec2(pos.x, pos.y + rect_size.y),
                         ImVec2(pos.x + rect_size.x, pos.y + rect_size.y),
                         ImColor(underline_color));
    }

    CherryGUI::PopStyleVar(2);
    CherryGUI::PopStyleColor(5);
    CherryGUI::PopStyleColor(i);
  }

private:
  std::string GetCurrentTime() {
    std::string m_LastClickTime;
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    m_LastClickTime = std::ctime(&now_c);
    m_LastClickTime.erase(m_LastClickTime.length() - 1);
    return m_LastClickTime;
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &ButtonTextUnderline(const Identifier &identifier,
                                      const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::ButtonTextUnderline>(
      identifier, label);
}

inline Component &ButtonTextUnderline(const std::string &label) {
  return Cherry::Kit::ButtonTextUnderline(Application::GenerateUniqueID(label),
                                          label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BUTTON_UNDERLINE
