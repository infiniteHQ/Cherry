#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"
//
// ButtonTextDropdown
// Summary : A dropdown button with the possibility of adding a custom menu.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_DROPDOWN_TEXT
#define CHERRY_KIT_BUTTON_DROPDOWN_TEXT

namespace Cherry {
namespace Components {
class ButtonTextDropdown : public Component {
public:
  ButtonTextDropdown(
      const Cherry::Identifier &id, const std::string &label,
      const std::function<void()> &dropdown_callback = []() {})
      : Component(id), m_DropdownCallback(dropdown_callback) {
    // Identifier
    SetIdentifier(id);

    // Images
    SetProperty("image_dropdown_up", Cherry::GetPath("resources/base/up.png"));
    SetProperty("image_dropdown_down",
                Cherry::GetPath("resources/base/down.png"));

    // Colors
    SetProperty("color_border", "theme:button_color_border");
    SetProperty("color_border_hovered", "theme:button_color_border_hovered");
    SetProperty("color_border_pressed", "theme:button_color_border_pressed");
    SetProperty("color_bg", "theme:button_color_bg");
    SetProperty("color_bg_hovered", "theme:button_color_bg_hovered");
    SetProperty("color_bg_pressed", "theme:button_color_bg_pressed");
    SetProperty("color_text", "theme:button_color_text");
    SetProperty("color_text_hovered", "theme:button_color_text_hovered");
    SetProperty("color_text_pressed", "theme:button_color_text_pressed");

    // Sizes
    SetProperty("size_x", "theme:button_size_x");
    SetProperty("size_y", "theme:button_size_y");
    SetProperty("padding_x", "theme:button_padding_x");
    SetProperty("padding_y", "theme:button_padding_y");

    SetProperty("scale", "theme:button_scale");
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
    ImTextureID texture;

    if (GetData("isMenuActivated") == "true") {
      texture = Application::Get().GetCurrentRenderedWindow()->get_texture(
          GetProperty("image_dropdown_up"));
    } else {
      texture = Application::Get().GetCurrentRenderedWindow()->get_texture(
          GetProperty("image_dropdown_down"));
    }

    const ImVec2 &size = ImVec2(std::stoi(GetProperty("size_x")),
                                std::stoi(GetProperty("size_y")));
    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                            ImVec2(std::stoi(GetProperty("padding_x")),
                                   std::stoi(GetProperty("padding_y"))));

    CherryGUI::PushStyleColor(ImGuiCol_Border,
                              HexToRGBA(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_Button,
                              HexToRGBA(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered,
                              HexToRGBA(GetProperty("color_bg_hovered")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonActive,
                              HexToRGBA(GetProperty("color_bg_clicked")));

    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");
    int style_props_opt = 0;

    if (GetData("isHovered") == "true") {
      if (GetProperty("disable_time") == "false")
        SetData("lastHovered", GetCurrentTime());
      CherryGUI::PushStyleColor(ImGuiCol_Border,
                                HexToRGBA(GetProperty("color_border_hovered")));
      CherryGUI::PushStyleColor(ImGuiCol_Button,
                                HexToRGBA(GetProperty("color_bg_hovered")));
      CherryGUI::PushStyleColor(ImGuiCol_Text,
                                HexToRGBA(GetProperty("color_text_hovered")));
      style_props_opt += 3;
    }

    if (GetData("isClicked") == "true") {
      if (GetProperty("disable_time") == "false")
        SetData("lastClicked", GetCurrentTime());

      CherryGUI::PushStyleColor(ImGuiCol_Border,
                                HexToRGBA(GetProperty("color_border_pressed")));
      CherryGUI::PushStyleColor(ImGuiCol_Button,
                                HexToRGBA(GetProperty("color_bg_pressed")));
      CherryGUI::PushStyleColor(ImGuiCol_Text,
                                HexToRGBA(GetProperty("color_text_pressed")));
      style_props_opt += 3;
    }

    if (GetData("isActivated") == "true") {
      if (GetProperty("disable_time") == "false")
        SetData("lastActivated", GetCurrentTime());
    }

    if (GetProperty("isPressed") == "true") {
      if (GetProperty("disable_time") == "false")
        SetData("lastPressed", GetCurrentTime());

      CherryGUI::PushStyleColor(ImGuiCol_Border,
                                HexToRGBA(GetProperty("color_border_pressed")));
      CherryGUI::PushStyleColor(ImGuiCol_Button,
                                HexToRGBA(GetProperty("color_bg_pressed")));
      CherryGUI::PushStyleColor(ImGuiCol_Text,
                                HexToRGBA(GetProperty("color_text_pressed")));
      style_props_opt += 3;
    }

    SetData("isHovered", "false");
    SetData("isClicked", "false");
    SetData("isPressed", "false");
    SetData("isActivated", "false");

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    bool isClicked =
        CherryGUI::RightImageButtonWithText(texture, Label.c_str(), size);

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
      SetData("isMenuActivated", "true");
    } else {
      SetData("isClicked", "false");
    }

    std::string context_menu_label = "TabContextMenu" + Label;
    if (GetData("isMenuActivated") == "true") {
      if (CherryGUI::BeginPopupContextItem(context_menu_label.c_str())) {
        ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f); // TODO : Props
        ImVec4 graySeparatorColor =
            ImVec4(0.4f, 0.4f, 0.4f, 0.5f); // TODO : Props
        ImVec4 darkBackgroundColor =
            ImVec4(0.15f, 0.15f, 0.15f, 1.0f); // TODO : Props
        ImVec4 lightBorderColor =
            ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // TODO : Props

        CherryGUI::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);
        CherryGUI::PushStyleColor(ImGuiCol_Border, lightBorderColor);
        CherryGUI::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);

        if (m_DropdownCallback) {
          m_DropdownCallback();
        }

        if (!CherryGUI::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) &&
            CherryGUI::IsMouseClicked(0)) {
          CherryGUI::CloseCurrentPopup();
          SetData("isMenuActivated", "false");
        }

        CherryGUI::PopStyleVar();
        CherryGUI::PopStyleColor(2);
        CherryGUI::EndPopup();
      }
    }

    if (GetData("isMenuActivated") == "true") {
      CherryGUI::OpenPopup(context_menu_label.c_str());
    }

    CherryGUI::PopStyleColor(style_props_opt);
    CherryGUI::PopStyleColor(4);
    CherryGUI::PopStyleVar();
  }

private:
  std::function<void()> m_DropdownCallback;

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

inline Component &ButtonTextDropdown(
    const Identifier &identifier, const std::string &label,
    const std::function<void()> &dropdown_callback = []() {}) {
  return CherryApp.PushComponent<Cherry::Components::ButtonTextDropdown>(
      identifier, label, dropdown_callback);
}

inline Component &ButtonTextDropdown(
    const std::string &label,
    const std::function<void()> &dropdown_callback = []() {}) {
  return Cherry::Kit::ButtonTextDropdown(
      Application::GenerateUniqueID(label, dropdown_callback,
                                    "ButtonTextDropdown"),
      label, dropdown_callback);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BUTTON_DROPDOWN_TEXT
