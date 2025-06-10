#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ButtonText
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_TEXT
#define CHERRY_KIT_BUTTON_TEXT

namespace Cherry {
namespace Components {
class ButtonText : public Component {
public:
  ButtonText(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    // SetProperty("color_border", "#454545FF");
    // SetProperty("color_border_hovered", "#555555FF");
    // SetProperty("color_border_pressed", "#757575FF");
    // SetProperty("color_bg", "#242424FF");
    // SetProperty("color_bg_hovered", "#343434FF");
    // SetProperty("color_bg_pressed", "#444444FF");
    // SetProperty("color_text", "#BCBCBCFF");
    // SetProperty("color_text_hovered", "#FFFFFFFF");
    // SetProperty("color_text_pressed", "#FFFFFFFF");
    SetProperty("color_border", "theme:color_border");
    SetProperty("color_border_hovered", "theme:color_border_hovered");
    SetProperty("color_border_pressed", "theme:color_border_pressed");
    SetProperty("color_bg", "theme:color_bg");
    SetProperty("color_bg_hovered", "theme:color_bg_hovered");
    SetProperty("color_bg_pressed", "theme:color_bg_pressed");
    SetProperty("color_text", "theme:color_text");
    SetProperty("color_text_hovered", "theme:color_text_hovered");
    SetProperty("color_text_pressed", "theme:color_text_pressed");

    // Sizes
    SetProperty("size_x", "0");
    SetProperty("size_y", "0");
    SetProperty("padding_x", "6");
    SetProperty("padding_y", "6");
    SetProperty("scale",
                "0"); // Instead of using sizes manually, we can use scale.

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

    std::cout << "1" << std::endl;
    std::cout << GetProperty("color_border") << std::endl;
    std::cout << "1" << std::endl;
    CherryGUI::PushStyleColor(ImGuiCol_Border,
                              HexToRGBA(GetProperty("color_border")));
    std::cout << "2" << std::endl;
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

    bool isClicked = CherryGUI::Button(Label.c_str(), size);

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
inline std::shared_ptr<Component> ButtonText(const std::string &label) {
  auto anonymous_id = Application::GenerateUniqueID(label);
  auto existing = Application::GetAnonymousComponent(anonymous_id);
  if (existing) {
    existing->Render();
    return existing;
  } else {
    auto button = Application::CreateAnonymousComponent<Components::ButtonText>(
        Components::ButtonText(anonymous_id, label));
    button->Render();
    return button;
  }
}

inline std::shared_ptr<Component>
ButtonText(const Cherry::Identifier &identifier, const std::string &label) {
  if (identifier.string() == "__inline") {
    auto new_button = std::make_shared<Components::ButtonText>(
        Components::ButtonText(identifier, label));
    new_button->Render();
    return new_button;
  }

  // Get the object if exist
  auto existing_button = Application::GetComponent(identifier);
  if (existing_button) {
    existing_button->Render();
  } else {
    // Create the object if not exist
    auto new_button = Application::CreateComponent<Components::ButtonText>(
        Components::ButtonText(identifier, label));
    new_button->Render();
    return new_button;
  }
  return existing_button;
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BUTTON_TEXT
