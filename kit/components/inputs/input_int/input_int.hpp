#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputInteger
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_INTEGER
#define CHERRY_KIT_INPUT_INTEGER

namespace Cherry
{
    namespace Components
    {
        class InputInteger : public Component
        {
        public:
            InputInteger(const Cherry::Identifier &id, const std::string &label, int *value)
                : Component(id), m_Value(value)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("step", "1");
                SetProperty("step_fast", "5");

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += Label + "####" + identifier;
                }

                if (m_Value)
                {
                    ImGui::InputInt(Label.c_str(), m_Value, std::stoi(GetProperty("step")), std::stoi(GetProperty("step_fast")));
                }
                else
                {
                    ImGui::Text("INVALID VALUE");
                }
            }

        private:
            int *m_Value;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> InputInteger(const std::string &label, int *value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::InputInteger>(Components::InputInteger(Cherry::Identifier(""), label, value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> InputInteger(const Cherry::Identifier &identifier, const std::string &label, int *value)
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
                auto new_keyval = Application::CreateComponent<Components::InputInteger>(Components::InputInteger(identifier, label, value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_INPUT_INTEGER
