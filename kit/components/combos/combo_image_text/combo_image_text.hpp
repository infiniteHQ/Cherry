#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ComboImageText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT__IMAGE_TEXT
#define CHERRY_KIT__IMAGE_TEXT

namespace Cherry
{
    namespace Components
    {
        class ComboImageText : public Component
        {
        public:
            ComboImageText(const Cherry::Identifier &id, const std::string &label, const std::vector<std::shared_ptr<Component>> &list, int default_index)
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
                int selected = std::stoi(GetProperty("selected"));
                int default_index = std::stoi(GetProperty("default_index"));
                static ImGuiComboFlags flags = 0;

                if (default_index < 0 || default_index >= m_List.size())
                    default_index = 0;
                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                if (ImGui::BeginCombo(Label.c_str(), [&](){this->m_List[selected]->Render();}))
                {
                    for (int n = 0; n < m_List.size(); n++)
                    {
                        const bool is_selected = (selected == n);

                        if (ImGui::Selectable("", is_selected))
                        {
                            if (selected != n)
                            {
                                selected = n;
                                // UpdateLastChangedTime();
                            }
                        }

                        ImGui::SameLine();
                        m_List[n]->Render();

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }

        private:
            std::vector<std::shared_ptr<Component>> m_List;
        };
    }

    // End-User API
    namespace Kit
    {
        bool ComboImageText(const std::string &label, const std::vector<std::shared_ptr<Component>> &list, int default_index = 0)
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
                auto button = Application::CreateAnonymousComponent<Components::ComboImageText>(Components::ComboImageText(anonymous_id, label, list, default_index));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
        }

        bool ComboImageText(const Cherry::Identifier &identifier, const std::string &label, const std::vector<std::shared_ptr<Component>> &list, int default_index = 0)
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
                auto new_button = Application::CreateComponent<Components::ComboImageText>(Components::ComboImageText(identifier, label, list, default_index));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT__IMAGE_TEXT
