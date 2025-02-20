#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextSimple
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_SIMPLE
#define CHERRY_KIT_TEXT_SIMPLE

namespace Cherry
{
    namespace Components
    {
        class TextSimple : public Component
        {
        public:
            TextSimple(const Cherry::Identifier &id, const std::string &label)
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
                CherryGUI::Text(GetProperty("label").c_str());

                auto parent = Cherry::GetParent();
                if(parent)
                {
                    CherryGUI::Text(parent->GetProperty("label").c_str());
                }
            }
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> TextSimple(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::TextSimple>(Components::TextSimple(Cherry::Identifier(""), label));
            title->Render();
            return title;
        }

        std::shared_ptr<Component> TextSimple(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_title = Application::CreateComponent<Components::TextSimple>(Components::TextSimple(identifier, label));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_TEXT_SIMPLE
