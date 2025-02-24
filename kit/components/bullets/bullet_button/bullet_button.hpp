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
                ImGui::Bullet();
                ImGui::Button(GetProperty("label").c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> BulletButton(const std::string &label)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::BulletButton>(Components::BulletButton(Cherry::Identifier(""), label));
            title->Render();
            return title;
        }

        std::shared_ptr<Component> BulletButton(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_title = Application::CreateComponent<Components::BulletButton>(Components::BulletButton(identifier, label));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_BULLET_BUTTON
