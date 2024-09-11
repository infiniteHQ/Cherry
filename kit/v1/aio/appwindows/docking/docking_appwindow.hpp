#include "../../../../../platform/engine/app.hpp"

/** WARNING
 * 
 *  DockingAppWindow was made to be used with framework "SubAppWindows",
 *  You can so use sub app windows in this parent app window.
 *  The dockspace of this window is different than main windows.
*/

#ifndef UIKIT_V1_DOCKING_APPWINDOW
#define UIKIT_V1_DOCKING_APPWINDOW

#include "../../../aio/appwindows/basic/empty_appwindow.hpp"

namespace UIKit
{
    class DockingAppWindow
    {
    public:
        DockingAppWindow();
        DockingAppWindow(const std::string& name);

        std::shared_ptr<UIKit::AppWindow> m_AppWindow;
    };
}

#endif //UIKIT_V1_DOCKING_APPWINDOW