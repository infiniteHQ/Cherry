#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextRight
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_RIGHT
#define CHERRY_KIT_TEXT_RIGHT

namespace Cherry
{
    namespace Components
    {
        class TextRight : public Component
        {
        public:
            TextRight(const Cherry::Identifier &id, const std::string &label)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#454545B2");

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                std::string text = GetProperty("label").c_str();
                float windowWidth = ImGui::GetWindowSize().x;
                float textWidth = ImGui::CalcTextSize(text.c_str()).x;

                ImGui::SetCursorPosX(windowWidth - textWidth);

                ImGui::TextWrapped(text.c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> TextRight(const std::string &label)
        {
            // Inline component
            auto text = Application::CreateAnonymousComponent<Components::TextRight>(Components::TextRight(Cherry::Identifier(""), label));
            text->Render();
            return text;
        }

        inline std::shared_ptr<Component> TextRight(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_text = Application::CreateComponent<Components::TextRight>(Components::TextRight(identifier, label));
                new_text->Render();
                return new_text;
            }
            return existing_text;
        }
    }

}

#endif // CHERRY_KIT_TEXT_RIGHT
