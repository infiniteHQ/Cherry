#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// ButtonTextImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_TEXT_IMAGE
#define CHERRY_KIT_BUTTON_TEXT_IMAGE

namespace Cherry {
namespace Components {
class ButtonTextImage : public Component {
public:
  ButtonTextImage(const Cherry::Identifier &id, const std::string &label,
                  const std::string &image_path)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Image Link
    SetProperty("image_path", image_path);

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

    // Instead of using sizes manually, we can use scale.
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
    const ImVec2 &size = ImVec2(std::stoi(GetProperty("size_x")),
                                std::stoi(GetProperty("size_y")));
    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                            ImVec2(std::stoi(GetProperty("padding_x")),
                                   std::stoi(GetProperty("padding_y"))));

    CherryGUI::PushStyleColor(ImGuiCol_Border,
                              HexToRGBA(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_Button,
                              HexToRGBA(GetProperty("color_bg")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered,
                              HexToRGBA(GetProperty("color_bg_hovered")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonActive,
                              HexToRGBA(GetProperty("color_bg_pressed")));
    CherryGUI::PushStyleColor(ImGuiCol_Text,
                              HexToRGBA(GetProperty("color_text")));

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

    ImTextureID texture =
        Application::Get().GetCurrentRenderedWindow()->get_texture(
            GetProperty("image_path"));
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
    }

    CherryGUI::PopStyleColor(style_props_opt);
    CherryGUI::PopStyleColor(5);
    CherryGUI::PopStyleVar();
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

inline Component &ButtonTextImage(const Identifier &identifier,
                                  const std::string &label,
                                  const std::string &image_path) {
  return CherryApp.PushComponent<Cherry::Components::ButtonTextImage>(
      identifier, label, image_path);
}

inline Component &ButtonTextImage(const std::string &label,
                                  const std::string &image_path) {
  return Cherry::Kit::ButtonTextImage(
      Application::GenerateUniqueID(label, image_path, "ButtonTextImage"),
      label, image_path);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BUTTON_TEXT_IMAGE
