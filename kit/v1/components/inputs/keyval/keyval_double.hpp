#include "../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_INPUTS_KEYVAL_DOUBLE
#define UIKIT_V1_INPUTS_KEYVAL_DOUBLE

namespace UIKit
{
    class DoubleKeyValString : public Component
    {
    public:
        DoubleKeyValString(
            const std::string &id,
            const std::shared_ptr<std::string> &value,
            const std::string &label = "String",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
              m_Label(label),
              m_Value(value)
        {
            m_ID = id;
        }

        void Render(int column)
        {
            if (column == 0)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
                ImGui::AlignTextToFramePadding();
                ImGui::Indent(10.0f);
                ImGui::TextWrapped(m_Label.c_str());
                ImGui::Unindent(10.0f);
            }
            else if (column == 1)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
                std::string label = "###Rest" + m_Label;

                if (m_Value)
                {
                    ImGui::SetNextItemWidth(-FLT_MIN); 

                    char buffer[256];                                      
                    std::strncpy(buffer, m_Value->c_str(), sizeof(buffer)); 

                    ImGui::InputText(label.c_str(), buffer, sizeof(buffer));
                    *m_Value = std::string(buffer);
                }
                else
                {
                    ImGui::Text("INVALID VALUE");
                }
            }
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "value")
            {
                return *m_Value.get();
            }

            return "none";
        }

    private:
        std::shared_ptr<std::string> m_Value;
        std::string m_Label;
    };

    class DoubleKeyValInteger : public Component
    {
    public:
        DoubleKeyValInteger(
            const std::string &id,
            const std::shared_ptr<int> &value,
            const std::string &label = "String",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
              m_Label(label),
              m_Value(value)
        {
            m_ID = id;
        }

        void Render(int column, int step = 1, int fast_step = 100)
        {
            if (column == 0)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
                ImGui::AlignTextToFramePadding();
                ImGui::Indent(10.0f);
                ImGui::TextWrapped(m_Label.c_str());
                ImGui::Unindent(10.0f);
            }
            else if (column == 1)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
                std::string label = "###Rest" + m_Label;

                if (m_Value)
                {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::InputInt(label.c_str(), m_Value.get(), step, fast_step);
                }
                else
                {
                    ImGui::Text("INVALID VALUE");
                }
            }
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "value")
            {
                return std::to_string(*m_Value.get());
            }

            return "none";
        }

    private:
        std::shared_ptr<int> m_Value;
        std::string m_Label;
    };

}

#endif // UIKIT_V1_INPUTS_KEYVAL_DOUBLE
