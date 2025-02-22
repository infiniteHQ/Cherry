#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValVector2
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_VECTOR2
#define CHERRY_KIT_KEYVAL_VECTOR2

namespace Cherry
{
    namespace Components
    {
        class KeyValVector2 : public Component
        {
        public:
            KeyValVector2(const Cherry::Identifier &id, const std::string &label, float *x_value, float *y_value)
                : Component(id), m_XValue(x_value), m_YValue(y_value)
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

                        if (m_XValue && m_YValue)
                        {
                            float inputWidth = 100.0f;
                            float colorBoxWidth = 20.0f;
                            float spacing = 5.0f;
                            float elementWidth = colorBoxWidth + spacing + inputWidth;
                            float availableWidth = ImGui::GetContentRegionAvail().x;

                            bool canFitOnSameLine = availableWidth >= 2 * elementWidth;

                            ImGui::PushItemWidth(inputWidth);

                            for (int i = 0; i < 2; ++i)
                            {
                                ImVec2 pos = ImGui::GetCursorScreenPos();
                                const char *texturePath;
                                float *value;
                                std::string id;

                                if (i == 0)
                                {
                                    texturePath = "resources/base/x.png";
                                    value = m_XValue;
                                    id = Label + "###X" + identifier;
                                }
                                else
                                {
                                    texturePath = "resources/base/y.png";
                                    value = m_YValue;
                                    id = Label + "###Y" + identifier;
                                }

                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f);
                                ImGui::Image(Cherry::GetTexture(Cherry::GetPath(texturePath)), ImVec2(23, 23));
                                ImGui::SetCursorScreenPos(ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
                                ImGui::InputFloat(id.c_str(), value);

                                if (i == 0 && canFitOnSameLine)
                                {
                                    ImGui::SameLine();
                                }
                            }

                            ImGui::PopItemWidth();
                        }
                        else
                        {
                            ImGui::Text("INVALID VALUE");
                        }
                    }
                }
            }

        private:
            float *m_XValue;
            float *m_YValue;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> KeyValVector2(const std::string &label, float *x_value, float *y_value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::KeyValVector2>(Components::KeyValVector2(Cherry::Identifier(""), label, x_value, y_value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> KeyValVector2(const Cherry::Identifier &identifier, const std::string &label, float *x_value, float *y_value)
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
                auto new_keyval = Application::CreateComponent<Components::KeyValVector2>(Components::KeyValVector2(identifier, label, x_value, y_value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
            return existing_keyval;
        }
    }

}

#endif // CHERRY_KIT_KEYVAL_VECTOR2
