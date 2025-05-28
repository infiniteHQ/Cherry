#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ButtonImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_IMAGE
#define CHERRY_KIT_BUTTON_IMAGE

namespace Cherry {
  namespace Components {
    class ButtonImage : public Component {
     public:
      ButtonImage(const Cherry::Identifier &id, const std::string &image_path) : Component(id) {
        // Identifier
        SetIdentifier(id);

        // Colors
        SetProperty("color_border", "#454545B2");
        SetProperty("color_border_hovered", "#454545B2");
        SetProperty("color_border_clicked", "#454545B2");
        SetProperty("color_bg", "#242424FF");
        SetProperty("color_bg_hovered", "#343434FF");
        SetProperty("color_bg_clicked", "#444444FF");
        SetProperty("color_text", "#BCBCBCFF");
        SetProperty("color_text_hovered", "#FFFFFFFF");
        SetProperty("color_text_pressed", "#FFFFFFFF");

        // Sizes
        SetProperty("size_x", "15");
        SetProperty("size_y", "15");
        SetProperty("size_image_x", "15");
        SetProperty("size_image_y", "15");
        SetProperty("padding_x", "10");
        SetProperty("padding_y", "6");

        // Params
        SetProperty("disabled", "false");
        SetProperty("disable_time", "false");

        // Informations
        SetProperty("label", "");
        SetProperty("image_path", image_path);

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
        std::cout << "FQq" << std::endl;
        const ImVec2 &size = ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y")));
        CherryGUI::PushStyleVar(
            ImGuiStyleVar_FramePadding, ImVec2(std::stof(GetProperty("padding_x")), std::stof(GetProperty("padding_y"))));
        std::cout << "FQq" << std::endl;

        CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border")));
        CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_bg")));
        CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("color_bg_hovered")));
        CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("color_bg_clicked")));

        std::cout << "FQq8" << std::endl;
        std::string identifier = GetIdentifier().string();
        std::string Label = GetProperty("label");
        int style_props_opt = 0;

        std::cout << "FQq" << std::endl;
        if (GetData("isHovered") == "true") {
          if (GetProperty("disable_time") == "false")
            SetData("lastHovered", GetCurrentTime());
          CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border_hovered")));
          CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_bg_hovered")));
          CherryGUI::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text_hovered")));
          style_props_opt += 3;
        }
        std::cout << "FQq9" << std::endl;

        if (GetData("isClicked") == "true") {
          if (GetProperty("disable_time") == "false")
            SetData("lastClicked", GetCurrentTime());

          CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border_pressed")));
          CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_bg_pressed")));
          CherryGUI::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text_pressed")));
          style_props_opt += 3;
        }

        std::cout << "FQq6" << std::endl;
        if (GetData("isActivated") == "true") {
          if (GetProperty("disable_time") == "false")
            SetData("lastActivated", GetCurrentTime());
        }

        std::cout << "FQq7" << std::endl;
        if (GetProperty("isPressed") == "true") {
          if (GetProperty("disable_time") == "false")
            SetData("lastPressed", GetCurrentTime());

          CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border_pressed")));
          CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_bg_pressed")));
          CherryGUI::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text_pressed")));
          style_props_opt += 3;
        }
        std::cout << "FQq5" << std::endl;

        SetData("isHovered", "false");
        SetData("isClicked", "false");
        SetData("isPressed", "false");
        SetData("isActivated", "false");
        std::cout << "FQq" << std::endl;

        if (!identifier.empty()) {
          Label += "####" + identifier;
        }

        ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(GetProperty("image_path"));
        bool isClicked = CherryGUI::ImageButtonWithText(texture, Label.c_str(), size);

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
        std::cout << "FQqe" << std::endl;
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
  }  // namespace Components

  // End-User API
  namespace Kit {
    inline std::shared_ptr<Component> ButtonImage(const std::string &image_path) {
      auto anonymous_id = Application::GetAnonymousID();
      auto existing = Application::GetAnonymousComponent(anonymous_id);
      if (existing) {
        existing->Render();
        return existing;
      } else {
        auto button = Application::CreateAnonymousComponent<Components::ButtonImage>(
            Components::ButtonImage(anonymous_id, image_path));
        button->Render();
        return button;
      }
    }

    inline std::shared_ptr<Component> ButtonImage(const Cherry::Identifier &identifier, const std::string &image_path) {
      if (identifier.string() == "__inline") {
        auto new_button =
            Application::CreateAnonymousComponent<Components::ButtonImage>(Components::ButtonImage(identifier, image_path));
        new_button->Render();
        return new_button;
      }

      // Get the object if exist
      auto existing_button = Application::GetComponent(identifier);
      if (existing_button) {
        existing_button->Render();
      } else {
        // Create the object if not exist
        auto new_button =
            Application::CreateComponent<Components::ButtonImage>(Components::ButtonImage(identifier, image_path));
        new_button->Render();
        return new_button;
      }
      return existing_button;
    }
  }  // namespace Kit

}  // namespace Cherry

#endif  // CHERRY_KIT_BUTTON_IMAGE
