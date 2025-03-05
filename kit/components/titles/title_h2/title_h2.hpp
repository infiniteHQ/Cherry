#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

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
                SetProperty("color_text", "#FFFFFFFF"); // TODO get the default theme

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                float oldsize = CherryGUI::GetFont()->Scale;
                CherryGUI::GetFont()->Scale *= 1.90; // TODO : property
                CherryGUI::PushFont(CherryGUI::GetFont());

                CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

                CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),GetProperty("label").c_str());

                CherryGUI::PopStyleVar();

                CherryGUI::GetFont()->Scale = oldsize;
                CherryGUI::PopFont();
            }
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> TitleTwo(const std::string &label)
        {
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
            auto title = Application::CreateAnonymousComponent<Components::TitleTwo>(Components::TitleTwo(Cherry::Identifier(""), label));
                title->Render();
                return title;
            }
        }
        std::shared_ptr<Component> TitleTwo(const Cherry::Identifier &identifier, const std::string &label)
        {
            if(identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::TitleTwo>(Components::TitleTwo(identifier, label));
                new_title->Render();
                return new_title;
            }

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
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_TITLE_H2
