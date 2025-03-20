#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ImageLocalCentered
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_IMAGE_LOCAL_CENTERED
#define CHERRY_KIT_IMAGE_LOCAL_CENTERED

namespace Cherry
{
    namespace Components
    {
        class ImageLocalCentered : public Component
        {
        public:
            ImageLocalCentered(const Cherry::Identifier &id, const std::string &image_path, float x_size, float y_size)
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
                ImVec2 child_avail = ImGui::GetContentRegionAvail();
                float posX = (child_avail.x - std::stof(GetProperty("size_x"))) * 0.5f;

                if (posX > 0)
                    ImGui::SetCursorPosX(posX);

                ImGui::Image(Cherry::GetTexture(GetProperty("image_path")), ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y"))));
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline bool ImageLocalCentered(const std::string &image_path, float x_size = 50.0f, float y_size = 50.0f)
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
                auto button = Application::CreateAnonymousComponent<Components::ImageLocalCentered>(Components::ImageLocalCentered(anonymous_id, image_path, x_size, y_size));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        inline bool ImageLocalCentered(const Cherry::Identifier &identifier, const std::string &image_path, float x_size = 50.0f, float y_size = 50.0f)
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
                auto new_button = Application::CreateComponent<Components::ImageLocalCentered>(Components::ImageLocalCentered(identifier, image_path, x_size, y_size));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_IMAGE_LOCAL_CENTERED
