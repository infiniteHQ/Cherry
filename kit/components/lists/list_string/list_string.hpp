#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ListString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_LIST_STRING
#define CHERRY_KIT_LIST_STRING

namespace Cherry
{
    namespace Components
    {
        class ListString : public Component
        {
        public:
            ListString(const Cherry::Identifier &id, const std::vector<std::string>& values)
                : Component(id), m_Values(values)
            {
                // Identifier
                SetIdentifier(id);
            }

            void Render() override
            {
                std::string identifier = GetIdentifier().string();
                std::string Label;

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                for(auto value : m_Values)
                {
                    ImGui::ListBox()
                }

                if (m_Value)
                {
                    char buffer[256];
                    std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));

                    ImGui::InputText(Label.c_str(), buffer, sizeof(buffer));
                    *m_Value = std::string(buffer);
                }
                else
                {
                    ImGui::Text("INVALID VALUE");
                }
            }

        private:
            std::vector<std::string> m_Values;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> ListString(const std::string &label, const std::vector<std::string>& values)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::ListString>(Components::ListString(Cherry::Identifier(""), values));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> ListString(const Cherry::Identifier &identifier, const std::vector<std::string>& values)
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
                auto new_keyval = Application::CreateComponent<Components::ListString>(Components::ListString(identifier, values));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_LIST_STRING
