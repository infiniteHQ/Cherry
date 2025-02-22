#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_IMAGE
#define CHERRY_KIT_TEXT_IMAGE

namespace Cherry
{
    namespace Components
    {
        class TextImage : public Component
        {
        public:
            TextImage(const Cherry::Identifier &id, const std::string &label, const std::string &image_path)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#454545B2");

                // Informations
                SetProperty("label", label);
                SetProperty("image_path", image_path);
            }

            void Render() override
            {
                std::string text = GetProperty("label").c_str();

                ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(GetProperty("image_path"));
                ImGui::Image(texture, ImVec2(15, 15));
                ImGui::SameLine();
                ImGui::Text(text.c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> TextImage(const std::string &label, const std::string &image_path)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::TextImage>(Components::TextImage(Cherry::Identifier(""), label, image_path));
            title->Render();
            return title;
        }

        std::shared_ptr<Component> TextImage(const Cherry::Identifier &identifier, const std::string &label, const std::string &image_path)
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
                auto new_title = Application::CreateComponent<Components::TextImage>(Components::TextImage(identifier, label, image_path));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_TEXT_IMAGE
