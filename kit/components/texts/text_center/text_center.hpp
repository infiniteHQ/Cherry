#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextCenter
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_CENTER
#define CHERRY_KIT_TEXT_CENTER

namespace Cherry
{
    namespace Components
    {
        class TextCenter : public Component
        {
        public:
            TextCenter(const Cherry::Identifier &id, const std::string &label)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#FFFFFFFF");

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                ImVec2 text_size = CherryGUI::CalcTextSize(GetProperty("label").c_str());

                ImVec2 child_size = CherryGUI::GetWindowSize();
                float posX = (child_size.x - text_size.x) * 0.5f;

                posX = posX > 0 ? posX : 0;

                CherryGUI::SetCursorPosX(posX);

                CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")), GetProperty("label").c_str());

                auto parent = Cherry::GetParent();
                if (parent)
                {
                    ImVec2 text_size = CherryGUI::CalcTextSize(GetProperty("label").c_str());

                    ImVec2 child_size = CherryGUI::GetWindowSize();
                    float posX = (child_size.x - text_size.x) * 0.5f;

                    posX = posX > 0 ? posX : 0;

                    CherryGUI::SetCursorPosX(posX);

                    CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")), GetProperty("label").c_str());
                }
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> TextCenter(const std::string &label)
        {
            // Inline component
            auto text = Application::CreateAnonymousComponent<Components::TextCenter>(Components::TextCenter(Cherry::Identifier(""), label));
            text->Render();
            return text;
        }

        inline std::shared_ptr<Component> TextCenter(const Cherry::Identifier &identifier, const std::string &label)
        {
            // Get the object if exist
            auto existing_text = Application::GetComponent(identifier);
            if (existing_text)
            {
                existing_text->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_text = Application::CreateComponent<Components::TextCenter>(Components::TextCenter(identifier, label));
                new_text->Render();
                return new_text;
            }
            return existing_text;
        }
    }

}

#endif // CHERRY_KIT_TEXT_CENTER
