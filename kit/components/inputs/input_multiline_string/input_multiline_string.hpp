#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputMultilineString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_MULTILINE_STRING
#define CHERRY_KIT_INPUT_MULTILINE_STRING

namespace Cherry
{
    namespace Components
    {
        class InputMultilineString : public Component
        {
        public:
            InputMultilineString(const Cherry::Identifier &id, const std::string &label, std::string *value)
                : Component(id), m_Value(value)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#454545B2");

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
                    char buffer[256];
                    std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));

                    // TODO remplace 16 by the number of lines we want to show
                    ImGui::InputTextMultiline(Label.c_str(), buffer, sizeof(buffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
                    *m_Value = std::string(buffer);
                }
                else
                {
                    ImGui::Text("INVALID VALUE");
                }
            }

        private:
            std::string *m_Value;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> InputMultilineString(const std::string &label, std::string *value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::InputMultilineString>(Components::InputMultilineString(Cherry::Identifier(""), label, value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> InputMultilineString(const Cherry::Identifier &identifier, const std::string &label, std::string *value)
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
                auto new_keyval = Application::CreateComponent<Components::InputMultilineString>(Components::InputMultilineString(identifier, label, value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_INPUT_MULTILINE_STRING
