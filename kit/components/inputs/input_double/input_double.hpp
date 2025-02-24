#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputDouble
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_DOUBLE
#define CHERRY_KIT_INPUT_DOUBLE

namespace Cherry
{
    namespace Components
    {
        class InputDouble : public Component
        {
        public:
            InputDouble(const Cherry::Identifier &id, const std::string &label, double *value)
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
                    ImGui::InputDouble(Label.c_str(), m_Value, std::stof(GetProperty("step")), std::stof(GetProperty("step_fast")));
                }
                else
                {
                    ImGui::Text("INVALID VALUE");
                }
            }

        private:
            double *m_Value;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> InputDouble(const std::string &label, double *value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::InputDouble>(Components::InputDouble(Cherry::Identifier(""), label, value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> InputDouble(const Cherry::Identifier &identifier, const std::string &label, double *value)
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
                auto new_keyval = Application::CreateComponent<Components::InputDouble>(Components::InputDouble(identifier, label, value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_INPUT_DOUBLE
