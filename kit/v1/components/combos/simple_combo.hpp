#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_SIMPLE_COMBO
#define UIKIT_V1_SIMPLE_COMBO

namespace Cherry
{
    class ComboSimple : public Component
    {
    public:
        ComboSimple(
            const std::string &id,
            const std::string &label,
            const std::vector<std::string> &list,
            int default_index,
            const std::string &hex_text_color = "#B1FF31FF")
            : Component(id),
              m_Label(label),
              m_List(list),
              m_DefaultIndex(default_index),
              m_HexTextColor(hex_text_color),
              m_SelectedIndex(default_index)
        {
            UpdateLastChangedTime();
        }

        void Render(const std::string &duplication_name)
        {
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

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "selected_index")
            {
                return std::to_string(m_SelectedIndex);
            }
            else if (data_type == "selected_string")
            {
                return m_List[m_SelectedIndex];
            }
            else if (data_type == "lastChanged")
            {
                return m_LastChanged;
            }
            return "Unknown data type";
        }

    private:
        std::string m_Label;
        std::vector<std::string> m_List;
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

#endif // UIKIT_V1_SIMPLE_COMBO
