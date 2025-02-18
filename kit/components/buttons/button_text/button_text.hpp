#pragma once
#include "../../../../platform/engine/app.hpp"

//
// ButtonText
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

// API
// CherryKit::ButtonText(label)             -> The most simple approch to make a inline button with text
// CherryKit::ButtonText(identifier, label) -> The most simple approch to make a component button with text

// Datas
////////////////////////////////////////////////////////////////////////////////////////////|
//          Data name               |               Description of property                 |
////////////////////////////////////////////////////////////////////////////////////////////|
//   isPressed                      | Is the button pressed (constantly) ? Return "true"    |
//                                  | or "false"                                            |
//                                  |                                                       |
//   isClicked                      | Is the button clicked ? Return "true" or "false"      |
//                                  |                                                       |
//   lastClicked                    | Last time the button was clicked ?                    |
//                                  | Return the date formated in "DD-HH-MM-SS-MMM"         |
//                                  |                                                       |
////////////////////////////////////////////////////////////////////////////////////////////|

// Properties
////////////////////////////////////////////////////////////////////////////////////////////|
//          Property name           |               Description of property                 |
////////////////////////////////////////////////////////////////////////////////////////////|
//   color_border                   | Color of the button border                            |
//                                  |                                                       |
//   color_border_hovered           | Color of the button border when hovered by mouse      |
//                                  |                                                       |
//   color_border_clicked           | Color of the button border when clicked               |
//                                  |                                                       |
//   color_bg                       | Color of the button backgound                         |
//                                  |                                                       |
//   color_bg_hovered               | Color of the button backgound when hovered by mouse   |
//                                  |                                                       |
//   color_bg_clicked               | Color of the button backgound when clicked            |
//                                  |                                                       |
////////////////////////////////////////////////////////////////////////////////////////////|


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
                SetData("isClicked", "false");
            }

            void Render() override
            {
                const ImVec2 &size = ImVec2(0, 0);
                CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

                CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("color_bg_hovered")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("color_bg_clicked")));

                std::string Label = GetProperty("label") + "####" + GetIdentifier().string();

                if (CherryGUI::Button(Label.c_str(), size))
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
        bool ButtonText(const std::string &label)
        {
            // Inline component
            auto button = Application::CreateAnonymousComponent<Components::ButtonText>(Components::ButtonText(Cherry::Identifier("anonymous"), label));
            button->Render();
            return button->GetData("isClicked") == "true" ? true : false;
        }

        bool ButtonText(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_button = Application::CreateComponent<Components::ButtonText>(Components::ButtonText(identifier, label));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }

}

#endif // CHERRY_KIT_BUTTON_TEXT
