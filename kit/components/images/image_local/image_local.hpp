#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ImageLocal
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_IMAGE_LOCAL
#define CHERRY_KIT_IMAGE_LOCAL

namespace Cherry
{
    namespace Components
    {
        class ImageLocal : public Component
        {
        public:
            ImageLocal(const Cherry::Identifier &id, const std::string &image_path)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_border", "#454545B2");
                SetProperty("color_border_hovered", "#454545B2");
                SetProperty("color_border_clicked", "#454545B2");
                SetProperty("color_bg", "#242424FF");
                SetProperty("color_bg_hovered", "#343434FF");
                SetProperty("color_bg_clicked", "#444444FF");

                // Sizes
                SetProperty("size_x", "50.0");
                SetProperty("size_y", "50.0");

                // Informations
                SetProperty("image_path", image_path);

                // Data & User-level informations
                SetData("isChecked", "false");
            }

            void Render() override
            {
                ImGui::Image(Cherry::GetTexture(GetProperty("image_path")), ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y"))));
            }
        };
    }

    // End-User API
    namespace Kit
    {
        bool ImageLocal(const std::string &image_path)
        {
            // Inline component
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing->GetData("isClicked") == "true" ? true : false;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::ImageLocal>(Components::ImageLocal(anonymous_id, image_path));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        bool ImageLocal(const Cherry::Identifier &identifier, const std::string &image_path)
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
                auto new_button = Application::CreateComponent<Components::ImageLocal>(Components::ImageLocal(identifier, image_path));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_IMAGE_LOCAL
