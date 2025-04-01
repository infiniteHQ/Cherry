#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

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
                SetProperty("color_text", "#FFFFFFFF"); // TODO get the default theme
                SetProperty("font_scale", "1.1");       // TODO get the default theme

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                if (NeedRefreshing())
                {
                    cached_text_color = HexToRGBA(GetProperty("color_text"));
                    cached_label = GetProperty("label");
                    font_scale = std::stof(GetProperty("font_scale"));

                    ///Refreshed();
                }

                float old_font_size = CherryGUI::GetFont()->Scale;
                CherryGUI::GetFont()->Scale *= font_scale;
                CherryGUI::PushFont(CherryGUI::GetFont());

                CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
                CherryGUI::TextColored(cached_text_color, cached_label.c_str());
                CherryGUI::PopStyleVar();

                CherryGUI::GetFont()->Scale = old_font_size;
                CherryGUI::PopFont();
            }

        private:
            ImVec4 cached_text_color;
            std::string cached_label;
            float font_scale;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> TitleSix(const std::string &label)
        {
            auto anonymous_id = Application::GenerateUniqueID(label);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }

            auto button = Application::CreateAnonymousComponent<Components::TitleSix>(Components::TitleSix(anonymous_id, label));
                button->Render();
            return button;
        }

        inline std::shared_ptr<Component> TitleSix(const Cherry::Identifier &identifier, const std::string &label)
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::TitleSix>(Components::TitleSix(identifier, label));
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
                auto new_title = Application::CreateComponent<Components::TitleSix>(Components::TitleSix(identifier, label));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_TITLE_H6
