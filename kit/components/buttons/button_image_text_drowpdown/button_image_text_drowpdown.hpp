#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"
//
// ButtonImageTextDropdown
// Summary : A dropdown button with the possibility of adding a custom menu.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_IMAGE_TEXT_DROPDOWN
#define CHERRY_KIT_BUTTON_IMAGE_TEXT_DROPDOWN

namespace Cherry {
namespace Components {
class ButtonImageTextDropdown : public Component {
public:
  ButtonImageTextDropdown(const Cherry::Identifier &id,
                          const std::string &label,
                          const std::string &image_path)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Images
    SetProperty("image_dropdown_up", Cherry::GetPath("resources/base/up.png"));
    SetProperty("image_dropdown_down",
                Cherry::GetPath("resources/base/down.png"));

    // Colors
    SetProperty("color_border", "#454545B2");
    SetProperty("color_border_hovered", "#454545B2");
    SetProperty("color_border_clicked", "#454545B2");
    SetProperty("color_bg", "#242424FF");
    SetProperty("color_bg_hovered", "#343434FF");
    SetProperty("color_bg_clicked", "#444444FF");

    // Layout
    SetProperty("dropdown_place", "right");

    // Sizes
    SetProperty("size_x", "15");
    SetProperty("size_y", "15");

    // Informations
    SetProperty("label", label);
    SetProperty("image_path", image_path);

    // Data & User-level informations
    SetData("lastClicked", "never");
    SetData("isClicked", "false");
    SetData("isMenuActivated", "false");
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

    ImTextureID image_texture =
        Application::Get().GetCurrentRenderedWindow()->get_texture(
            GetProperty("image_path"));

    const ImVec2 &size = ImVec2(std::stoi(GetProperty("size_x")),
                                std::stoi(GetProperty("size_y")));
    CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

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

    if (CherryGUI::ImageButtonWithTextWithIcon(image_texture, texture,
                                               Label.c_str(), size, size)) {
      SetData("isClicked", "true");
      SetData("isMenuActivated", "true");
      UpdateLastClickTime();
    } else {
      SetData("isClicked", "false");
    }

    if (GetData("isMenuActivated") == "true") {
      if (CherryGUI::BeginPopupContextItem("TabContextMenu")) {
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

        if (CherryGUI::MenuItem("Close window", "Close this current window")) {
        }

        CherryGUI::PopStyleVar();
        CherryGUI::PopStyleColor(2);
        CherryGUI::EndPopup();
      }
    }

    if (GetData("isMenuActivated") == "true") {
      CherryGUI::OpenPopup("TabContextMenu");
    }

    if (CherryGUI::IsMouseClicked(ImGuiMouseButton_Left)) {
      SetData("isMenuActivated", "false");
    }

    CherryGUI::PopStyleColor(4);
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
inline Component &ButtonImageTextDropdown(const Identifier &identifier,
                                          const std::string &label,
                                          const std::string &image_path) {
  return CherryApp.PushComponent<Cherry::Components::ButtonImageTextDropdown>(
      identifier, label, image_path);
}

inline Component &ButtonImageTextDropdown(const std::string &label,
                                          const std::string &image_path) {
  return Cherry::Kit::ButtonImageTextDropdown(
      Application::GenerateUniqueID(label, image_path), label, image_path);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BUTTON_IMAGE_TEXT_DROPDOWN
