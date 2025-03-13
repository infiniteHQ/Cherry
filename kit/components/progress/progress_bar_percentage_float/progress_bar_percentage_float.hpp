#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ProgressBarPercentageFloat
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_FLOAT
#define CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_FLOAT

namespace Cherry
{
    namespace Components
    {
        class ProgressBarPercentageFloat : public Component
        {
        public:
            ProgressBarPercentageFloat(const Cherry::Identifier &id, float* number, float*goal)
                : Component(id), m_Number(number), m_Goal(goal)
            {
                // Identifier
                SetIdentifier(id);
            }

            void Render() override
            {            
                float fraction = (*m_Number) / (*m_Goal);
                ImGui::ProgressBar(fraction, ImVec2(-1.0f, 0.0f));
            }

        private:
            float *m_Number;
            float *m_Goal;
        };

    }

    // End-User API
    namespace Kit
    {
        inline bool ProgressBarPercentageFloat(float* number, float*goal)
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
                auto button = Application::CreateAnonymousComponent<Components::ProgressBarPercentageFloat>(Components::ProgressBarPercentageFloat(anonymous_id, number, goal));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        inline bool ProgressBarPercentageFloat(const Cherry::Identifier &identifier, float* number, float*goal)
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
                auto new_button = Application::CreateComponent<Components::ProgressBarPercentageFloat>(Components::ProgressBarPercentageFloat(identifier, number, goal));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_FLOAT
