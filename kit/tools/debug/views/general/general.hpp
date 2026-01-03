#include "../../../../../cherry.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_GENERAL
#define CHERRY_DEBUG_VIEW_GENERAL

class GeneralView : public std::enable_shared_from_this<GeneralView> {
public:
  GeneralView(const std::string &name) {
    m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
    m_AppWindow->SetIcon(
        Cherry::GetPath("resources/imgs/icons/misc/icon_home.png"));

    m_AppWindow->SetClosable(true);
    m_AppWindow->m_CloseCallback = [=]() { m_AppWindow->SetVisibility(false); };

    m_AppWindow->SetInternalPaddingX(8.0f);
    m_AppWindow->SetInternalPaddingY(8.0f);

    std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
  }

  std::shared_ptr<Cherry::AppWindow> &GetAppWindow() { return m_AppWindow; }

  static std::shared_ptr<GeneralView> Create(const std::string &name) {
    auto instance = std::shared_ptr<GeneralView>(new GeneralView(name));
    instance->SetupRenderCallback();
    return instance;
  }

  void SetupRenderCallback() {
    auto self = shared_from_this();
    m_AppWindow->SetRenderCallback([self]() {
      if (self) {
        self->Render();
      }
    });
  }

  void Render() { Cherry::Kit::TextSimple("te"); }

  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif // CHERRY_DEBUG_VIEW_GENERAL
#endif // CHERRY_DEBUG