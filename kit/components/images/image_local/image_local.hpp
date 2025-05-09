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
            ImageLocal(const Cherry::Identifier &id, const std::string &image_path, float x_size, float y_size)
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
                SetProperty("size_x", std::to_string(x_size));
                SetProperty("size_y", std::to_string(y_size));

                // Informations
                SetProperty("image_path", image_path);

                // Data & User-level informations
                SetData("isChecked", "false");
            }

            void Render() override
            {
                CherryGUI::Image(Cherry::GetTexture(GetProperty("image_path")), ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y"))));
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> ImageLocal(const std::string &image_path, float x_size = 50.0f, float y_size = 50.0f)
        {
            auto anonymous_id = Application::GenerateUniqueID(image_path, x_size, y_size);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }

            auto button = Application::CreateAnonymousComponent<Components::ImageLocal>(Components::ImageLocal(anonymous_id, image_path, x_size, y_size));
            button->Render();
            return button;
        }

        inline bool ImageLocal(const Cherry::Identifier &identifier, const std::string &image_path, float x_size = 50.0f, float y_size = 50.0f)
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
                auto new_button = Application::CreateComponent<Components::ImageLocal>(Components::ImageLocal(identifier, image_path, x_size, y_size));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_IMAGE_LOCAL
