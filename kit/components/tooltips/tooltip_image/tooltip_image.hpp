#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TooltipImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TOOLTIP_IMAGE
#define CHERRY_KIT_TOOLTIP_IMAGE

namespace Cherry
{
    namespace Components
    {
        class TooltipImage : public Component
        {
        public:
            TooltipImage(const Cherry::Identifier &id, const std::string &image_path, const std::string &description)
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

                // Informations
                SetProperty("image_path", image_path);
                SetProperty("description", description);
            }

            void Render() override
            {
                ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(GetProperty("image_path"));
                ImGui::Image(texture, ImVec2(15, 15));

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(GetProperty("description").c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }
            }
        };
    }

    // End-User API
    namespace Kit
    {
        bool TooltipImage(const std::string &image_path, const std::string &description)
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
                auto button = Application::CreateAnonymousComponent<Components::TooltipImage>(Components::TooltipImage(anonymous_id, image_path, description));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        bool TooltipImage(const Cherry::Identifier &identifier, const std::string &image_path, const std::string &description)
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
                auto new_button = Application::CreateComponent<Components::TooltipImage>(Components::TooltipImage(identifier, image_path, description));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_TOOLTIP_IMAGE
