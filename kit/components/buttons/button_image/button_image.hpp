#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ButtonImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_IMAGE
#define CHERRY_KIT_BUTTON_IMAGE

namespace Cherry
{
    namespace Components
    {
        class ButtonImage : public Component
        {
        public:
            ButtonImage(const Cherry::Identifier &id, const std::string &image_path)
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

                // Sizes
                SetProperty("size_x", "20");
                SetProperty("size_y", "20");
                SetProperty("padding_x", "2");
                SetProperty("padding_y", "2");

                // Informations
                SetProperty("label", "");
                SetProperty("image_path", image_path);

                // Data & User-level informations
                SetData("lastClicked", "never");
                SetData("isClicked", "false");
            }

            void Render() override
            {
                const ImVec2 &size = ImVec2(std::stoi(GetProperty("size_x")), std::stoi(GetProperty("size_y")));
                CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(std::stoi(GetProperty("padding_x")), std::stoi(GetProperty("padding_y"))));

                CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("color_bg_hovered")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("color_bg_clicked")));

                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(GetProperty("image_path"));
                if (ImGui::ImageButtonWithText(texture, Label.c_str(), size))
                {
                    SetData("isClicked", "true");
                    UpdateLastClickTime();
                }
                else
                {
                    SetData("isClicked", "false");
                }

                CherryGUI::PopStyleColor(4);
                CherryGUI::PopStyleVar();
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

    // End-User API
    namespace Kit
    {
        bool ButtonImage(const std::string &image_path)
        {
            // Inline component
            auto button = Application::CreateAnonymousComponent<Components::ButtonImage>(Components::ButtonImage(Cherry::Identifier("anonymous"), image_path));
            button->Render();
            return button->GetData("isClicked") == "true" ? true : false;
        }

        bool ButtonImage(const Cherry::Identifier &identifier, const std::string &image_path)
        {
            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
                return existing_button->GetData("isClicked") == "true" ? true : false;
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::ButtonImage>(Components::ButtonImage(identifier, image_path));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }

}

#endif // CHERRY_KIT_BUTTON_IMAGE
