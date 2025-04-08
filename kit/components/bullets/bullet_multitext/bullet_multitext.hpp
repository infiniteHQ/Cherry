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
                CherryGUI::BulletText(GetProperty("label").c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> BulletMultiText(const std::vector<std::string> &labels)
        {
            // Inline component
            auto anonymous_id = Application::GenerateUniqueID(labels);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::BulletMultiText>(Components::BulletMultiText(anonymous_id, labels));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> BulletMultiText(const Cherry::Identifier &identifier, const std::vector<std::string> &labels)
        {
            // Get the object if exist
            auto existing_bullet = Application::GetComponent(identifier);
            if (existing_bullet)
            {
                existing_bullet->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_bullet = Application::CreateComponent<Components::BulletMultiText>(Components::BulletMultiText(identifier, labels));
                new_bullet->Render();
                return new_bullet;
            }
            return existing_bullet;
        }
    }

}

#endif // CHERRY_KIT_BULLET_MULTI_TEXT
