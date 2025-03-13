#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValFloat
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_FLOAT
#define CHERRY_KIT_KEYVAL_FLOAT

namespace Cherry
{
    namespace Components
    {
        class KeyValFloat : public Component
        {
        public:
            KeyValFloat(const Cherry::Identifier &id, const std::string &label, float *value)
                : Component(id), m_Value(value)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#454545B2");

                // Behavior
                SetProperty("step", "1");
                SetProperty("step_fast", "10");

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
                            ImGui::InputFloat(Label.c_str(), m_Value);
                        }
                        else
                        {
                            ImGui::Text("INVALID VALUE");
                        }
                    }
                }
            }

        private:
            float *m_Value;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> KeyValFloat(const std::string &label, float *value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::KeyValFloat>(Components::KeyValFloat(Cherry::Identifier(""), label, value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        inline std::shared_ptr<Component> KeyValFloat(const Cherry::Identifier &identifier, const std::string &label, float *value)
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
                auto new_keyval = Application::CreateComponent<Components::KeyValFloat>(Components::KeyValFloat(identifier, label, value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return existing_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_KEYVAL_FLOAT
