#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// Margin
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_STYLE_MARGIN
#define CHERRY_STYLE_MARGIN

namespace Cherry
{
    // End-User API
    namespace Style
    {
        inline void AddXMargin(const float& margin)
        {
            CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() + margin);
        }

        inline void AddYMargin(const float& margin)
        {
            CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + margin);
        }        
        
        inline void RemoveXMargin(const float& margin)
        {
            CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() - margin);
        }

        inline void RemoveYMargin(const float& margin)
        {
            CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() - margin);
        }
    }

}

#endif // CHERRY_STYLE_MARGIN
