#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValComboString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_COMBO_SIMPLE
#define CHERRY_KIT_KEYVAL_COMBO_SIMPLE

namespace Cherry
{
    namespace Components
    {
        class KeyValComboString : public Component
        {
        public:
            KeyValComboString(const Cherry::Identifier &id, const std::string &label, const std::vector<std::string> &list, int default_selected_index = 0)
                : Component(id), m_List(list)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#454545B2");

                // Behavior
                SetProperty("default_index", std::to_string(default_selected_index));
                SetProperty("selected_index", std::to_string(default_selected_index));

                // Informations
                SetProperty("label", label);
            }

            void Render() override
            {
                auto parent = Cherry::GetParent();
                if (parent)
                {
                    if (std::stoi(parent->GetData("renderedColumn")) == 0)
                    {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
                        ImGui::AlignTextToFramePadding();
                        ImGui::Indent(10.0f);
                        ImGui::TextWrapped(GetProperty("label").c_str());
                        ImGui::Unindent(10.0f);
                    }
                    else if (std::stoi(parent->GetData("renderedColumn")) == 1)
                    {
                        std::string identifier = GetIdentifier().string();
                        std::string Label = "####" + GetProperty("label");

                        if (!identifier.empty())
                        {
                            Label += Label + identifier;
                        }

                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));

                        if (!m_List.empty())
                        {
                            static ImGuiComboFlags flags = 0;

                            int default_index = std::stoi(GetProperty("default_index"));
                            int selected = std::stoi(GetProperty("selected_index"));

                            if (default_index < 0 || default_index >= m_List.size())
                                default_index = 0;

                            std::string identifier = GetIdentifier().string();
                            std::string Label = "####" + GetProperty("label");

                            if (!identifier.empty())
                            {
                                Label += Label + identifier;
                            }

                            const char *combo_preview_value = m_List[selected].c_str();
                            if (ImGui::BeginCombo(Label.c_str(), combo_preview_value, flags))
                            {
                                for (int n = 0; n < m_List.size(); n++)
                                {
                                    const bool is_selected = (selected == n);
                                    if (ImGui::Selectable(m_List[n].c_str(), is_selected))
                                    {
                                        if (selected != n)
                                        {
                                            selected = n;
                                            SetProperty("selected_index", std::to_string(n));
                                            // UpdateLastChangedTime();
                                        }
                                    }

                                    if (is_selected)
                                        ImGui::SetItemDefaultFocus();
                                }
                                ImGui::EndCombo();
                            }
                        }
                        else
                        {
                            ImGui::Text("INVALID/EMPTY VALUE");
                        }
                    }
                }
            }

        private:
            std::vector<std::string> m_List;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> KeyValComboString(const std::string &label, const std::vector<std::string> &list, int default_selected_index = 0)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::KeyValComboString>(Components::KeyValComboString(Cherry::Identifier(""), label, list, default_selected_index));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> KeyValComboString(const Cherry::Identifier &identifier, const std::string &label, const std::vector<std::string> &list, int default_selected_index = 0)
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
                auto new_keyval = Application::CreateComponent<Components::KeyValComboString>(Components::KeyValComboString(identifier, label, list, default_selected_index));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return existing_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_KEYVAL_COMBO_SIMPLE
