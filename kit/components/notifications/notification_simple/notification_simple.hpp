#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// NotificationSimple
//

#ifndef CHERRY_KIT_NOTIFICATION_SIMPLE
#define CHERRY_KIT_NOTIFICATION_SIMPLE

namespace Cherry
{
    namespace Components
    {
        class NotificationSimple : public Component
        {
        public:
            NotificationSimple(const Cherry::Identifier &id, bool *activate, int timer_in_seconds = 8, const std::string& type = "info", const std::string& title = "Super Notification", const std::string& description = "This is a notification !")
                : Component(id), m_Activate(activate), m_TimerSeconds(timer_in_seconds)
            {
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

            void Render() override
            {
                if (m_Activate != nullptr)
                {
                    if (*m_Activate)
                    {
                        int milliseconds_timer = m_TimerSeconds * 1000;

                        std::string path = Cherry::GetPath("resources/base/info.png");

                        if(GetProperty("type") == "error")
                        {
                            path = Cherry::GetPath("resources/base/error.png");
                        }
                        else if(GetProperty("type") == "warn")
                        {
                            path = Cherry::GetPath("resources/base/warn.png");
                        }

                        std::string custom_icon = GetProperty("icon");
                        if(custom_icon != "undefined")
                        {
                            path = custom_icon;
                        }

                        ImVec2 buttonSize(15,15);
                        ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(Cherry::GetPath("resources/imgs/icons/misc/icon_close.png"));

                        ImGuiToast toast(ImGuiToastType::Info, milliseconds_timer,
                        [](){}, // Button callback
                        [=]()
                        {
                            if(GetProperty("close_button") == "true")
                            {
                                ImGui::PushStyleColor(ImGuiCol_Button, Cherry::HexToRGBA("#00000000"));
                                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Cherry::HexToRGBA("#00000000"));
                                ImGui::PushStyleColor(ImGuiCol_ButtonActive, Cherry::HexToRGBA("#00000000"));
                                ImGui::PushStyleColor(ImGuiCol_Border, Cherry::HexToRGBA("#00000000"));
                                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
                                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
                                bool isClicked = false;
                                if(CherryGUI::ImageButtonWithText(texture, "###CloseBtn", buttonSize))
                                {
                                    isClicked = true;
                                }

                                ImGui::PopStyleVar(2);
                                ImGui::PopStyleColor(4);
                                return isClicked;
                            }
                            else
                            {
                                return false;
                            }
                        }, // Close button callback
                        nullptr, // Render callback
                        Cherry::GetTexture(path));
                        
                        toast.setTitle(GetProperty("title").c_str());
                        toast.setContent(GetProperty("description").c_str());
                        toast.setOnButtonPress([](){});

                        toast.setButtonLabel("OK");

                        Cherry::AddNotification(toast);
                        *m_Activate = false;
                    }
                }
            }

        private:
            bool *m_Activate;
            int m_TimerSeconds;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> NotificationSimple(bool *activate, int timer_in_seconds = 8, const std::string& type = "info", const std::string& title = "Super Notification", const std::string& description = "This is a notification !")
        {
            auto anonymous_id = Application::GenerateUniqueID(activate, timer_in_seconds, title, description);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::NotificationSimple>(Components::NotificationSimple(anonymous_id, activate, timer_in_seconds, type, title, description));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> NotificationSimple(const Cherry::Identifier &identifier, bool *activate, int timer_in_seconds = 8, const std::string& type = "info", const std::string& title = "Super Notification", const std::string& description = "This is a notification !")
        {
            if (identifier.string() == "__inline")
            {
                auto new_button = Application::CreateAnonymousComponent<Components::NotificationSimple>(Components::NotificationSimple(identifier, activate, timer_in_seconds, type, title, description));
                new_button->Render();
                return new_button;
            }

            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::NotificationSimple>(Components::NotificationSimple(identifier, activate, timer_in_seconds, type, title, description));
                new_button->Render();
                return new_button;
            }
            return existing_button;
        }
    }

}

#endif // CHERRY_KIT_NOTIFICATION_SIMPLE
