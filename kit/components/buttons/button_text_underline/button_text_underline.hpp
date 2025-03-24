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
                bool pressed = false;
                int i = 0;

                CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, std::stof(GetProperty("margin_y"))));
                CherryGUI::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, std::stof(GetProperty("margin_y"))));

                CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA("#00000000"));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA("#00000000"));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA("#00000000"));
                CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA("#00000000"));
                CherryGUI::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text")));

                if (CherryGUI::Button(GetProperty("label").c_str(),  ImVec2(std::stoi(GetProperty("size_x")), std::stoi(GetProperty("size_y")))))
                {
                    pressed = true;
                }

                if (CherryGUI::IsItemHovered())
                {
                    CherryGUI::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text")));
                    i++;
                }
                if (CherryGUI::IsItemActive())
                {
                    CherryGUI::PushStyleColor(ImGuiCol_Text, HexToRGBA(GetProperty("color_text")));
                    i++;
                }

                if (CherryGUI::IsItemHovered())
                {
                    ImVec2 pos = CherryGUI::GetItemRectMin();
                    ImVec2 rect_size = CherryGUI::GetItemRectSize();
                    ImDrawList *draw_list = CherryGUI::GetWindowDrawList();
                    ImVec4 underline_color = CherryGUI::IsItemActive() ? HexToRGBA(GetProperty("color_text")) : HexToRGBA(GetProperty("color_text"));
                    draw_list->AddLine(ImVec2(pos.x, pos.y + rect_size.y), ImVec2(pos.x + rect_size.x, pos.y + rect_size.y), ImColor(underline_color));
                }

                CherryGUI::PopStyleVar(2);
                CherryGUI::PopStyleColor(5);
                CherryGUI::PopStyleColor(i);
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
            static std::unordered_map<std::size_t, std::weak_ptr<Component>> component_cache;

            std::size_t label_hash = std::hash<std::string>{}(label);

            if (auto existing = component_cache[label_hash].lock())
            {
                existing->Render();
                return existing;
            }

            auto title = Application::CreateAnonymousComponent<Components::ButtonTextUnderline>(
                Components::ButtonTextUnderline(Cherry::Identifier(std::to_string(label_hash)), label));

            component_cache[label_hash] = title;
            title->Render();
            return title;
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
