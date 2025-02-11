#pragma once
#include "../../../../platform/engine/app.hpp"

#ifndef CHERRY_KIT_BUTTON_TEXT
#define CHERRY_KIT_BUTTON_TEXT

namespace Cherry
{
    namespace Components
    {
        class ButtonText : public Component
        {
        public:
            ButtonText(const Cherry::Identifier &id, const std::string &label)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_border", "#454545B2");
                SetProperty("color_border_hovered", "#454545B2");
                SetProperty("color_border_clicked", "#454545B2");
                SetProperty("color_bg", "#242424FF");
                SetProperty("color_bg_hovered", "#343434FF");
                SetProperty("color_bg_clicked", "#444444FF");

                // Informations
                SetProperty("label", label);

                // Data & User-level informations
                SetData("lastClicked", "never");
                SetData("isPressed", "false");
            }

            void Render() override
            {
                const ImVec2 &size = ImVec2(0, 0);

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

                ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border")));
                ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_border")));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("color_bg_hovered")));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("color_bg_clicked")));

                std::string Label = GetProperty("label") + "####" + GetIdentifier().string();

                if (ImGui::Button(Label.c_str(), size))
                {
                    SetData("isPressed", "true");
                    UpdateLastClickTime();
                }
                else
                {
                    SetData("isPressed", "false");
                }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
            }

        private:
            void UpdateLastClickTime()
            {
                std::string m_LastClickTime;
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                m_LastClickTime = std::ctime(&now_c);
                m_LastClickTime.erase(m_LastClickTime.length() - 1);
                SetData("lastClicked", m_LastClickTime);
            }
        };

    }

    namespace Kit
    {
        bool ButtonText(const std::string &label)
        {
            // Inline component
            auto button = Application::CreateAnonymousComponent<Components::ButtonText>(Components::ButtonText(Cherry::Identifier("anonymous"), label));
            button->Render();
            return button->GetData("isPressed") == "true" ? true : false;
        }

        bool ButtonText(const Cherry::Identifier &identifier, const std::string &label)
        {
            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
                return existing_button->GetData("isPressed") == "true" ? true : false;
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::ButtonText>(Components::ButtonText(identifier, label));
                new_button->Render();
                return new_button->GetData("isPressed") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_BUTTON_TEXT
