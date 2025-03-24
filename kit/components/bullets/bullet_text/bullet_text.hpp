#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// BulletText
// Summary : A big bullet.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BULLET_TEXT
#define CHERRY_KIT_BULLET_TEXT

namespace Cherry
{
    namespace Components
    {
        class BulletText : public Component
        {
        public:
            BulletText(const Cherry::Identifier &id, const std::string &label)
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
                CherryGUI::BulletText(GetProperty("label").c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> BulletText(const std::string &label)
        {
            // Inline component
            auto bullet = Application::CreateAnonymousComponent<Components::BulletText>(Components::BulletText(Cherry::Identifier(""), label));
            bullet->Render();
            return bullet;
        }

        inline std::shared_ptr<Component> BulletText(const Cherry::Identifier &identifier, const std::string &label)
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
                auto new_bullet = Application::CreateComponent<Components::BulletText>(Components::BulletText(identifier, label));
                new_bullet->Render();
                return new_bullet;
            }
            return existing_bullet;
        }
    }

}

#endif // CHERRY_KIT_BULLET_TEXT
