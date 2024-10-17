#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_MENUITEM
#define UIKIT_V1_MENUITEM

namespace Cherry
{
    inline bool MenuItemSimple(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true)
    {
        return ImGui::MenuItem(label, shortcut, selected, enabled);
    }

    inline bool MenuItemSimple(const char* label, const char* shortcut, bool* p_selected, bool enabled = false)
    {
        return ImGui::MenuItem(label, shortcut, p_selected, enabled);
    }

    inline bool MenuItemImage(const char* label, const char* shortcut, ImTextureID texture, bool selected, bool enabled = true)
    {
        return ImGui::MenuItem(label, shortcut, texture, selected, enabled);
    }
}

#endif // UIKIT_V1_MENUITEM
