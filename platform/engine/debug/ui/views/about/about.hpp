#include "../../../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_ABOUT
#define CHERRY_DEBUG_VIEW_ABOUT

#include "../../../../app_window/app_window.hpp"

class AboutView : public std::enable_shared_from_this<AboutView> {
public:
  AboutView(const std::string &name);

  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<AboutView> Create(const std::string &name);

  void SetupRenderCallback();
  void Render();

  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif // CHERRY_DEBUG_VIEW_ABOUT
#endif // CHERRY_DEBUG