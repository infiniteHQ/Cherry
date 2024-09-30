#include "../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CUSTOM_GROUPED_BUTTONS
#define UIKIT_V1_CUSTOM_GROUPED_BUTTONS

#include "./custom_buttons.hpp"

namespace UIKit
{
    class CustomGroupedButtons : public Component
    {
    public:
        CustomGroupedButtons(
            const std::string &id,
            const std::string &label = "Button")
            : Component(id),
              m_Label(label)
        {
            m_ID = id;
        }


    bool Render(const std::string &duplication_name, const ImVec2 &size = ImVec2(0, 0)) {
        return false;
    }


        void SetBackgroundColor() {}

        std::string GetData(const std::string &data_type) override
        {
            return "none";
        }

    private:
        std::string m_Label;
        
        //std::vector<T> m_ButtonsList;
    };


}

#endif // UIKIT_V1_CUSTOM_GROUPED_BUTTONS
