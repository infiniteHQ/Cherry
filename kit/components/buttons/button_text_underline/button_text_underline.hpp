#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ButtonTextUnderline
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_UNDERLINE
#define CHERRY_KIT_BUTTON_UNDERLINE

namespace Cherry
{
    namespace Components
    {
        class ButtonTextUnderline : public Component
        {
        public:
            ButtonTextUnderline(const Cherry::Identifier &id, const std::string &label)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_underline", "#242424FF");
                SetProperty("color_underline_hovered", "#343434FF");
                SetProperty("color_underline_pressed", "#444444FF");
                SetProperty("color_text", "#BCBCBCFF");
                SetProperty("color_text_hovered", "#FFFFFFFF");
                SetProperty("color_text_pressed", "#FFFFFFFF");

                // Sizes
                SetProperty("margin_y", "2");
                SetProperty("size_y", "0");
                SetProperty("size_x", "0");


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

            void Render() override
            {
                std::cout << "fg" << std::endl;
                bool pressed = false;
                int i = 0;

                std::cout << "fgd" << std::endl;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, std::stof(GetProperty("margin_y"))));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, std::stof(GetProperty("margin_y"))));

                std::cout << "fgg" << std::endl;
                ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("#00000000")));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("00000000")));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("00000000")));
                ImGui::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_text")));
                ImGui::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text")));

                std::cout << "fg2" << std::endl;
                if (ImGui::Button(GetProperty("label").c_str(),  ImVec2(std::stoi(GetProperty("size_x")), std::stoi(GetProperty("size_y")))))
                {
                    pressed = true;
                }
                std::cout << "fg3" << std::endl;

                if (ImGui::IsItemHovered())
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text")));
                    i++;
                }
                if (ImGui::IsItemActive())
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text")));
                    i++;
                }

                if (ImGui::IsItemHovered())
                {
                    ImVec2 pos = ImGui::GetItemRectMin();
                    ImVec2 rect_size = ImGui::GetItemRectSize();
                    ImDrawList *draw_list = ImGui::GetWindowDrawList();
                    ImVec4 underline_color = ImGui::IsItemActive() ? HexToRGBA(GetProperty("color_text")) : HexToRGBA(GetProperty("color_text"));
                    draw_list->AddLine(ImVec2(pos.x, pos.y + rect_size.y), ImVec2(pos.x + rect_size.x, pos.y + rect_size.y), ImColor(underline_color));
                }

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(5);
                ImGui::PopStyleColor(i);
                std::cout << "fg" << std::endl;
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
        inline std::shared_ptr<Component> ButtonTextUnderline(const std::string &label)
        {
            auto anonymous_id = Application::GetAnonymousID(label);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::ButtonTextUnderline>(Components::ButtonTextUnderline(Cherry::Identifier(""), label));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> ButtonTextUnderline(const Cherry::Identifier &identifier, const std::string &label)
        {
            if (identifier.string() == "__inline")
            {
                auto new_button = std::make_shared<Components::ButtonTextUnderline>(Components::ButtonTextUnderline(identifier, label));
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
                auto new_button = Application::CreateComponent<Components::ButtonTextUnderline>(Components::ButtonTextUnderline(identifier, label));
                new_button->Render();
                return new_button;
            }
            return existing_button;
        }
    }

}

#endif // CHERRY_KIT_BUTTON_UNDERLINE
