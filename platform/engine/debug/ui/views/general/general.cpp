
#include "general.hpp"
#ifdef CHERRY_DEBUG
#include "../../../../app/app.hpp"

GeneralView::GeneralView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetClosable(true);
  m_AppWindow->m_CloseCallback = [=]() { m_AppWindow->SetVisibility(false); };
  m_AppWindow->SetInternalPaddingX(8.0f);
  m_AppWindow->SetInternalPaddingY(8.0f);
  std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
}

std::shared_ptr<Cherry::AppWindow> &GeneralView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<GeneralView> GeneralView::Create(const std::string &name) {
  auto instance = std::shared_ptr<GeneralView>(new GeneralView(name));
  instance->SetupRenderCallback();
  return instance;
}

void GeneralView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self) {
      self->Render();
    }
  });
}

void GeneralView::Render() {
  float deltaTime = 0.0f;
  float fps = ImGui::GetIO().Framerate;
  float ms = 1000.0f / fps;

  ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.8f, 1.0f), "SYSTEM STATUS");
  ImGui::Separator();
  ImGui::Text("FPS: %.1f", fps);
  ImGui::SameLine(150);
  ImGui::Text("Frame Time: %.3f ms", ms);

  static float values[90] = {0};
  static int values_offset = 0;
  static double refresh_time = 0.0;

  if (ImGui::GetTime() > refresh_time) {
    values[values_offset] = fps;
    values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
    refresh_time = ImGui::GetTime() + 1.0f / 60.0f; // 60Hz
  }

  ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
  ImVec2 canvas_size = ImVec2(ImGui::GetContentRegionAvail().x, 80.0f);
  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  draw_list->AddRectFilled(
      canvas_pos,
      ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
      IM_COL32(30, 30, 30, 150), 5.0f);

  if (IM_ARRAYSIZE(values) > 0) {
    float max_val = 144.0f;

    for (int i = 0; i < IM_ARRAYSIZE(values) - 1; i++) {
      int idx0 = (values_offset + i) % IM_ARRAYSIZE(values);
      int idx1 = (values_offset + i + 1) % IM_ARRAYSIZE(values);

      float val0 = (values[idx0] / max_val);
      float val1 = (values[idx1] / max_val);
      if (val0 > 1.0f)
        val0 = 1.0f;
      if (val1 > 1.0f)
        val1 = 1.0f;

      ImVec2 p0 = ImVec2(canvas_pos.x + (i * (canvas_size.x / 90)),
                         canvas_pos.y + canvas_size.y - (val0 * canvas_size.y));
      ImVec2 p1 = ImVec2(canvas_pos.x + ((i + 1) * (canvas_size.x / 90)),
                         canvas_pos.y + canvas_size.y - (val1 * canvas_size.y));

      draw_list->AddLine(p0, p1, IM_COL32(0, 255, 200, 255), 2.0f);

      draw_list->AddRectFilledMultiColor(
          ImVec2(p0.x, p0.y), ImVec2(p1.x, canvas_pos.y + canvas_size.y),
          IM_COL32(0, 255, 200, 100), IM_COL32(0, 255, 200, 100),
          IM_COL32(0, 255, 200, 0), IM_COL32(0, 255, 200, 0));
    }
  }

  ImGui::Dummy(ImVec2(0, 85.0f));

  if (ImGui::CollapsingHeader("DSL & Memory")) {
    static float mem_usage = 0.45f;
    ImGui::Text("VRAM Usage");
    ImGui::ProgressBar(mem_usage, ImVec2(-1.0f, 0.0f));
  }
}

#endif // CHERRY_DEBUG