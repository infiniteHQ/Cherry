#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

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
                SetProperty("color_text", "#FFFFFFFF"); // TODO get the default theme

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                float oldsize = CherryGUI::GetFont()->Scale;
                CherryGUI::GetFont()->Scale *= 1.50; // TODO : property
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
        inline std::shared_ptr<Component> TitleFour(const std::string &label)
        {
            static std::unordered_map<std::size_t, std::weak_ptr<Component>> component_cache;

            std::size_t label_hash = std::hash<std::string>{}(label);

            if (auto existing = component_cache[label_hash].lock())
            {
                existing->Render();
                return existing;
            }

            auto title = Application::CreateAnonymousComponent<Components::TitleFour>(
                Components::TitleFour(Cherry::Identifier(std::to_string(label_hash)), label));

            component_cache[label_hash] = title;
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> TitleFour(const Cherry::Identifier &identifier, const std::string &label)
        {
            if(identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::TitleFour>(Components::TitleFour(identifier, label));
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
                auto new_title = Application::CreateComponent<Components::TitleFour>(Components::TitleFour(identifier, label));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_TITLE_H4
