#pragma once
#include "../../../../platform/engine/app.hpp"

//
// TextLeft
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_LEFT
#define CHERRY_KIT_TEXT_LEFT

namespace Cherry
{
    namespace Components
    {
        class TextLeft : public Component
        {
        public:
            TextLeft(const Cherry::Identifier &id, const std::string &label)
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
                ImGui::TextWrapped(text.c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        void TextLeft(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::TextLeft>(Components::TextLeft(Cherry::Identifier(""), label));
            title->Render();
        }

        void TextLeft(const Cherry::Identifier &identifier, const std::string &label)
        {
            // Get the object if exist
            auto existing_title = Application::GetComponent(identifier);
            if (existing_title)
            {
                existing_title->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_title = Application::CreateComponent<Components::TextLeft>(Components::TextLeft(identifier, label));
                new_title->Render();
            }
        }
    }

}

#endif // CHERRY_KIT_TEXT_LEFT
