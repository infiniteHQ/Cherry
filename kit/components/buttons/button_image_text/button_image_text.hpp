#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// ButtonImageText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_IMAGE_TEXT
#define CHERRY_KIT_BUTTON_IMAGE_TEXT

namespace Cherry {
namespace Components {
class ButtonImageText : public Component {
public:
  ButtonImageText(const Cherry::Identifier &id, const std::string &label,
                  const std::string &image_path)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "theme:button_color_border");
    SetProperty("color_border_hovered", "theme:button_color_border_hovered");
    SetProperty("color_border_pressed", "theme:button_color_border_pressed");
    SetProperty("color_bg", "theme:button_color_bg");
    SetProperty("color_bg_hovered", "theme:button_color_bg_hovered");
    SetProperty("color_bg_pressed", "theme:button_color_bg_pressed");
    SetProperty("color_bg_clicked", "theme:button_color_bg_clicked");
    SetProperty("color_text", "theme:button_color_text");
    SetProperty("color_text_hovered", "theme:button_color_text_hovered");
    SetProperty("color_text_pressed", "theme:button_color_text_pressed");

    // Sizes
    SetProperty("size_x", "theme:button_size_x");
    SetProperty("size_y", "theme:button_size_y");
    SetProperty("padding_x", "theme:button_padding_x");
    SetProperty("padding_y", "theme:button_padding_y");

    SetProperty("size_image_x", "15");
    SetProperty("size_image_y", "15");

    // Informations
    SetProperty("label", label);
    SetProperty("image_path", image_path);

    // Data & User-level informations
    SetData("lastClicked", "never");
    SetData("isClicked", "false");
  }

  void Render() override {
    const ImVec2 &size = ImVec2(std::stoi(GetProperty("size_x")),
                                std::stoi(GetProperty("size_y")));
    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                            ImVec2(std::stoi(GetProperty("padding_x")),
                                   std::stoi(GetProperty("padding_y"))));

    CherryGUI::PushStyleColor(ImGuiCol_Text,
                              HexToRGBA(GetProperty("color_text")));
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

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    ImTextureID texture =
        Application::Get().GetCurrentRenderedWindow()->get_texture(
            GetProperty("image_path"));
    CherryGUI::SetNextItemWidth(std::stof(GetProperty("size_x")));
    if (CherryGUI::ImageButtonWithText(
            texture, Label.c_str(),
            ImVec2(std::stof(GetProperty("size_image_x")),
                   std::stof(GetProperty("size_image_y"))))) {
      SetData("isClicked", "true");
      UpdateLastClickTime();
    } else {
      SetData("isClicked", "false");
    }

    CherryGUI::PopStyleColor(5);
    CherryGUI::PopStyleVar();
  }

private:
  void UpdateLastClickTime() {
    std::string m_LastClickTime;
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    m_LastClickTime = std::ctime(&now_c);
    m_LastClickTime.erase(m_LastClickTime.length() - 1);
    SetData("lastClicked", m_LastClickTime);
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &ButtonImageText(const Identifier &identifier,
                                  const std::string &label,
                                  const std::string &image_path) {
  return CherryApp.PushComponent<Cherry::Components::ButtonImageText>(
      identifier, label, image_path);
}

inline Component &ButtonImageText(const std::string &label,
                                  const std::string &image_path) {
  return Cherry::Kit::ButtonImageText(
      Application::GenerateUniqueID(label, image_path, "ButtonImageText"),
      label, image_path);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BUTTON_IMAGE_TEXT
