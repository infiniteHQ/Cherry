#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_TABLES_SIMPLES
#define UIKIT_V1_TABLES_SIMPLES

namespace UIKit
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

        void Render(std::vector<SimpleTableRow> rows, const std::string &duplication_name = "base")
        {
            ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

            if (ImGui::BeginTable(m_Label.c_str(), m_Columns.size(), flags))
            {
                for (auto &column : m_Columns)
                {
                    ImGui::TableSetupColumn(column.c_str());
                }

                ImGui::TableHeadersRow();

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
                ImGui::EndTable();
            }
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
