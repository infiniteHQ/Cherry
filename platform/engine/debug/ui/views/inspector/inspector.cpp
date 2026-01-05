
#include "inspector.hpp"
#ifdef CHERRY_DEBUG
#include "../../../../app/app.hpp"

InspectorView::InspectorView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetClosable(true);
  m_AppWindow->m_CloseCallback = [=]() { m_AppWindow->SetVisibility(false); };
  m_AppWindow->SetInternalPaddingX(8.0f);
  m_AppWindow->SetInternalPaddingY(8.0f);
  std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
}

std::shared_ptr<Cherry::AppWindow> &InspectorView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<InspectorView> InspectorView::Create(const std::string &name) {
  auto instance = std::shared_ptr<InspectorView>(new InspectorView(name));
  instance->SetupRenderCallback();
  return instance;
}

void InspectorView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self) {
      self->Render();
    }
  });
}
void InspectorView::Render() {
  if (ImGui::BeginTabBar("InspectorTabs")) {
    if (ImGui::BeginTabItem("Components")) {
      //
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Effects")) {
      //
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Animations")) {
      //
      ImGui::EndTabItem();
    }

    //
    if (ImGui::BeginTabItem("DragDrop")) {
      auto &state = CherryApp.GetCurrentDragDropState();

      if (!state) {
        ImGui::TextDisabled("No drag drop state available.");
      } else {
        RenderDragDropWidget(state);
      }
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }
}

void InspectorView::RenderDragDropWidget(
    const std::shared_ptr<Cherry::WindowDragDropState> &state) {
  bool isDragging = state->DockIsDragging || (state->DragOwner != "none");
  ImVec4 statusColor = isDragging ? ImVec4(0.15f, 0.8f, 0.15f, 1.0f)
                                  : ImVec4(0.8f, 0.15f, 0.15f, 1.0f);

  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
  ImGui::BeginChild("StatusHeader", ImVec2(0, 40), true);
  {
    ImGui::Columns(2, "statusCols", false);
    ImGui::SetColumnWidth(0, 100);

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    draw_list->AddCircleFilled(ImVec2(p.x + 10, p.y + 10), 6.0f,
                               ImGui::GetColorU32(statusColor));

    ImGui::Indent(25);
    ImGui::Text(isDragging ? "ACTIVE" : "IDLE");
    ImGui::Unindent(25);

    ImGui::NextColumn();
    ImGui::Text("Owner: %s", state->DragOwner.c_str());
    ImGui::Columns(1);
  }
  ImGui::EndChild();
  ImGui::PopStyleColor();

  ImGui::Spacing();

  if (ImGui::BeginTable("DragDetails", 2,
                        ImGuiTableFlags_BordersInnerH |
                            ImGuiTableFlags_RowBg)) {
    ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed,
                            120.0f);
    ImGui::TableSetupColumn("Value");

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextDisabled("WINDOWING");

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Last Window");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", state->LastDraggingWindow.c_str());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("App Host");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", state->LastDraggingAppWindowHost.c_str());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextDisabled("DOCKING");

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Emplacement");
    ImGui::TableSetColumnIndex(1);
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Emplacement #%d",
                       (int)state->LastDraggingPlace);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Parenting");
    ImGui::TableSetColumnIndex(1);
    ImGui::Checkbox("Has Parent", &state->LastDraggingAppWindowHaveParent);

    ImGui::EndTable();
  }

  ImGui::Separator();
  ImGui::Text("Mouse Context: %d, %d", state->mouseX, state->mouseY);

  ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
  ImVec2 canvas_sz = ImVec2(ImGui::GetContentRegionAvail().x, 60.0f);
  ImDrawList *dl = ImGui::GetWindowDrawList();
  dl->AddRectFilled(
      canvas_p0, ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y),
      IM_COL32(30, 30, 30, 255));

  if (isDragging) {
    float relX = (float)(state->mouseX % (int)canvas_sz.x);
    dl->AddCircleFilled(ImVec2(canvas_p0.x + 50, canvas_p0.y + 30), 4.0f,
                        IM_COL32(255, 255, 0, 255));
  }
}

#endif // CHERRY_DEBUG