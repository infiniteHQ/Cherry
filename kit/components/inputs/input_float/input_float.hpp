#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputFloat
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_FLOAT
#define CHERRY_KIT_INPUT_FLOAT

namespace Cherry
{
    namespace Components
    {
        class InputFloat : public Component
        {
        public:
            InputFloat(const Cherry::Identifier &id, const std::string &label, float *value)
                : Component(id), m_Value(value)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("step", "1.0");
                SetProperty("step_fast", "5.0");

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                if (m_Value)
                {
                    CherryGUI::InputFloat(Label.c_str(), m_Value, std::stof(GetProperty("step")), std::stof(GetProperty("step_fast")));
                }
                else
                {
                    CherryGUI::Text("INVALID VALUE");
                }
            }

        private:
            float *m_Value;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> InputFloat(const std::string &label, float *value)
        {
            // Inline component
            auto anonymous_id = Application::GenerateUniqueID(label, value);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {

                auto button = Application::CreateAnonymousComponent<Components::InputFloat>(Components::InputFloat(anonymous_id, label, value));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> InputFloat(const Cherry::Identifier &identifier, const std::string &label, float *value)
        {
            // Get the object if exist
            auto existing_keyval = Application::GetComponent(identifier);
            if (existing_keyval)
            {
                existing_keyval->RefreshContextProperties();
                existing_keyval->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_keyval = Application::CreateComponent<Components::InputFloat>(Components::InputFloat(identifier, label, value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_INPUT_FLOAT
