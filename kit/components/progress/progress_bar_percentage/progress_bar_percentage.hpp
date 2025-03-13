#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ProgressBarPercentage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_PROGRESS_BAR_PERCENTAGE
#define CHERRY_KIT_PROGRESS_BAR_PERCENTAGE

namespace Cherry
{
    namespace Components
    {
        class ProgressBarPercentage : public Component
        {
        public:
            ProgressBarPercentage(const Cherry::Identifier &id, float *percentage)
                : Component(id), m_Percentage(percentage)
            {
                // Identifier
                SetIdentifier(id);
            }

            void Render() override
            {
                float fraction = (*m_Percentage) / 100.0f;
                ImGui::ProgressBar(fraction, ImVec2(-1.0f, 0.0f));
            }

        private:
            float *m_Percentage;
        };

    }

    // End-User API
    namespace Kit
    {
        inline bool ProgressBarPercentage(float *percentage)
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
                auto button = Application::CreateAnonymousComponent<Components::ProgressBarPercentage>(Components::ProgressBarPercentage(anonymous_id, percentage));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        inline bool ProgressBarPercentage(const Cherry::Identifier &identifier, float *percentage)
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
                auto new_button = Application::CreateComponent<Components::ProgressBarPercentage>(Components::ProgressBarPercentage(identifier, percentage));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_PROGRESS_BAR_PERCENTAGE
