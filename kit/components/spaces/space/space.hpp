#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// Space
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_Space
#define CHERRY_KIT_Space

namespace Cherry
{
    namespace Components
    {
        class Space : public Component
        {
        public:
            Space(const Cherry::Identifier &id, const float &spacing)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color", "#454545B2");
                SetProperty("spacing", std::to_string(spacing));
            }

            void Render() override
            {
                CherryGUI::BeginDisabled();
                CherryGUI::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
                CherryGUI::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
                CherryGUI::Button("", ImVec2(0, std::stof(GetProperty("spacing"))));
                CherryGUI::PopStyleColor(2);
                CherryGUI::EndDisabled();
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> Space(const float &spacing)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::Space>(Components::Space(Cherry::Identifier("anonymous"), spacing));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> Space(const Cherry::Identifier &identifier, const float &spacing)
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
                auto new_title = Application::CreateComponent<Components::Space>(Components::Space(identifier, spacing));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_Space
