#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// SeparatorText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_SEPARATOR_TEXT
#define CHERRY_KIT_SEPARATOR_TEXT

namespace Cherry
{
    namespace Components
    {
        class SeparatorText : public Component
        {
        public:
            SeparatorText(const Cherry::Identifier &id, const std::string &label)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#454545B2");

                // Informations
                SetProperty("label", label);
                SetProperty("text_scale", "0.8");
            }

            void Render() override
            {
                std::string label = GetProperty("label");
                float text_scale = std::stof(GetProperty("text_scale"));
                const float &ctx_text_scale = Application::GetCurrentRenderedWindow()->m_Specifications.FontGlobalScale;

                ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
                ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
                CherryGUI::GetFont()->Scale *= text_scale;
                CherryGUI::PushFont(CherryGUI::GetFont());

                CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + 5.0f);

                CherryGUI::PushStyleColor(ImGuiCol_Text, grayColor);
                CherryGUI::Text(label.c_str());
                CherryGUI::PopStyleColor();

                CherryGUI::PushStyleColor(ImGuiCol_Separator, graySeparatorColor);
                CherryGUI::Separator();
                CherryGUI::PopStyleColor();

                CherryGUI::GetFont()->Scale = ctx_text_scale;
                CherryGUI::PopFont();
                CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + 2.0f);
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> SeparatorText(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::SeparatorText>(Components::SeparatorText(Cherry::Identifier(""), label));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> SeparatorText(const Cherry::Identifier &identifier, const std::string &label)
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::SeparatorText>(Components::SeparatorText(identifier, label));
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
                auto new_title = Application::CreateComponent<Components::SeparatorText>(Components::SeparatorText(identifier, label));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_SEPARATOR_TEXT
