#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// NotificationButton
//

#ifndef CHERRY_KIT_NOTIFICATION_BUTTON
#define CHERRY_KIT_NOTIFICATION_BUTTON

namespace Cherry {
namespace Components {
class NotificationButton : public Component {
public:
  NotificationButton(
      const Cherry::Identifier &id, bool *activate, int timer_in_seconds = 8,
      const std::string &type = "info",
      const std::string &title = "Super Notification",
      const std::string &description = "This is a notification !",
      const std::function<void()> &button_callback = []() {})
      : Component(id), m_Activate(activate), m_TimerSeconds(timer_in_seconds),
        m_ButtonCallback(button_callback) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "#454545FF");
    SetProperty("color_border_hovered", "#555555FF");
    SetProperty("color_bg", "#242424FF");
    SetProperty("color_bg_hovered", "#343434FF");

    SetProperty("icon", "undefined");
    SetProperty("close_button", "false");

    SetProperty("type", type);
    SetProperty("title", title);
    SetProperty("description", description);
  }

  void Render() override {
    if (m_Activate != nullptr) {
      if (*m_Activate) {
        int milliseconds_timer = m_TimerSeconds * 1000;

        std::string path = Cherry::GetPath("resources/base/info.png");

        if (GetProperty("type") == "error") {
          path = Cherry::GetPath("resources/base/error.png");
        } else if (GetProperty("type") == "warn") {
          path = Cherry::GetPath("resources/base/warn.png");
        }

        std::string custom_icon = GetProperty("icon");
        if (custom_icon != "undefined") {
          path = custom_icon;
        }

        ImVec2 buttonSize(15, 15);
        ImTextureID texture =
            Application::Get().GetCurrentRenderedWindow()->get_texture(
                Cherry::GetPath("resources/imgs/icons/misc/icon_close.png"));

        ImGuiToast toast(
            ImGuiToastType::Info, milliseconds_timer,
            m_ButtonCallback, // Button callback
            [=]() {
              if (GetProperty("close_button") == "true") {
                CherryGUI::PushStyleColor(ImGuiCol_Button,
                                          Cherry::HexToRGBA("#00000000"));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered,
                                          Cherry::HexToRGBA("#00000000"));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonActive,
                                          Cherry::HexToRGBA("#00000000"));
                CherryGUI::PushStyleColor(ImGuiCol_Border,
                                          Cherry::HexToRGBA("#00000000"));
                CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                                        ImVec2(0, 0));
                CherryGUI::PushStyleVar(ImGuiStyleVar_WindowPadding,
                                        ImVec2(0, 0));
                bool isClicked = false;
                if (CherryGUI::ImageButtonWithText(texture, "###CloseBtn",
                                                   buttonSize)) {
                  isClicked = true;
                }

                CherryGUI::PopStyleVar(2);
                CherryGUI::PopStyleColor(4);
                return isClicked;
              } else {
                return false;
              }
            },       // Close button callback
            nullptr, // Render callback
            Cherry::GetTexture(path));

        toast.setTitle(GetProperty("title").c_str());
        toast.setContent(GetProperty("description").c_str());
        toast.setOnButtonPress([]() {});

        toast.setButtonLabel("OK");

        Cherry::AddNotification(toast);
        *m_Activate = false;
      }
    }
  }

private:
  bool *m_Activate;
  int m_TimerSeconds;
  std::function<void()> m_ButtonCallback;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &NotificationButton(
    const Identifier &identifier, bool *activate, int timer_in_seconds = 8,
    const std::string &type = "info",
    const std::string &title = "Super Notification",
    const std::string &description = "This is a notification !",
    const std::function<void()> &button_callback = []() {}) {
  return CherryApp.PushComponent<Cherry::Components::NotificationButton>(
      identifier, activate, timer_in_seconds, type, title, description,
      button_callback);
}

inline Component &NotificationButton(
    bool *activate, int timer_in_seconds = 8, const std::string &type = "info",
    const std::string &title = "Super Notification",
    const std::string &description = "This is a notification !",
    const std::function<void()> &button_callback = []() {}) {
  return Cherry::Kit::NotificationButton(
      Application::GenerateUniqueID(activate, timer_in_seconds, type, title,
                                    description, button_callback),
      activate, timer_in_seconds, type, title, description, button_callback);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_NOTIFICATION_BUTTON
