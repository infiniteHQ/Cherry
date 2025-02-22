#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValVector3_5
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_VECTOR3_5
#define CHERRY_KIT_KEYVAL_VECTOR3_5

namespace Cherry
{
    namespace Components
    {
        class KeyValVector3_5 : public Component
        {
        public:
            KeyValVector3_5(const Cherry::Identifier &id, const std::string &label, float *x_value, float *y_value, float *z_value, float *xw_value, float* yw_value, float* zw_value)
                : Component(id), m_XValue(x_value), m_YValue(y_value), m_ZValue(z_value), m_XWValue(xw_value), m_YWValue(yw_value), m_ZWValue(zw_value)
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

            if (m_XValue && m_YValue && m_ZValue && m_XWValue && m_YWValue && m_ZWValue)
            {
                float inputWidth = 100.0f;
                float colorBoxWidth = 20.0f;
                float spacing = 5.0f;
                float elementWidth = colorBoxWidth + spacing + inputWidth; 
                float availableWidth = ImGui::GetContentRegionAvail().x;

                int elementsPerRow = (availableWidth >= 4 * elementWidth) ? 4 : (availableWidth >= 3 * elementWidth) ? 3
                                                                                        : (availableWidth >= 2 * elementWidth)   ? 2
                                                                                                                                 : 1;

                ImGui::PushItemWidth(inputWidth);

                for (int i = 0; i < 3; ++i)
                {
                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    const char *texturePath;
                    const char *wtexturePath;
                    float *value, *valueW;
                    std::string id, subId;

                    switch (i)
                    {
                    case 0:
                        texturePath = "resources/base/x.png";
                        wtexturePath = "resources/base/wx.png";
                        value = m_XValue;
                        valueW = m_XWValue;
                        id = Label + "###1";
                        subId = Label + "###1sub";
                        break;
                    case 1:
                        texturePath = "resources/base/y.png";
                        wtexturePath = "resources/base/wy.png";
                        value = m_YValue;
                        valueW = m_YWValue;
                        id = Label + "###2";
                        subId = Label + "###2sub";
                        break;
                    case 2:
                        texturePath = "resources/base/z.png";
                        wtexturePath = "resources/base/wz.png";
                        value = m_ZValue;
                        valueW = m_ZWValue;
                        id = Label + "###3";
                        subId = Label + "###3sub";
                        break;
                    }

                    ImGui::BeginGroup();
                                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f);
                    ImGui::Image(Cherry::GetTexture(Cherry::GetPath(texturePath)), ImVec2(23, 23));
                    ImGui::SetCursorScreenPos(ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
                    ImGui::InputFloat(id.c_str(), value);

                    ImGui::SetCursorScreenPos(ImVec2(pos.x + colorBoxWidth + spacing, pos.y + ImGui::GetTextLineHeightWithSpacing()));
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 25.0f);
                    ImGui::Image(Cherry::GetTexture(Cherry::GetPath(wtexturePath)), ImVec2(23, 18));
                    ImGui::SameLine();
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12,3));
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 2.5f);
                    ImGui::InputFloat(subId.c_str(), valueW, 0.0f, 0.0f, "%.3f");
                    ImGui::PopStyleVar();
                    ImGui::EndGroup();

                    if ((i + 1) % elementsPerRow != 0 && i < 2)
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
            float *m_ZValue;
            float *m_XWValue;
            float *m_YWValue;
            float *m_ZWValue;
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<Component> KeyValVector3_5(const std::string &label, float *x_value, float *y_value, float *z_value, float *xw_value, float* yw_value, float* zw_value)
        {
            // Inline component
            auto keyval = Application::CreateAnonymousComponent<Components::KeyValVector3_5>(Components::KeyValVector3_5(Cherry::Identifier(""), label, x_value, y_value, z_value, xw_value, yw_value, zw_value));
            keyval->RefreshContextProperties();
            keyval->Render();
            return keyval;
        }

        std::shared_ptr<Component> KeyValVector3_5(const Cherry::Identifier &identifier, const std::string &label, float *x_value, float *y_value, float *z_value, float *xw_value, float* yw_value, float* zw_value)
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
                auto new_keyval = Application::CreateComponent<Components::KeyValVector3_5>(Components::KeyValVector3_5(identifier, label, x_value, y_value, z_value, xw_value, yw_value, zw_value));
                new_keyval->RefreshContextProperties();
                new_keyval->Render();
                return new_keyval;
            }
        }
    }

}

#endif // CHERRY_KIT_KEYVAL_VECTOR3_5
