#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_TABLES_SIMPLES
#define UIKIT_V1_TABLES_SIMPLES

namespace Cherry
{
    class SimpleTable : public Component
    {
    public:
        struct SimpleTableRow
        {
            std::vector<std::function<void()>> m_ColumnCallbacks;
            SimpleTableRow(std::vector<std::function<void()>> callbacks)
                : m_ColumnCallbacks(callbacks) {}
        };

        SimpleTable(
            const std::string &id,
            const std::string &label,
            const std::vector<std::string> &columns,
            const std::string &hex_text_color = "#B1FF31FF")
            : Component(id),
              m_Label(label),
              m_Columns(columns),
              m_HexTextColor(hex_text_color)
        {
            UpdateLastChangedTime();
        }

        void Render(std::vector<SimpleTableRow> rows, const std::string &duplication_name = "base", ImGuiTableFlags other_flags = 0)
        {
            ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | other_flags;

            ImGuiStyle style = ImGui::GetStyle();
            m_HeaderCellPaddingX = style.CellPadding.x;
            m_HeaderCellPaddingY = style.CellPadding.y;
            m_RowsCellPaddingX = style.CellPadding.x;
            m_RowsCellPaddingY = style.CellPadding.y;

            if (ImGui::BeginTable(m_Label.c_str(), m_Columns.size(), flags))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(m_HeaderCellPaddingX, m_HeaderCellPaddingY));

                for (int i = 0; i < m_Columns.size(); ++i)
                {
                    if (i == m_Columns.size() - 1)
                    {
                        ImGui::TableSetupColumn(m_Columns[i].c_str(), ImGuiTableColumnFlags_NoResize);
                    }
                    else
                    {
                        ImGui::TableSetupColumn(m_Columns[i].c_str());
                    }
                }

                if (m_HeaderVisibility)
                {
                    ImGui::TableHeadersRow();
                }

                ImGui::PopStyleVar();

                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(m_RowsCellPaddingX, m_RowsCellPaddingY));
                for (int row = 0; row < rows.size(); row++)
                {
                    ImGui::TableNextRow();
                    for (int column = 0; column < m_Columns.size(); column++)
                    {
                        ImGui::TableSetColumnIndex(column);
                        if (column < rows[row].m_ColumnCallbacks.size())
                        {
                            rows[row].m_ColumnCallbacks[column]();
                        }
                    }
                }
                ImGui::PopStyleVar();
                ImGui::EndTable();
            }
        }

        void SetHeaderCellPaddingY(const float &padding_y)
        {
            m_HeaderCellPaddingY = padding_y;
        }

        void SetHeaderCellPaddingX(const float &padding_x)
        {
            m_HeaderCellPaddingX = padding_x;
        }

        void SetRowsCellPaddingY(const float &padding_y)
        {
            m_RowsCellPaddingY = padding_y;
        }

        void SetRowsCellPaddingX(const float &padding_x)
        {
            m_RowsCellPaddingX = padding_x;
        }

        void SetHeaderVisibility(const bool &visibility)
        {
            m_HeaderVisibility = visibility;
        }

        std::string GetData(const std::string &data_type) override
        {
            return "none";
        }

    private:
        std::string m_Label;
        std::vector<std::string> m_Columns;
        std::string m_HexTextColor;
        int m_DefaultIndex;
        int m_SelectedIndex;
        std::string m_LastChanged;

        // Design properties
        float m_HeaderCellPaddingX;
        float m_HeaderCellPaddingY;
        float m_RowsCellPaddingX;
        float m_RowsCellPaddingY;

        bool m_HeaderVisibility;

        void UpdateLastChangedTime()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            m_LastChanged = std::ctime(&now_c);
            m_LastChanged.erase(m_LastChanged.length() - 1);
        }
    };
}

#endif // UIKIT_V1_TABLES_SIMPLES
