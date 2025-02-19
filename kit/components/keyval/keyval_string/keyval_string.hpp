#pragma once
#include "../../../../platform/engine/app.hpp"

//
// KeyValString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_STRING
#define CHERRY_KIT_KEYVAL_STRING

namespace Cherry
{
    namespace Components
    {
        class KeyValString : public Component
        {
        public:
            KeyValString(const Cherry::Identifier &id, const std::string &label, std::string *value)
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

                        if (m_Value)
                        {
                            ImGui::SetNextItemWidth(-FLT_MIN);

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
                }
            }

        private:
            std::string *m_Value;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> KeyValString(const std::string &label, std::string *value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::KeyValString>(Components::KeyValString(Cherry::Identifier(""), label, value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> KeyValString(const Cherry::Identifier &identifier, const std::string &label, std::string *value)
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
                auto new_keyval = Application::CreateComponent<Components::KeyValString>(Components::KeyValString(identifier, label, value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_KEYVAL_STRING
