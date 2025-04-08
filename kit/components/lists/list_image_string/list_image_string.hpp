#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ListImageString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_LIST_IMAGE_STRING
#define CHERRY_KIT_LIST_IMAGE_STRING

namespace Cherry
{
    namespace Components
    {
        class ListImageString : public Component
        {
        public:
            ListImageString(const Cherry::Identifier &id, const std::string &label, const std::vector<std::pair<std::string, std::string>> &values, int default_selected)
                : Component(id), m_Values(values)
            {
                // Identifier
                SetIdentifier(id);

                SetProperty("label", label);
                SetProperty("default_selected", std::to_string(default_selected));
                SetProperty("current_selected", std::to_string(default_selected));
            }

            void Render() override
            {
                std::string identifier = GetIdentifier().string();
                std::string Label;

                if (!identifier.empty())
                {
                    Label += Label + "####" + identifier;
                }

                int current_selected = std::stoi(GetProperty("current_selected"));

                if (CherryGUI::BeginListBox(Label.c_str()))
                {
                    for (int n = 0; n < m_Values.size(); n++)
                    {
                        const bool is_selected = (current_selected == n);
                        if (CherryGUI::Selectable(std::string("####" + identifier + std::to_string(n)).c_str(), is_selected))
                        {
                            current_selected = n;
                            SetProperty("current_selected", std::to_string(current_selected));
                        }

                        CherryGUI::SameLine();

                        ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_Values[n].second);
                        CherryGUI::Image(texture, ImVec2(15, 15));
                        CherryGUI::SameLine();
                        CherryGUI::Text(m_Values[n].first.c_str());

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            CherryGUI::SetItemDefaultFocus();
                    }
                    CherryGUI::EndListBox();
                }
            }

        private:
            std::vector<std::pair<std::string, std::string>> m_Values;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> ListImageString(const std::string &label, const std::vector<std::pair<std::string, std::string>> &values, int default_selected = 0)
        {
            // Inline component
            auto anonymous_id = Application::GenerateUniqueID(label, values, default_selected);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {

                auto button = Application::CreateAnonymousComponent<Components::ListImageString>(Components::ListImageString(anonymous_id, label, values, default_selected));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> ListImageString(const Cherry::Identifier &identifier, const std::string &label, const std::vector<std::pair<std::string, std::string>> &values, int default_selected = 0)
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
                auto new_keyval = Application::CreateComponent<Components::ListImageString>(Components::ListImageString(identifier, label, values, default_selected));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_LIST_IMAGE_STRING
