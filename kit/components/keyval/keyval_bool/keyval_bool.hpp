#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValBool
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_BOOL
#define CHERRY_KIT_KEYVAL_BOOL

namespace Cherry
{
    namespace Components
    {
        class KeyValBool : public Component
        {
        public:
            KeyValBool(const Cherry::Identifier &id, const std::string &label, bool* value)
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
                        CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
                        CherryGUI::AlignTextToFramePadding();
                        CherryGUI::Indent(10.0f);
                        CherryGUI::TextWrapped(GetProperty("label").c_str());
                        CherryGUI::Unindent(10.0f);
                    }
                    else if (std::stoi(parent->GetData("renderedColumn")) == 1)
                    {
                        std::string identifier = GetIdentifier().string();
                        std::string Label = "####" + GetProperty("label");

                        if (!identifier.empty())
                        {
                            Label += Label + identifier;
                        }

                        CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));

                        if (m_Value)
                        {
                            CherryGUI::SetNextItemWidth(-FLT_MIN);
                            CherryGUI::Checkbox(Label.c_str(), m_Value);
                        }
                        else
                        {
                            CherryGUI::Text("INVALID VALUE");
                        }
                    }
                }
            }

        private:
            bool *m_Value;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> KeyValBool(const std::string &label, bool* value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::KeyValBool>(Components::KeyValBool(Cherry::Identifier(""), label, value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        inline std::shared_ptr<Component> KeyValBool(const Cherry::Identifier &identifier, const std::string &label, bool* value)
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
                auto new_keyval = Application::CreateComponent<Components::KeyValBool>(Components::KeyValBool(identifier, label, value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_KEYVAL_BOOL
