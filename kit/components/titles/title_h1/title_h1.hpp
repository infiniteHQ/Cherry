#pragma once
#include "../../../../platform/engine/app.hpp"

//
// TitleOne
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H1
#define CHERRY_KIT_TITLE_H1

namespace Cherry
{
    namespace Components
    {
        class TitleOne : public Component
        {
        public:
            TitleOne(const Cherry::Identifier &id, const std::string &label)
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
                float oldsize = ImGui::GetFont()->Scale;
                ImGui::GetFont()->Scale *= 2.10;
                ImGui::PushFont(ImGui::GetFont());

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

                ImGui::Text(GetProperty("label").c_str());

                ImGui::PopStyleVar();

                ImGui::GetFont()->Scale = oldsize;
                ImGui::PopFont();
            }
        };
    }

    // End-User API
    namespace Kit
    {
        bool TitleOne(const std::string &label)
        {
            // Inline component
            auto button = Application::CreateAnonymousComponent<Components::TitleOne>(Components::TitleOne(Cherry::Identifier(""), label));
            button->Render();
            return button->GetData("isClicked") == "true" ? true : false;
        }

        bool TitleOne(const Cherry::Identifier &identifier, const std::string &label)
        {
            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
                return existing_button->GetData("isClicked") == "true" ? true : false;
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::TitleOne>(Components::TitleOne(identifier, label));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }

}

#endif // CHERRY_KIT_TITLE_H1
