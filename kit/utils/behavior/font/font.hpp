#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// Margin
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_STYLE_FONT
#define CHERRY_STYLE_FONT

namespace Cherry
{
    // End-User API
    namespace Style
    {
        // Font size
        inline void SetNextComponentFontSize(const float &font_size)
        {
            // Set the font size on BeforeComponentRender (generic component callback)
        }

        inline void SetContextFontSize(const float &font_size)
        {
            // Apply to the current kit context (window or UpgradeIncrementorLevel)
        }

        inline void PopFontSize()
        {
            CherryGUI::GetFont()->Scale = Application::GetCurrentRenderedWindow()->m_Specifications.FontGlobalScale;
            CherryGUI::PopFont();
        }

        inline void PushFontSize(const float &font_size)
        {
            CherryGUI::GetFont()->Scale = font_size;
            CherryGUI::PushFont(CherryGUI::GetFont());
        }

        // Font selection
        inline void SetNextComponentFont(const std::string &font_name)
        {
            // Set the font on BeforeComponentRender (generic component callback)
            CherryGUI::SetCurrentFont(Cherry::Application::GetFont(font_name));
        }

        inline void SetContextFont(const std::string &font_name)
        {
            // Apply to the current kit context (window or UpgradeIncrementorLevel)
            CherryGUI::SetCurrentFont(Cherry::Application::GetFont(font_name));
        }

        inline void SetFont(const std::string &font_name)
        {
            CherryGUI::SetCurrentFont(Cherry::Application::GetFont(font_name));
        }
    }
}

#endif // CHERRY_STYLE_FONT
