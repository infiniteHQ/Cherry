#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_HEADERS_SIMPLECOLLAPSING
#define UIKIT_V1_HEADERS_SIMPLECOLLAPSING

namespace UIKit
{

    inline bool SimpleCollapsingHeader(
        const std::string &label,
        const std::function<void()> &content)
    {
        bool openned = false;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
        if (ImGui::CollapsingHeader(label.c_str()))
        {
            openned = true;

            if (content)
            {
                content();
            }
        }
        ImGui::PopStyleVar();

        return openned;
    }

}

#endif // UIKIT_V1_HEADERS_SIMPLECOLLAPSING
