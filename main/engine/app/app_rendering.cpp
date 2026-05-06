#include "app.hpp"

namespace Cherry {

  void Application::HandleTabsModeRendering(Window *window) {
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGuiID dockspaceID = ImGui::GetID("MainDockspace");

    float oldsize = ImGui::GetFont()->Scale;
    ImGui::GetFont()->Scale *= window->GetSpecifications().FontGlobalScale;
    ImGui::PushFont(ImGui::GetFont());

    ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
    ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

    ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);

    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 8.0f));

    ImGui::GetCurrentContext()->Style.DockSpaceMenubarPaddingY = 12.0f;
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);

    ImGui::GetFont()->Scale = oldsize;
    ImGui::PopFont();

    bool context_loaded = false;
    for (auto &appwindow : m_AppWindows) {
      if (appwindow) {
        for (auto &subwin : m_AppWindows) {
          if (context_loaded) {
            continue;
          }

          if (subwin->m_HaveParentAppWindow) {
            if (subwin->m_ParentAppWindow->m_IdName == appwindow->m_IdName) {
              SetCurrentRenderedAppWindow(appwindow);
              appwindow->CtxRender(&m_RedockRequests, window->GetName());
              context_loaded = true;
            }
          }
        }

        if (appwindow->CheckWinParent(window->GetName()) && !appwindow->m_HaveParentAppWindow) {
          SetCurrentRenderedAppWindow(appwindow);
          appwindow->CtxRender(&m_RedockRequests, window->GetName());
          context_loaded = true;
        }
      }
    }
  }

  void Application::HandleSimpleRendering(Window *window) {
    float oldsize = ImGui::GetFont()->Scale;
    ImGui::GetFont()->Scale *= window->GetSpecifications().FontGlobalScale;
    ImGui::PushFont(ImGui::GetFont());
    if (m_MainRenderCallback) {
      m_MainRenderCallback();
    }
    ImGui::GetFont()->Scale = oldsize;
    ImGui::PopFont();
  }

  void Application::HandleSimpleWindowRendering(Window *window) {
    bool finded = false;
    for (auto &appwin : Application::Get().GetAppWindows()) {
      if (appwin) {
        if (appwin->m_Name == Application::GetCurrentRenderedWindow()->GetSpecifications().UniqueAppWindowName) {
          SetCurrentRenderedAppWindow(appwin);
          appwin->CtxRender(nullptr, window->GetName());
          finded = true;
        }
      }
    }

    if (!finded) {
      ImGui::TextColored(
          ImVec4(1.0f, 0.2f, 0.2f, 1.0f),
          "Cherry Error : You need to specify the "
          "\"UniqueAppWindowName\" in spec !");
    }
  }

  void Application::HandleDockingModeRendering(Window *window) {
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGuiID dockspaceID = ImGui::GetID("MainDockspace");

    float oldsize = ImGui::GetFont()->Scale;
    ImGui::GetFont()->Scale *= window->GetSpecifications().FontGlobalScale;
    ImGui::PushFont(ImGui::GetFont());

    ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
    ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

    ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);

    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 8.0f));

    ImGui::GetCurrentContext()->Style.DockSpaceMenubarPaddingY = 12.0f;
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);

    ImGui::GetFont()->Scale = oldsize;
    ImGui::PopFont();

    if (GetDockIsDragging()) {
      for (auto &appwin : m_AppWindows) {
        if (GetCurrentDragDropState()->LastDraggingAppWindowHost == appwin->m_IdName) {
          if (!appwin->m_HaveParentAppWindow) {
            Window::ShowDockingPreview(dockspaceID, window, GetCurrentDragDropState(), appwin);
          }
        }
      }
    }
    bool context_loaded = false;
    for (auto &appwindow : m_AppWindows) {
      if (appwindow) {
        for (auto &subwin : m_AppWindows) {
          if (context_loaded) {
            continue;
          }

          if (subwin->m_HaveParentAppWindow) {
            if (subwin->m_ParentAppWindow->m_IdName == appwindow->m_IdName) {
              SetCurrentRenderedAppWindow(appwindow);
              appwindow->CtxRender(&m_RedockRequests, window->GetName());
              context_loaded = true;
            }
          }
        }

        if (appwindow->CheckWinParent(window->GetName()) && !appwindow->m_HaveParentAppWindow) {
          SetCurrentRenderedAppWindow(appwindow);
          appwindow->CtxRender(&m_RedockRequests, window->GetName());
          context_loaded = true;
        }
      }
    }
  }

  void Application::PrepareViewport(Window *window) {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
  }

  ImDrawData *Application::RenderWindow(Window *window) {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    PrepareViewport(window);

    float minWidth = window->GetSpecifications().MinWidth;
    float minHeight = window->GetSpecifications().MinHeight;
    ImGui::SetNextWindowSizeConstraints(ImVec2(minWidth, minHeight), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (Application::GetCurrentRenderedWindow()) {
      if (!Application::GetCurrentRenderedWindow()->GetSpecifications().WindowResizeable) {
        window_flags |= ImGuiWindowFlags_NoResize;
      }
    }

    if (!window->GetSpecifications().CustomTitlebar && m_MenubarCallback)
      window_flags |= ImGuiWindowFlags_MenuBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

    std::string label = "DockSpaceWindow." + window->GetName();
    ImGui::SetNextWindowDockID(0);
    ImGui::Begin(label.c_str(), nullptr, window_flags);
    window->GetImGuiWindow() = ImGui::GetCurrentWindow();

    ImVec2 newSize = ImGui::GetWindowSize();

    SDL_Window *sdlWindow = window->GetWindowHandle();
    int sdlWidth, sdlHeight;
    SDL_GetWindowSize(sdlWindow, &sdlWidth, &sdlHeight);

    if (newSize.x < minWidth)
      newSize.x = minWidth;
    if (newSize.y < minHeight)
      newSize.y = minHeight;

    ImVec2 deltaSize = ImVec2(newSize.x - ImGui::GetWindowSize().x, newSize.y - ImGui::GetWindowSize().y);

    ImVec2 windowPos = ImGui::GetWindowPos();

    window->SetResizing(false);
    if (newSize.x != sdlWidth || newSize.y != sdlHeight) {
      window->SetResizing(true);

      if (ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + sdlWidth, windowPos.y))) {
        windowPos.y -= deltaSize.y;
      }

      if (ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x, windowPos.y + sdlHeight))) {
        windowPos.x -= deltaSize.x;
      }

      SDL_SetWindowSize(sdlWindow, static_cast<int>(newSize.x), static_cast<int>(newSize.y));
      SDL_SetWindowPosition(sdlWindow, static_cast<int>(windowPos.x), static_cast<int>(windowPos.y));
    }

    ImGuiWindow *win = ImGui::GetCurrentWindow();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);

    if (window->GetSpecifications().CustomTitlebar && !window->GetSpecifications().DisableTitleBar) {
      float titleBarHeight;
      window->UI_DrawTitlebar(titleBarHeight);
      ImGui::SetCursorPosY(titleBarHeight);
    }

    // Render notifications
    ImGui::RenderNotifications();

    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle &style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;

    AppPushTabStyle();

    switch (window->GetSpecifications().RenderMode) {
      case WindowRenderingMethod::DockingWindows: {
        HandleDockingModeRendering(window);
        break;
      }
      case (WindowRenderingMethod::TabWidows): {
        HandleTabsModeRendering(window);
        break;
      }
      case (WindowRenderingMethod::SimpleWindow): {
        HandleSimpleWindowRendering(window);
        break;
      }
      case (WindowRenderingMethod::SimpleRender): {
        HandleSimpleRendering(window);
        break;
      }
    }

    AppPopTabStyle();

    style.WindowMinSize.x = minWinSizeX;

// Devtools
#ifdef CHERRY_DEBUG
    static bool s_DevtoolsLock = false;
    bool isF12Down = IsKeyPressed(CherryKey::F12);

    if (isF12Down) {
      if (!s_DevtoolsLock) {
        ToggleDebugTools();
        s_DevtoolsLock = true;
      }
    } else {
      s_DevtoolsLock = false;
    }
#endif

// Debug overlay
#ifdef CHERRY_DEBUG
    static float ms_values[90] = { 0 };
    static int frame_offset = 0;

    if (Application::DebugToolState().load() && window->GetName() != "Cherry devtools") {
      const float PAD = 10.0f;
      const ImGuiViewport *viewport = ImGui::GetMainViewport();

      ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + PAD, viewport->WorkPos.y + PAD), ImGuiCond_Always);
      ImGui::SetNextWindowSize(ImVec2(250, 0));
      ImGui::SetNextWindowBgAlpha(0.92f);

      ImGuiWindowFlags overlay_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                       ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                       ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs;

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));

      if (ImGui::Begin("##DebugOverlay", nullptr, overlay_flags)) {
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "CHERRY");
        ImGui::SameLine();
        ImGui::TextDisabled("| %s", CHERRY_VERSION);
        ImGui::Separator();
        ImGui::Spacing();

        float fps = ImGui::GetIO().Framerate;
        float ms = 1000.0f / fps;
        ms_values[frame_offset] = ms;
        frame_offset = (frame_offset + 1) % IM_ARRAYSIZE(ms_values);

        ImVec4 perf_color_vec = (fps > 58.0f) ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f) : ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
        ImU32 perf_color = ImGui::ColorConvertFloat4ToU32(perf_color_vec);

        auto DebugLine = [](const char *label, const char *value, ImVec4 val_color = ImVec4(1, 1, 1, 1)) {
          ImGui::TextDisabled("%s", label);
          ImGui::SameLine(100);
          ImGui::TextColored(val_color, "%s", value);
        };

        DebugLine("FPS", std::to_string((int)fps).c_str(), perf_color_vec);
        DebugLine("Frame", (std::to_string(ms).substr(0, 4) + " ms").c_str());

        ImGui::Spacing();
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImVec2(230, 45);

        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(
            canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(20, 20, 20, 255));

        static ImVec2 points[90];
        float max_v = 33.0f;

        for (int i = 0; i < 90; i++) {
          int idx = (frame_offset + i) % 90;
          float val = ImClamp(ms_values[idx], 0.0f, max_v);
          points[i].x = canvas_pos.x + ((float)i / 89.0f) * canvas_size.x;
          points[i].y = canvas_pos.y + (1.0f - (val / max_v)) * canvas_size.y;
        }

        for (int i = 0; i < 89; i++) {
          ImVec2 p1 = points[i];
          ImVec2 p2 = points[i + 1];
          ImVec2 b1 = ImVec2(p1.x, canvas_pos.y + canvas_size.y);
          ImVec2 b2 = ImVec2(p2.x, canvas_pos.y + canvas_size.y);

          draw_list->AddRectFilledMultiColor(
              p1,
              b2,
              perf_color & 0x00FFFFFF | 0x80000000,
              perf_color & 0x00FFFFFF | 0x80000000,
              perf_color & 0x00FFFFFF | 0x00000000,
              perf_color & 0x00FFFFFF | 0x00000000);
        }

        for (int i = 0; i < 89; i++) {
          draw_list->AddLine(points[i], points[i + 1], perf_color, 1.5f);
        }

        ImGui::Dummy(canvas_size);
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("SYSTEM", ImGuiTreeNodeFlags_DefaultOpen)) {
          DebugLine("Renderer", ImGui::GetIO().BackendRendererName ? ImGui::GetIO().BackendRendererName : "N/A");
          DebugLine("DrawCalls", std::to_string(ImGui::GetDrawData() ? ImGui::GetDrawData()->CmdListsCount : 0).c_str());
          DebugLine("CPU Cores", (std::to_string(SDL_GetCPUCount()) + " threads").c_str());
        }

        if (ImGui::CollapsingHeader("DISPLAY", ImGuiTreeNodeFlags_DefaultOpen)) {
          int d_idx = SDL_GetWindowDisplayIndex(window->GetWindowHandle());
          SDL_DisplayMode d_mode;
          SDL_GetCurrentDisplayMode(d_idx, &d_mode);

          DebugLine("Monitor", (std::string("#") + std::to_string(d_idx)).c_str());
          DebugLine(
              "Screen size",
              (std::to_string(ImGui::GetWindowSize().x) + "x" + std::to_string(ImGui::GetWindowSize().y)).c_str());
          DebugLine("Resolution", (std::to_string(d_mode.w) + "x" + std::to_string(d_mode.h)).c_str());
          DebugLine("Refresh", (std::to_string(d_mode.refresh_rate) + " Hz").c_str());
          DebugLine("DPI Scale", (std::to_string(viewport->DpiScale).substr(0, 3) + "x").c_str());
        }

        if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen)) {
          int d_idx = SDL_GetWindowDisplayIndex(window->GetWindowHandle());
          SDL_DisplayMode d_mode;
          SDL_GetCurrentDisplayMode(d_idx, &d_mode);
          DebugLine(
              "Anonymous",
              std::to_string(Cherry::Application::Get().GetComponentPool()->AnonymousComponents.size()).c_str());
          DebugLine(
              "Identified",
              std::to_string(Cherry::Application::Get().GetComponentPool()->IdentifiedComponents.size()).c_str());
          DebugLine("Pool stack size", std::to_string(Cherry::Application::Get().GetComponentPoolStack().size()).c_str());
        }

        if (ImGui::IsMousePosValid()) {
          ImGui::Separator();
          ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
          ImGui::TextDisabled("MOUSE: ");
          ImGui::SameLine();
          ImGui::Text("%.0f, %.0f", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
          ImGui::PopStyleVar();
        }
      }
      ImGui::End();
      ImGui::PopStyleVar(3);
    }
#endif

    ImGui::End();

    return window->GetDrawData();
  }

}  // namespace Cherry