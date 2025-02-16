#include "wrappers.hpp"

namespace Cherry
{
    void PushStyleVar(ImGuiStyleVar idx, float val)
    {
        ImGui::PushStyleVar(idx, val);
    }

    void PushStyleVar(ImGuiCol idx, const ImVec2 &val)
    {
        ImGui::PushStyleVar(idx, val);
    }

    void PopStyleVar(int count)
    {
        ImGui::PopStyleVar(count);
    }

    void PushStyleColor(ImGuiCol idx, const ImVec4 &col)
    {
        ImGui::PushStyleColor(idx, col);
    }

    void PushStyleColor(ImGuiCol idx, ImU32 col)
    {
        ImGui::PushStyleColor(idx, col);
    }

    void PopStyleColor(int count)
    {
        ImGui::PopStyleColor(count);
    }
    
    bool Button(const char* label, const ImVec2& size)
    {
        return ImGui::Button(label, size);
    }
}