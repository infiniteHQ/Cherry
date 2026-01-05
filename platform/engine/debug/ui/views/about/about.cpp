
#include "about.hpp"
#ifdef CHERRY_DEBUG
#include "../../../../app/app.hpp"

AboutView::AboutView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetClosable(true);
  m_AppWindow->m_CloseCallback = [=]() { m_AppWindow->SetVisibility(false); };
  m_AppWindow->SetInternalPaddingX(8.0f);
  m_AppWindow->SetInternalPaddingY(8.0f);
  std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
}

std::shared_ptr<Cherry::AppWindow> &AboutView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<AboutView> AboutView::Create(const std::string &name) {
  auto instance = std::shared_ptr<AboutView>(new AboutView(name));
  instance->SetupRenderCallback();
  return instance;
}

void AboutView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self) {
      self->Render();
    }
  });
}

void AboutView::Render() {}

#endif // CHERRY_DEBUG