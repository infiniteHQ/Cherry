#pragma once
#include "../../../../../platform/engine/app.hpp"
#include "../../../../../platform/engine/components.hpp"

#ifndef CHERRY_KIT_WINDOW_SIMPLE
#define CHERRY_KIT_WINDOW_SIMPLE

namespace Cherry
{
    namespace WindowComponents
    {
        class WindowSimple : public AppWindowWrapper
        {
            public:
            WindowSimple(){}
        };
    }

    // End-User API
    namespace Kit
    {
        std::shared_ptr<AppWindow> WindowSimple(const std::string &label, const std::function<void()>& callback)
        {
            auto appwin =WindowComponents::WindowSimple::Create(label, callback)->GetAppWindow();
            appwin->SetClosable(true);
            appwin->SetCloseCallback([&](){
                Cherry::DeleteAppWindow(appwin);
            });
            return appwin;
        }
    }

}

#endif // CHERRY_KIT_WINDOW_SIMPLE
