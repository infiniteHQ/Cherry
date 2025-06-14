#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ButtonImageTextImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_IMAGE_TEXT_IMAGE
#define CHERRY_KIT_BUTTON_IMAGE_TEXT_IMAGE

namespace Cherry {
namespace Components {
class ButtonImageTextImage : public Component {
public:
  ButtonImageTextImage(const Cherry::Identifier &id, const std::string &label,
                       const std::string &image_path,
                       const std::string &second_image_path)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "#454545FF");
    SetProperty("color_border_hovered", "#555555FF");
    SetProperty("color_border_pressed", "#757575FF");
    SetProperty("color_border_clicked", "#757575FF");
    SetProperty("color_bg", "#242424FF");
    SetProperty("color_bg_hovered", "#343434FF");
    SetProperty("color_bg_pressed", "#444444FF");
    SetProperty("color_bg_clicked", "#444444FF");
    SetProperty("color_text", "#BCBCBCFF");
    SetProperty("color_text_hovered", "#FFFFFFFF");
    SetProperty("color_text_pressed", "#FFFFFFFF");
    SetProperty("color_text_clicked", "#444444FF");

    // Sizes
    SetProperty("size_x", "15");
    SetProperty("size_y", "15");
    SetProperty("size_image_x", "15");
    SetProperty("size_image_y", "15");
    SetProperty("padding_x", "10");
    SetProperty("padding_y", "6");
    SetProperty("scale",
                "0"); // Instead of using sizes manually, we can use scale.

    // Params
    SetProperty("disabled", "false");
    SetProperty("disable_time", "false");

    // Informations
    SetProperty("label", label);
    SetProperty("image_path", image_path);
    SetProperty("second_image_path", second_image_path);

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
    std::cout << "1" << std::endl;
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

    ImTextureID texture =
        Application::Get().GetCurrentRenderedWindow()->get_texture(
            GetProperty("image_path"));
    ImTextureID second_texture =
        Application::Get().GetCurrentRenderedWindow()->get_texture(
            GetProperty("second_image_path"));
    CherryGUI::SetNextItemWidth(std::stof(GetProperty("size_x")));

    bool isClicked = CherryGUI::ImageButtonWithTextWithIcon(
        texture, second_texture, Label.c_str(),
        ImVec2(std::stof(GetProperty("size_image_x")),
               std::stof(GetProperty("size_image_y"))),
        ImVec2(std::stof(GetProperty("size_image_x")),
               std::stof(GetProperty("size_image_y"))));

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

    CherryGUI::PopStyleColor(style_props_opt);
    CherryGUI::PopStyleColor(4);
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
inline Component &ButtonImageTextImage(const Identifier &identifier,
                                       const std::string &label,
                                       const std::string &image_path,
                                       const std::string &second_image_path) {
  return CherryApp.PushComponent<Cherry::Components::ButtonImageTextImage>(
      identifier, label, image_path, second_image_path);
}

inline Component &ButtonImageTextImage(const std::string &label,
                                       const std::string &image_path,
                                       const std::string &second_image_path) {
  return Cherry::Kit::ButtonImageTextImage(
      Application::GenerateUniqueID(label, image_path, second_image_path),
      label, image_path, second_image_path);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_BUTTON_IMAGE_TEXT_IMAGE
