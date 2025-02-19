#pragma once
#include "../../../../platform/engine/app.hpp"

//
// TextWrapped
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_WRAPPED
#define CHERRY_KIT_TEXT_WRAPPED

namespace Cherry
{
    namespace Components
    {
        class TextWrapped : public Component
        {
        public:
            TextWrapped(const Cherry::Identifier &id, const std::string &label)
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
                CherryGUI::TextWrapped(GetProperty("label").c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        void TextWrapped(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::TextWrapped>(Components::TextWrapped(Cherry::Identifier(""), label));
            title->Render();
        }

        void TextWrapped(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_title = Application::CreateComponent<Components::TextWrapped>(Components::TextWrapped(identifier, label));
                new_title->Render();
            }
        }
    }

}

#endif // CHERRY_KIT_TEXT_WRAPPED
