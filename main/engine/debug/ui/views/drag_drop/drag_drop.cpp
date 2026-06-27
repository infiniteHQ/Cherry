//
//  drag_drop.cpp
//  Sources for devtools windows drag and drop feature inspector
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "drag_drop.hpp"
#ifdef CHERRY_DEBUG

#include "../../../../app/app.hpp"

DragDropView::DragDropView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetInternalPaddingX(0.0f);
  m_AppWindow->SetInternalPaddingY(0.0f);
}

std::shared_ptr<Cherry::AppWindow> &DragDropView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<DragDropView> DragDropView::Create(const std::string &name) {
  auto instance = std::shared_ptr<DragDropView>(new DragDropView(name));
  instance->SetupRenderCallback();
  return instance;
}

void DragDropView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self)
      self->Render();
  });
}
static const char *DockEmplacementToString(Cherry::DockEmplacement e) {
  switch (e) {
    case Cherry::DockEmplacement::DockLeft: return "Left";
    case Cherry::DockEmplacement::DockRight: return "Right";
    case Cherry::DockEmplacement::DockUp: return "Up";
    case Cherry::DockEmplacement::DockDown: return "Down";
    case Cherry::DockEmplacement::DockFull: return "Center";
    default: return "Unknown";
  }
}

void DragDropView::Render() {
  if (!CherryApp.LatestRequest) {
    ImGui::TextColored(Cherry::HexToRGBA("#F87171"), "No request");
  } else {
    auto &r = *CherryApp.LatestRequest;

    ImGui::TextColored(Cherry::HexToRGBA("#4ADE80"), "Latest request :");
    ImGui::Separator();

    ImGui::Text("m_ParentWindow       : %s", r.m_ParentWindow.c_str());
    ImGui::Text("m_ParentAppWindowHost: %s", r.m_ParentAppWindowHost.c_str());
    ImGui::Text("m_ParentAppWindow    : %s", r.m_ParentAppWindow.c_str());
    ImGui::Text("m_DockPlace          : %s", DockEmplacementToString(r.m_DockPlace));

    ImGui::Separator();

    ImGui::Text("m_IsHandled   : %s", r.m_IsHandled ? "true" : "false");
    ImGui::Text("m_FromSave    : %s", r.m_FromSave ? "true" : "false");
    ImGui::Text("m_FromNewWindow: %s", r.m_FromNewWindow ? "true" : "false");
    ImGui::Text("m_IsObsolete  : %s", r.m_IsObsolete ? "true" : "false");
  }
}
#endif  // CHERRY_DEBUG