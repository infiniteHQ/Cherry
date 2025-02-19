#pragma once
#include "../../../../platform/engine/app.hpp"

//
// TitleFour
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H4
#define CHERRY_KIT_TITLE_H4

namespace Cherry
{
    namespace Components
    {
        class TitleFour : public Component
        {
        public:
            TitleFour(const Cherry::Identifier &id, const std::string &label)
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
                float oldsize = CherryGUI::GetFont()->Scale;
                CherryGUI::GetFont()->Scale *= 1.50; // TODO : property
                CherryGUI::PushFont(CherryGUI::GetFont());

                CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

                CherryGUI::Text(GetProperty("label").c_str());

                CherryGUI::PopStyleVar();

                CherryGUI::GetFont()->Scale = oldsize;
                CherryGUI::PopFont();
            }
        };
    }

    // End-User API
    namespace Kit
    {
        void TitleFour(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::TitleFour>(Components::TitleFour(Cherry::Identifier(""), label));
            title->Render();
        }

        void TitleFour(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_title = Application::CreateComponent<Components::TitleFour>(Components::TitleFour(identifier, label));
                new_title->Render();
            }
        }
    }

}

#endif // CHERRY_KIT_TITLE_H4
