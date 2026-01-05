#include "../../../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_GENERAL
#define CHERRY_DEBUG_VIEW_GENERAL

#include "../../../../app_window/app_window.hpp"

class GeneralView : public std::enable_shared_from_this<GeneralView> {
public:
  GeneralView(const std::string &name);
  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<GeneralView> Create(const std::string &name);
  void SetupRenderCallback();
  void Render();
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif // CHERRY_DEBUG_VIEW_GENERAL
#endif // CHERRY_DEBUG