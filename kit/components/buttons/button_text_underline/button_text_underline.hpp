#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ButtonTextUnderline
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BUTTON_TEXT
#define CHERRY_KIT_BUTTON_TEXT

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
                bool pressed = false;
                int i = 0;

                //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, options.y_margin));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, options.y_margin));

                ImVec4 text_idle = HexToRGBA(options.hex_text_idle.c_str());
                ImVec4 text_hovered = HexToRGBA(options.hex_text_hovered.c_str());
                ImVec4 text_clicked = HexToRGBA(options.hex_text_clicked.c_str());
                ImVec4 underline_idle = HexToRGBA(options.hex_underline_idle.c_str());
                ImVec4 underline_hovered = HexToRGBA(options.hex_underline_hovered.c_str());
                ImVec4 underline_clicked = HexToRGBA(options.hex_underline_clicked.c_str());

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_Text, text_idle);

                if (ImGui::Button(GetProperty("label").c_str(), options.size))
                {
                    pressed = true;
                }

                if (ImGui::IsItemHovered())
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, text_hovered);
                    i++;
                }
                if (ImGui::IsItemActive())
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, text_clicked);
                    i++;
                }

                if (ImGui::IsItemHovered())
                {
                    ImVec2 pos = ImGui::GetItemRectMin();
                    ImVec2 rect_size = ImGui::GetItemRectSize();
                    ImDrawList *draw_list = ImGui::GetWindowDrawList();
                    ImVec4 underline_color = ImGui::IsItemActive() ? underline_clicked : underline_hovered;
                    draw_list->AddLine(ImVec2(pos.x, pos.y + rect_size.y), ImVec2(pos.x + rect_size.x, pos.y + rect_size.y), ImColor(underline_color));
                }

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(5);
                ImGui::PopStyleColor(i);
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

#endif // CHERRY_KIT_BUTTON_TEXT
