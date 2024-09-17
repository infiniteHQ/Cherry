#include "../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_EMPTY_APPWINDOW
#define UIKIT_V1_EMPTY_APPWINDOW

#include "../../../components/buttons/custom_buttons.hpp"
#include "../../../components/buttons/image_buttons.hpp"
#include "../../../components/buttons/danger_buttons.hpp"
#include "../../../components/buttons/cancel_buttons.hpp"

namespace UIKit
{

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class EmptyAppWindow
    {
    public:
        EmptyAppWindow(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        EmptyAppWindow(const std::string &name);
        EmptyAppWindow();

        std::shared_ptr<NCustomButtonSimple> cp_ButtonOne;

        std::shared_ptr<UIKit::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_EMPTY_APPWINDOW