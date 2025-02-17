#include "../base.hpp"

namespace Cherry
{
    namespace GUI
    {
        // Legacy ImGui style variables
        void PushStyleVar(ImGuiStyleVar idx, float val);
        void PushStyleVar(ImGuiCol idx, const ImVec2 &val);
        void PopStyleVar(int count = 1);

        // Legacy ImGui style colors
        void PushStyleColor(ImGuiCol idx, const ImVec4 &col);
        void PushStyleColor(ImGuiCol idx, ImU32 col);
        void PopStyleColor(int count = 1);

        // Legacy ImGui buttons
        bool Button(const char *label, const ImVec2 &size = ImVec2(0, 0));
    }
}

namespace CherryGUI = Cherry::GUI;