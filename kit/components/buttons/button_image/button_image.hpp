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
                SetProperty("color_border", "#454545FF");
                SetProperty("color_border_hovered", "#555555FF");
                SetProperty("color_border_pressed", "#757575FF");
                SetProperty("color_bg", "#242424FF");
                SetProperty("color_bg_hovered", "#343434FF");
                SetProperty("color_bg_pressed", "#444444FF");
                SetProperty("color_text", "#BCBCBCFF");
                SetProperty("color_text_hovered", "#FFFFFFFF");
                SetProperty("color_text_pressed", "#FFFFFFFF");

                // Sizes
                SetProperty("size_x", "0");
                SetProperty("size_y", "0");
                SetProperty("padding_x", "6");
                SetProperty("padding_y", "6");
                SetProperty("scale", "0"); // Instead of using sizes manually, we can use scale.

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
                int style_props_opt = 0;

                if (GetData("isHovered") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastHovered", GetCurrentTime());
                    ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border_hovered")));
                    ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_bg_hovered")));
                    ImGui::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text_hovered")));
                    style_props_opt += 3;
                }

                if (GetData("isClicked") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastClicked", GetCurrentTime());

                    ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border_pressed")));
                    ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_bg_pressed")));
                    ImGui::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text_pressed")));
                    style_props_opt += 3;
                }

                if (GetData("isActivated") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastActivated", GetCurrentTime());
                }

                if (GetProperty("isPressed") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastPressed", GetCurrentTime());

                    ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border_pressed")));
                    ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_bg_pressed")));
                    ImGui::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text_pressed")));
                    style_props_opt += 3;
                }

                SetData("isHovered", "false");
                SetData("isClicked", "false");
                SetData("isPressed", "false");
                SetData("isActivated", "false");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(GetProperty("image_path"));
                bool isClicked = ImGui::ImageButtonWithText(texture, Label.c_str(), size);

                if (ImGui::IsItemHovered())
                {
                    SetData("isHovered", "true");
                }

                if (ImGui::IsItemActivated())
                {
                    SetData("isActivated", "true");
                }

                if (ImGui::IsItemActive())
                {
                    SetData("isPressed", "true");
                }

                if (isClicked)
                {
                    SetData("isClicked", "true");
                    SetData("isMenuActivated", "true");
                }
                else
                {
                    SetData("isClicked", "false");
                }

                ImGui::PopStyleColor(style_props_opt);
                CherryGUI::PopStyleColor(4);
                CherryGUI::PopStyleVar();
            }

        private:
            std::string GetCurrentTime()
            {
                std::string m_LastClickTime;
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                m_LastClickTime = std::ctime(&now_c);
                m_LastClickTime.erase(m_LastClickTime.length() - 1);
                return m_LastClickTime;
            }
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> ButtonImage(const std::string &image_path)
        {
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto title = Application::CreateAnonymousComponent<Components::ButtonImage>(Components::ButtonImage(Cherry::Identifier(""), image_path));
                title->Render();
                return title;
            }
        }

        std::shared_ptr<Component> ButtonImage(const Cherry::Identifier &identifier, const std::string &image_path)
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::ButtonImage>(Components::ButtonImage(identifier, image_path));
                new_title->Render();
                return new_title;
            }

            // Get the object if exist
            auto existing_title = Application::GetComponent(identifier);
            if (existing_title)
            {
                existing_title->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_title = Application::CreateComponent<Components::ButtonImage>(Components::ButtonImage(identifier, image_path));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_BUTTON_IMAGE
