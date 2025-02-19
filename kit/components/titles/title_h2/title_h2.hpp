#pragma once
#include "../../../../platform/engine/app.hpp"

//
// TitleTwo
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H2
#define CHERRY_KIT_TITLE_H2

namespace Cherry
{
    namespace Components
    {
        class TitleTwo : public Component
        {
        public:
            TitleTwo(const Cherry::Identifier &id, const std::string &label)
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
                CherryGUI::GetFont()->Scale *= 1.90; // TODO : property
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
        void TitleTwo(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::TitleTwo>(Components::TitleTwo(Cherry::Identifier(""), label));
            title->Render();
        }

        void TitleTwo(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_title = Application::CreateComponent<Components::TitleTwo>(Components::TitleTwo(identifier, label));
                new_title->Render();
            }
        }
    }

}

#endif // CHERRY_KIT_TITLE_H2
