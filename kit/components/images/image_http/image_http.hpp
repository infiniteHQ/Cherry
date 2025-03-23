#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ImageHttp
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_IMAGE_HTTP
#define CHERRY_KIT_IMAGE_HTTP

namespace Cherry
{
    namespace Components
    {
#ifdef CHERRY_ENABLE_NET
        class ImageHttp : public Component
        {
        public:
            ImageHttp(const Cherry::Identifier &id, const std::string &image_http_path)
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
                SetProperty("image_http_path", image_http_path);

                // Data & User-level informations
                SetData("isChecked", "false");
            }

            void Render() override
            {
                ImGui::Image(Cherry::GetTexture(Cherry::GetHttpPath(GetProperty("image_http_path"))), ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y"))));
            }
        };
#else  // CHERRY_ENABLE_NET
        class ImageHttp : public Component
        {
        public:
            ImageHttp(const Cherry::Identifier &id, const std::string &image_http_path)
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
                SetProperty("image_http_path", image_http_path);

                // Data & User-level informations
                SetData("isChecked", "false");
            }

            void Render() override
            {
                ImGui::TextColored(Cherry::HexToRGBA("#FF3333FF"),"ERROR: Cannot display http(s) image without define CHERRY_ENABLE_NET");
            }
        };
#endif // CHERRY_ENABLE_NET

    }

    // End-User API
    namespace Kit
    {
        inline bool ImageHttp(const std::string &image_http_path)
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
                auto button = Application::CreateAnonymousComponent<Components::ImageHttp>(Components::ImageHttp(anonymous_id, image_http_path));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        inline bool ImageHttp(const Cherry::Identifier &identifier, const std::string &image_http_path)
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
                auto new_button = Application::CreateComponent<Components::ImageHttp>(Components::ImageHttp(identifier, image_http_path));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_IMAGE_HTTP
