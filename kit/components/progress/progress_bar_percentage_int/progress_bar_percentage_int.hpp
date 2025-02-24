#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ProgressBarPercentageInteger
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_INTEGER
#define CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_INTEGER

namespace Cherry
{
    namespace Components
    {
        class ProgressBarPercentageInteger : public Component
        {
        public:
            ProgressBarPercentageInteger(const Cherry::Identifier &id, int* number, int*goal)
                : Component(id), m_Number(number), m_Goal(goal)
            {
                // Identifier
                SetIdentifier(id);
            }

            void Render() override
            {            
                float fraction = static_cast<float>(*m_Number) / static_cast<float>(*m_Goal);
                ImGui::ProgressBar(fraction, ImVec2(-1.0f, 0.0f));
            }

        private:
            int *m_Number;
            int *m_Goal;
        };

    }

    // End-User API
    namespace Kit
    {
        bool ProgressBarPercentageInteger(int* number, int*goal)
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
                auto button = Application::CreateAnonymousComponent<Components::ProgressBarPercentageInteger>(Components::ProgressBarPercentageInteger(anonymous_id, number, goal));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        bool ProgressBarPercentageInteger(const Cherry::Identifier &identifier, int* number, int*goal)
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
                auto new_button = Application::CreateComponent<Components::ProgressBarPercentageInteger>(Components::ProgressBarPercentageInteger(identifier, number, goal));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_INTEGER
