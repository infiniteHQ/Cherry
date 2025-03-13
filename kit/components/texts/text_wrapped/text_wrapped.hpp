#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextWrapped
// Summary : A big text.
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
        inline std::shared_ptr<Component> TextWrapped(const std::string &label)
        {
            // Inline component
            auto text = Application::CreateAnonymousComponent<Components::TextWrapped>(Components::TextWrapped(Cherry::Identifier(""), label));
            text->Render();
            return text;
        }

        inline std::shared_ptr<Component> TextWrapped(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_text = Application::CreateComponent<Components::TextWrapped>(Components::TextWrapped(identifier, label));
                new_text->Render();
                return new_text;
            }
            return existing_text;
        }
    }

}

#endif // CHERRY_KIT_TEXT_WRAPPED
