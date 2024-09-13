#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_SIMPLE_COMBO
#define UIKIT_V1_SIMPLE_COMBO

namespace UIKit
{

inline void SimpleCombo(
    const std::string &label,
    const std::vector<std::string> &list,
    int default_index,
    const std::string &hex_text_color = "#B1FF31FF")
{
    static ImGuiComboFlags flags = 0;
    
    if (default_index < 0 || default_index >= list.size())
        default_index = 0;

    static int item_current_idx = default_index;

    const char *combo_preview_value = list[item_current_idx].c_str();
    if (ImGui::BeginCombo(label.c_str(), combo_preview_value, flags))
    {
        for (int n = 0; n < list.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(list[n].c_str(), is_selected))
                item_current_idx = n;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}


}

#endif // UIKIT_V1_SIMPLE_COMBO
