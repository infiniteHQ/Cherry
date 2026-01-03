#include "../../../../../cherry.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_ABOUT
#define CHERRY_DEBUG_VIEW_ABOUT

class AboutView : public std::enable_shared_from_this<AboutView> {
public:
  AboutView(const std::string &name) {
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

  static std::shared_ptr<AboutView> Create(const std::string &name) {
    auto instance = std::shared_ptr<AboutView>(new AboutView(name));
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

  void Render() { Cherry::Kit::TextSimple(m_AppWindow->m_WinParent); }

  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif // CHERRY_DEBUG_VIEW_ABOUT
#endif // CHERRY_DEBUG