#pragma once
#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CONTENT_CONTENT_LIST_SIMPLE
#define UIKIT_V1_CONTENT_CONTENT_LIST_SIMPLE

#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"
#include "../../../../components/lists/custom_lists.hpp"
#include "../../../../components/inputs/simple/simple_inputs.hpp"

namespace UIKit
{

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class ContentListSimple
    {
    public:
        ContentListSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        ContentListSimple(const std::string &name);
        ContentListSimple();

        void RefreshRender(const std::shared_ptr<ContentListSimple> &instance);

    private:
        // Ui Components
        std::shared_ptr<CustomButtonSimple> cp_ButtonOne;
        std::shared_ptr<SimpleTree> cp_Tree;
        std::shared_ptr<SimpleStringInput> cp_SearchBar;   

        // Values
        std::shared_ptr<std::string> v_StringOne; 

        // Renderer
        std::shared_ptr<UIKit::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_CONTENT_CONTENT_LIST_SIMPLE