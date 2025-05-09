#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextLeft
// Summary : A big text.
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
                CherryGUI::TextWrapped(text.c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> TextLeft(const std::string &label)
        {
            // Inline component
            auto anonymous_id = Application::GenerateUniqueID(label);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {

                auto button = Application::CreateAnonymousComponent<Components::TextLeft>(Components::TextLeft(anonymous_id, label));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> TextLeft(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_text = Application::CreateComponent<Components::TextLeft>(Components::TextLeft(identifier, label));
                new_text->Render();
            }
        }
    }

}

#endif // CHERRY_KIT_TEXT_LEFT
