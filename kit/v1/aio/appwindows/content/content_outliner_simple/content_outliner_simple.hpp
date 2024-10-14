#pragma once
#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CONTENT_CONTENT_OUTLINER_SIMPLE
#define UIKIT_V1_CONTENT_CONTENT_OUTLINER_SIMPLE

#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"
#include "../../../../components/lists/custom_lists.hpp"
#include "../../../../components/inputs/simple/simple_inputs.hpp"

namespace Cherry
{

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class ContentOutlinerSimple : public std::enable_shared_from_this<ContentOutlinerSimple>
    {
    public:
        ContentOutlinerSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        ContentOutlinerSimple(const std::string &name);
        ContentOutlinerSimple();

        void RefreshRender(const std::shared_ptr<ContentOutlinerSimple> &instance);

        std::shared_ptr<Cherry::AppWindow> &GetAppWindow();

        static std::shared_ptr<ContentOutlinerSimple> Create(const std::string &name);
        void SetupRenderCallback();
        void Render();
    private:
        // Ui Components
        std::shared_ptr<CustomButtonSimple> cp_ButtonOne;
        std::shared_ptr<SimpleTree> cp_Tree;
        std::shared_ptr<SimpleStringInput> cp_SearchBar;   

        // Values
        std::shared_ptr<std::string> v_StringOne; 

        // Renderer
        std::shared_ptr<Cherry::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_CONTENT_CONTENT_OUTLINER_SIMPLE