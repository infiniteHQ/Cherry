#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ComboText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_COMBO_TEXT
#define CHERRY_KIT_COMBO_TEXT

namespace Cherry
{
    namespace Components
    {
        class ComboText : public Component
        {
        public:
            ComboText(const Cherry::Identifier &id, const std::string &label, const std::vector<std::string> &list, int default_index)
                : Component(id), m_List(list)
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

                // Images
                SetProperty("default_index", std::to_string(default_index));
                SetProperty("selected", std::to_string(default_index));

                // Informations
                SetProperty("label", label);

                // Data & User-level informations
                SetData("lastClicked", "never");
                SetData("isClicked", "false");
            }

            void Render() override
            {
                static ImGuiComboFlags flags = 0;

                int selected = std::stoi(GetProperty("selected"));
                int default_index = std::stoi(GetProperty("default_index"));

                if (default_index < 0 || default_index >= m_List.size())
                    default_index = 0;

                const char *combo_preview_value = m_List[selected].c_str();

                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                if (ImGui::BeginCombo(Label.c_str(), combo_preview_value, flags))
                {
                    for (int n = 0; n < m_List.size(); n++)
                    {
                        const bool is_selected = (selected == n);
                        if (ImGui::Selectable(m_List[n].c_str(), is_selected))
                        {
                            if (selected != n)
                            {
                                SetProperty("selected", std::to_string(n));
                                // UpdateLastChangedTime();
                            }
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }

        private:
            std::vector<std::string> m_List;
        };
    }

    // End-User API
    namespace Kit
    {
        bool ComboText(const std::string &label, const std::vector<std::string> &list, int default_index = 0)
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
                auto button = Application::CreateAnonymousComponent<Components::ComboText>(Components::ComboText(anonymous_id, label, list, default_index));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
        }

        bool ComboText(const Cherry::Identifier &identifier, const std::string &label, const std::vector<std::string> &list, int default_index = 0)
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
                auto new_button = Application::CreateComponent<Components::ComboText>(Components::ComboText(identifier, label, list, default_index));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_COMBO_TEXT
