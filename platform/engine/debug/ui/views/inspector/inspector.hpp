#include "../../../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_INSPECTOR
#define CHERRY_DEBUG_VIEW_INSPECTOR

#include "../../../../app_window/app_window.hpp"

class InspectorView : public std::enable_shared_from_this<InspectorView> {
public:
  InspectorView(const std::string &name);
  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<InspectorView> Create(const std::string &name);

  void SetupRenderCallback();

  void Render();
  void RenderDragDropWidget(
      const std::shared_ptr<Cherry::WindowDragDropState> &state);
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif // CHERRY_DEBUG_VIEW_INSPECTOR
#endif // CHERRY_DEBUG