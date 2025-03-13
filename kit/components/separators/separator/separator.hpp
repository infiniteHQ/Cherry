#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// Separator
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_SEPARATOR
#define CHERRY_KIT_SEPARATOR

namespace Cherry
{
    namespace Components
    {
        class Separator : public Component
        {
        public:
            Separator(const Cherry::Identifier &id)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color", "#454545B2");
            }

            void Render() override
            {
                ImGui::Separator();
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> Separator()
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::Separator>(Components::Separator(Cherry::Identifier("anonymous")));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> Separator(const Cherry::Identifier &identifier)
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
                auto new_title = Application::CreateComponent<Components::Separator>(Components::Separator(identifier));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_SEPARATOR
