#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// BulletMultiText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BULLET_MULTI_TEXT
#define CHERRY_KIT_BULLET_MULTI_TEXT

namespace Cherry
{
    namespace Components
    {
        class BulletMultiText : public Component
        {
        public:
            BulletMultiText(const Cherry::Identifier &id, const std::vector<std::string> &labels)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#454545B2");

                // Informations
                std::string label;
                bool first_label = true;
                for(auto string_label : labels)
                {
                    std::string extension = first_label == false ? "\n" : "";
                    label += string_label + extension;
                    first_label = false;
                }

                SetProperty("label", label);
            }

            void Render() override
            {
                ImGui::BulletText(GetProperty("label").c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> BulletMultiText(const std::vector<std::string> &labels)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::BulletMultiText>(Components::BulletMultiText(Cherry::Identifier(""), labels));
            title->Render();
            return title;
        }

        std::shared_ptr<Component> BulletMultiText(const Cherry::Identifier &identifier, const std::vector<std::string> &labels)
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
                auto new_title = Application::CreateComponent<Components::BulletMultiText>(Components::BulletMultiText(identifier, labels));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_BULLET_MULTI_TEXT
