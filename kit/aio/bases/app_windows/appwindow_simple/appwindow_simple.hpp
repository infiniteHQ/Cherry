#pragma once
#include "../../../../../platform/engine/app/app.hpp"
#include "../../../../../platform/engine/components.hpp"

#ifndef CHERRY_KIT_WINDOW_SIMPLE
#define CHERRY_KIT_WINDOW_SIMPLE

namespace Cherry {
namespace WindowComponents {
class WindowSimple : public AppWindowWrapper {
public:
  WindowSimple() {}
};
} // namespace WindowComponents

// End-User API
namespace Kit {
inline std::shared_ptr<AppWindow>
WindowSimple(const std::string &label, const std::function<void()> &callback) {
  auto appwin =
      WindowComponents::WindowSimple::Create(label, callback)->GetAppWindow();
  appwin->SetClosable(true);
  appwin->SetCloseCallback([&]() { Cherry::DeleteAppWindow(appwin); });
  return appwin;
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_WINDOW_SIMPLE
