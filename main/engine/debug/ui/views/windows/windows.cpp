//
//  windows.cpp
//  Sources for devtools windows inspector
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "windows.hpp"
#ifdef CHERRY_DEBUG

#include "../../../../app/app.hpp"

WindowsView::WindowsView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetInternalPaddingX(0.0f);
  m_AppWindow->SetInternalPaddingY(0.0f);
}

std::shared_ptr<Cherry::AppWindow> &WindowsView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<WindowsView> WindowsView::Create(const std::string &name) {
  auto instance = std::shared_ptr<WindowsView>(new WindowsView(name));
  instance->SetupRenderCallback();
  return instance;
}

void WindowsView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self)
      self->Render();
  });
}

void WindowsView::Render() {
  for (auto &win : CherryApp.GetWindows()) {
    ImGui::TextColored(Cherry::HexToRGBA("#BB3434"), win->GetName().c_str());
    for (auto &appwin : CherryApp.GetAppWindows()) {
      if (appwin->m_HaveParentAppWindow) {
        continue;
      }

      if (!appwin->CheckWinParent(win->GetName())) {
        continue;
      }
      CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() + 10.0f);
      ImGui::TextColored(Cherry::HexToRGBA("#AA34AA"), appwin->m_IdName.c_str());

      for (auto &appwinchild : CherryApp.GetAppWindows()) {
        if (appwinchild->m_HaveParentAppWindow && appwinchild->m_ParentAppWindow->m_IdName == appwin->m_IdName) {
          CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() + 20.0f);
          ImGui::TextColored(Cherry::HexToRGBA("#3434BB"), appwinchild->m_IdName.c_str());
          ImGui::SameLine();
          ImGui::TextColored(Cherry::HexToRGBA("#AB8923"), " -->");
          ImGui::SameLine();
          ImGui::TextColored(Cherry::HexToRGBA("#AB8923"), appwinchild->m_WinParent.c_str());
        }
      }
    }
  }
}

#endif  // CHERRY_DEBUG