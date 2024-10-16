#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_EMPTY_APPWINDOW
#define UIKIT_V1_EMPTY_APPWINDOW

#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"

namespace Cherry
{
    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class EmptyAppWindow : public std::enable_shared_from_this<EmptyAppWindow>
    {
    public:
        EmptyAppWindow(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        EmptyAppWindow(const std::string &name);
        EmptyAppWindow();

        void Render();
        std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
        static std::shared_ptr<EmptyAppWindow> Create(const std::string &name);
        static std::shared_ptr<EmptyAppWindow> Create(const std::string &name, std::shared_ptr<Cherry::AppWindow> parent);
        
        void SetupRenderCallback();

        std::shared_ptr<CustomButtonSimple> cp_ButtonOne;
        std::shared_ptr<Cherry::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_EMPTY_APPWINDOW