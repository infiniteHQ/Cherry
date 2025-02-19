#pragma once
#include "../../../../platform/engine/app.hpp"

//
// TitleSix
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H6
#define CHERRY_KIT_TITLE_H6

namespace Cherry
{
    namespace Components
    {
        class TitleSix : public Component
        {
        public:
            TitleSix(const Cherry::Identifier &id, const std::string &label)
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
                CherryGUI::GetFont()->Scale *= 1.10; // TODO : property
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
        std::shared_ptr<Component> TitleSix(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::TitleSix>(Components::TitleSix(Cherry::Identifier(""), label));
            title->Render();
            return title;
        }

        std::shared_ptr<Component> TitleSix(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_title = Application::CreateComponent<Components::TitleSix>(Components::TitleSix(identifier, label));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_TITLE_H6
