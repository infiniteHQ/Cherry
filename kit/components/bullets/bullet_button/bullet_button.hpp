#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// BulletButton
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BULLET_BUTTON
#define CHERRY_KIT_BULLET_BUTTON

namespace Cherry
{
    namespace Components
    {
        class BulletButton : public Component
        {
        public:
            BulletButton(const Cherry::Identifier &id, const std::string &label)
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
                CherryGUI::Bullet();
                CherryGUI::Button(GetProperty("label").c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> BulletButton(const std::string &label)
        {
            // Inline component
            auto anonymous_id = Application::GenerateUniqueID(label);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::BulletButton>(Components::BulletButton(anonymous_id, label));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> BulletButton(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_bullet = Application::CreateComponent<Components::BulletButton>(Components::BulletButton(identifier, label));
                new_bullet->Render();
                return new_bullet;
            }
            return existing_bullet;
        }
    }

}

#endif // CHERRY_KIT_BULLET_BUTTON
