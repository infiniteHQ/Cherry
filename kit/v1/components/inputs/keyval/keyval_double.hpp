#include "../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_INPUTS_KEYVAL_DOUBLE
#define UIKIT_V1_INPUTS_KEYVAL_DOUBLE

namespace Cherry
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

    class DoubleKeyValBoolean : public Component
    {
    public:
        DoubleKeyValBoolean(
            const std::string &id,
            const std::shared_ptr<bool> &value,
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

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));

                if (m_Value)
                {
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::Checkbox(label.c_str(), m_Value.get());
                }
                else
                {
                    ImGui::Text("INVALID VALUE");
                }

                ImGui::PopStyleVar(2);
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
        std::shared_ptr<bool> m_Value;
        std::string m_Label;
    };

    class DoubleKeyValFloat : public Component
    {
    public:
        DoubleKeyValFloat(
            const std::string &id,
            const std::shared_ptr<float> &value,
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
                    ImGui::InputFloat(label.c_str(), m_Value.get());
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
        std::shared_ptr<float> m_Value;
        std::string m_Label;
    };

    class DoubleKeyValDouble : public Component
    {
    public:
        DoubleKeyValDouble(
            const std::string &id,
            const std::shared_ptr<double> &value,
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
                    ImGui::InputDouble(label.c_str(), m_Value.get());
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
        std::shared_ptr<double> m_Value;
        std::string m_Label;
    };

    class DoubleKeyValXYZVector : public Component
    {
    public:
        DoubleKeyValXYZVector(
            const std::string &id,
            const std::shared_ptr<float> &x,
            const std::shared_ptr<float> &y,
            const std::shared_ptr<float> &z,
            const std::string &label = "String",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
              m_Label(label),
              m_XValue(x),
              m_YValue(y),
              m_ZValue(z)
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
                if (m_XValue && m_YValue && m_ZValue)
                {
                    float inputWidth = 100.0f;
                    float colorBoxWidth = 20.0f;
                    float spacing = 5.0f;
                    ImGui::PushItemWidth(inputWidth);

                    {
                        ImVec2 pos = ImGui::GetCursorScreenPos();
                        ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

                        ImDrawList *draw_list = ImGui::GetWindowDrawList();
                        draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + colorBoxWidth, pos.y + ImGui::GetTextLineHeight()), ImColor(color));

                        ImGui::SetCursorScreenPos(ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
                        std::string id = label + "###1";
                        ImGui::InputFloat(id.c_str(), m_XValue.get());
                    }
                    ImGui::SameLine();

                    {
                        ImVec2 pos = ImGui::GetCursorScreenPos();
                        ImVec4 color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
                        ImDrawList *draw_list = ImGui::GetWindowDrawList();
                        draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + colorBoxWidth, pos.y + ImGui::GetTextLineHeight()), ImColor(color));

                        ImGui::SetCursorScreenPos(ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
                        std::string id = label + "###2";
                        ImGui::InputFloat(id.c_str(), m_YValue.get());
                    }
                    ImGui::SameLine();

                    {
                        ImVec2 pos = ImGui::GetCursorScreenPos();
                        ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

                        ImDrawList *draw_list = ImGui::GetWindowDrawList();
                        draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + colorBoxWidth, pos.y + ImGui::GetTextLineHeight()), ImColor(color));

                        ImGui::SetCursorScreenPos(ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
                        std::string id = label + "###3";
                        ImGui::InputFloat(id.c_str(), m_ZValue.get());
                    }

                    ImGui::PopItemWidth();
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
                std::string formatted_output = "";
                return formatted_output;
            }

            return "none";
        }

    private:
        std::shared_ptr<float> m_XValue;
        std::shared_ptr<float> m_YValue;
        std::shared_ptr<float> m_ZValue;
        std::string m_Label;
    };

    class DoubleKeyValSimpleCombo : public Component
    {
    public:
        DoubleKeyValSimpleCombo(
            const std::string &id,
            const std::vector<std::string> &list,
            int default_index,
            const std::string &label = "String",
            const std::string &hex_border_idle = "#454545B2",
            const std::string &hex_bg_hovered = "#343434FF",
            const std::string &hex_border_hovered = "#454545B2",
            const std::string &hex_bg_clicked = "#444444FF",
            const std::string &hex_border_clicked = "#454545B2")
            : Component(id),
              m_Label(label),
              m_List(list),
              m_DefaultIndex(default_index),
              m_SelectedIndex(default_index)
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

                ImGui::SetNextItemWidth(-FLT_MIN);

                static ImGuiComboFlags flags = 0;

                if (m_DefaultIndex < 0 || m_DefaultIndex >= m_List.size())
                    m_DefaultIndex = 0;

                const char *combo_preview_value = m_List[m_SelectedIndex].c_str();
                if (ImGui::BeginCombo(m_Label.c_str(), combo_preview_value, flags))
                {
                    for (int n = 0; n < m_List.size(); n++)
                    {
                        const bool is_selected = (m_SelectedIndex == n);
                        if (ImGui::Selectable(m_List[n].c_str(), is_selected))
                        {
                            if (m_SelectedIndex != n)
                            {
                                m_SelectedIndex = n;
                                UpdateLastChangedTime();
                            }
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "selected")
            {
                return std::to_string(m_SelectedIndex);
            }

            return "none";
        }

    private:
        std::vector<std::string> m_List;
        int m_DefaultIndex;
        int m_SelectedIndex;
        std::string m_Label;

        std::string m_LastChanged;
        void UpdateLastChangedTime()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            m_LastChanged = std::ctime(&now_c);
            m_LastChanged.erase(m_LastChanged.length() - 1);
        }
    };
}

#endif // UIKIT_V1_INPUTS_KEYVAL_DOUBLE
