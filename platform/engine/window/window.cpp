#include "window.hpp"

#include "../embed/not_found_img.embed"
#include "../embed/window.embed"

// Emedded font
#include "../imgui/Hack-Regular.embed"
#include "../imgui/Inconsolatas.embed"
#include "../imgui/Roboto-Bold.embed"
#include "../imgui/Roboto-Italic.embed"
#include "../imgui/Roboto-Regular.embed"

namespace ImGui {
// Docking
static const float DOCKING_TRANSPARENT_PAYLOAD_ALPHA =
    0.50f; // For use with io.ConfigDockingTransparentPayload. Apply to Viewport
           // _or_ WindowBg in host viewport.
static const float DOCKING_SPLITTER_SIZE = 2.0f;

void DockNodeCalcTabBarLayout(const ImGuiDockNode *node, ImRect *out_title_rect,
                              ImRect *out_tab_bar_rect,
                              ImVec2 *out_window_menu_button_pos,
                              ImVec2 *out_close_button_pos) {
  ImGuiContext &g = *GImGui;
  ImGuiStyle &style = g.Style;

  ImRect r = ImRect(node->Pos.x, node->Pos.y, node->Pos.x + node->Size.x,
                    node->Pos.y + g.FontSize + g.Style.FramePadding.y * 2.0f);
  if (out_title_rect) {
    *out_title_rect = r;
  }

  r.Min.x += style.WindowBorderSize;
  r.Max.x -= style.WindowBorderSize;

  float button_sz = g.FontSize;

  ImVec2 window_menu_button_pos = r.Min;
  r.Min.x += style.FramePadding.x;
  r.Max.x -= style.FramePadding.x;
  if (node->HasCloseButton) {
    r.Max.x -= button_sz;
    if (out_close_button_pos)
      *out_close_button_pos = ImVec2(r.Max.x - style.FramePadding.x, r.Min.y);
  }
  if (node->HasWindowMenuButton &&
      style.WindowMenuButtonPosition == ImGuiDir_Left) {
    r.Min.x += button_sz + style.ItemInnerSpacing.x;
  } else if (node->HasWindowMenuButton &&
             style.WindowMenuButtonPosition == ImGuiDir_Right) {
    r.Max.x -= button_sz + style.FramePadding.x;
    window_menu_button_pos = ImVec2(r.Max.x, r.Min.y);
  }
  if (out_tab_bar_rect) {
    *out_tab_bar_rect = r;
  }
  if (out_window_menu_button_pos) {
    *out_window_menu_button_pos = window_menu_button_pos;
  }
}

bool DockNodeIsDropAllowedOne(ImGuiWindow *payload, ImGuiWindow *host_window) {
  if (host_window->DockNodeAsHost &&
      host_window->DockNodeAsHost->IsDockSpace() &&
      payload->BeginOrderWithinContext < host_window->BeginOrderWithinContext)
    return false;

  ImGuiWindowClass *host_class = host_window->DockNodeAsHost
                                     ? &host_window->DockNodeAsHost->WindowClass
                                     : &host_window->WindowClass;
  ImGuiWindowClass *payload_class = &payload->WindowClass;
  if (host_class->ClassId != payload_class->ClassId) {
    if (host_class->ClassId != 0 && host_class->DockingAllowUnclassed &&
        payload_class->ClassId == 0)
      return true;
    if (payload_class->ClassId != 0 && payload_class->DockingAllowUnclassed &&
        host_class->ClassId == 0)
      return true;
    return false;
  }

  // Prevent docking any window created above a popup
  // Technically we should support it (e.g. in the case of a long-lived modal
  // window that had fancy docking features), by e.g. adding a 'if
  // (!ImGui::IsWindowWithinBeginStackOf(host_window, popup_window))' test. But
  // it would requires more work on our end because the dock host windows is
  // technically created in NewFrame() and our ->ParentXXX and ->RootXXX
  // pointers inside windows are currently mislading or lacking.
  ImGuiContext &g = *GImGui;
  for (int i = g.OpenPopupStack.Size - 1; i >= 0; i--)
    if (ImGuiWindow *popup_window = g.OpenPopupStack[i].Window)
      if (ImGui::IsWindowWithinBeginStackOf(
              payload, popup_window)) // Payload is created from within a popup
                                      // begin stack.
        return false;

  return true;
}

void DockNodePreviewDockR(ImGuiWindow *host_window, ImGuiDockNode *host_node,
                          ImGuiWindow *root_payload,
                          const ImGuiDockPreviewData *data) {
  ImGuiContext &g = *GImGui;
  IM_ASSERT(g.CurrentWindow ==
            host_window); // Because we rely on font size to calculate tab sizes

  // With this option, we only display the preview on the target viewport, and
  // the payload viewport is made transparent. To compensate for the single
  // layer obstructed by the payload, we'll increase the alpha of the preview
  // nodes.
  const bool is_transparent_payload = g.IO.ConfigDockingTransparentPayload;

  // In case the two windows involved are on different viewports, we will draw
  // the overlay on each of them.
  int overlay_draw_lists_count = 0;
  ImDrawList *overlay_draw_lists[2];
  overlay_draw_lists[overlay_draw_lists_count++] =
      GetForegroundDrawList(host_window->Viewport);
  if (host_window->Viewport != root_payload->Viewport &&
      !is_transparent_payload)
    overlay_draw_lists[overlay_draw_lists_count++] =
        GetForegroundDrawList(root_payload->Viewport);

  // Draw main preview rectangle
  const ImU32 overlay_col_main = GetColorU32(
      ImGuiCol_DockingPreview, is_transparent_payload ? 0.60f : 0.40f);
  const ImU32 overlay_col_drop = GetColorU32(
      ImGuiCol_DockingPreview, is_transparent_payload ? 0.90f : 0.70f);
  const ImU32 overlay_col_drop_hovered = GetColorU32(
      ImGuiCol_DockingPreview, is_transparent_payload ? 1.20f : 1.00f);
  const ImU32 overlay_col_lines = GetColorU32(
      ImGuiCol_NavWindowingHighlight, is_transparent_payload ? 0.80f : 0.60f);

  // Display area preview
  const bool can_preview_tabs =
      (root_payload->DockNodeAsHost == NULL ||
       root_payload->DockNodeAsHost->Windows.Size > 0);
  if (data->IsDropAllowed) {
    ImRect overlay_rect = data->FutureNode.Rect();
    if (data->SplitDir == ImGuiDir_None && can_preview_tabs)
      overlay_rect.Min.y += GetFrameHeight();
    if (data->SplitDir != ImGuiDir_None || data->IsCenterAvailable)
      for (int overlay_n = 0; overlay_n < overlay_draw_lists_count; overlay_n++)
        overlay_draw_lists[overlay_n]->AddRectFilled(
            overlay_rect.Min, overlay_rect.Max, overlay_col_main,
            host_window->WindowRounding,
            CalcRoundingFlagsForRectInRect(overlay_rect, host_window->Rect(),
                                           DOCKING_SPLITTER_SIZE));
  }

  // Display tab shape/label preview unless we are splitting node (it generally
  // makes the situation harder to read)
  if (data->IsDropAllowed && can_preview_tabs &&
      data->SplitDir == ImGuiDir_None && data->IsCenterAvailable) {
    // Compute target tab bar geometry so we can locate our preview tabs
    ImRect tab_bar_rect;
    DockNodeCalcTabBarLayout(&data->FutureNode, NULL, &tab_bar_rect, NULL,
                             NULL);
    ImVec2 tab_pos = tab_bar_rect.Min;
    if (host_node && host_node->TabBar) {
      if (!host_node->IsHiddenTabBar() && !host_node->IsNoTabBar())
        tab_pos.x += host_node->TabBar->WidthAllTabs +
                     g.Style.ItemInnerSpacing
                         .x; // We don't use OffsetNewTab because when using
                             // non-persistent-order tab bar it is incremented
                             // with each Tab submission.
      else
        tab_pos.x += g.Style.ItemInnerSpacing.x +
                     TabItemCalcSize(host_node->Windows[0]->Name,
                                     host_node->Windows[0]->HasCloseButton)
                         .x;
    } else if (!(host_window->Flags & ImGuiWindowFlags_DockNodeHost)) {
      tab_pos.x +=
          g.Style.ItemInnerSpacing.x +
          TabItemCalcSize(host_window->Name, host_window->HasCloseButton)
              .x; // Account for slight offset which will be added when changing
                  // from title bar to tab bar
    }

    // Draw tab shape/label preview (payload may be a loose window or a host
    // window carrying multiple tabbed windows)
    if (root_payload->DockNodeAsHost)
      IM_ASSERT(root_payload->DockNodeAsHost->Windows.Size <=
                root_payload->DockNodeAsHost->TabBar->Tabs.Size);
    ImGuiTabBar *tab_bar_with_payload =
        root_payload->DockNodeAsHost ? root_payload->DockNodeAsHost->TabBar
                                     : NULL;
    const int payload_count =
        tab_bar_with_payload ? tab_bar_with_payload->Tabs.Size : 1;
    for (int payload_n = 0; payload_n < payload_count; payload_n++) {
      // DockNode's TabBar may have non-window Tabs manually appended by user
      ImGuiWindow *payload_window =
          tab_bar_with_payload ? tab_bar_with_payload->Tabs[payload_n].Window
                               : root_payload;
      if (tab_bar_with_payload && payload_window == NULL)
        continue;
      if (!DockNodeIsDropAllowedOne(payload_window, host_window))
        continue;

      // Calculate the tab bounding box for each payload window
      ImVec2 tab_size =
          TabItemCalcSize(payload_window->Name, payload_window->HasCloseButton);
      ImRect tab_bb(tab_pos.x, tab_pos.y, tab_pos.x + tab_size.x,
                    tab_pos.y + tab_size.y);
      tab_pos.x += tab_size.x + g.Style.ItemInnerSpacing.x;
      const ImU32 overlay_col_text = GetColorU32(
          payload_window->DockStyle.Colors[ImGuiWindowDockStyleCol_Text]);
      const ImU32 overlay_col_tabs = GetColorU32(
          payload_window->DockStyle.Colors[ImGuiWindowDockStyleCol_TabActive]);

      PushStyleColor(ImGuiCol_Text, overlay_col_text);
      for (int overlay_n = 0; overlay_n < overlay_draw_lists_count;
           overlay_n++) {
        ImGuiTabItemFlags tab_flags =
            ImGuiTabItemFlags_Preview |
            ((payload_window->Flags & ImGuiWindowFlags_UnsavedDocument)
                 ? ImGuiTabItemFlags_UnsavedDocument
                 : 0);
        if (!tab_bar_rect.Contains(tab_bb))
          overlay_draw_lists[overlay_n]->PushClipRect(tab_bar_rect.Min,
                                                      tab_bar_rect.Max);
        TabItemBackground(overlay_draw_lists[overlay_n], tab_bb, tab_flags,
                          overlay_col_tabs);
        TabItemLabelAndCloseButton(overlay_draw_lists[overlay_n], tab_bb,
                                   tab_flags, g.Style.FramePadding,
                                   payload_window->Name, 0, 0, false, NULL,
                                   NULL);
        if (!tab_bar_rect.Contains(tab_bb))
          overlay_draw_lists[overlay_n]->PopClipRect();
      }
      PopStyleColor();
    }
  }

  const float overlay_rounding = ImMax(3.0f, g.Style.FrameRounding);
  for (int dir = ImGuiDir_None; dir < ImGuiDir_COUNT; dir++) {
    ImRect draw_r = data->DropRectsDraw[dir];
    if (!draw_r.IsInverted()) {
      ImRect draw_r_in = draw_r;
      draw_r_in.Expand(-2.0f);
      ImU32 overlay_col =
          (data->SplitDir == (ImGuiDir)dir && data->IsSplitDirExplicit)
              ? overlay_col_drop_hovered
              : overlay_col_drop;
      for (int overlay_n = 0; overlay_n < overlay_draw_lists_count;
           overlay_n++) {
        ImVec2 center = ImFloor(draw_r_in.GetCenter());
        overlay_draw_lists[overlay_n]->AddRectFilled(
            draw_r.Min, draw_r.Max, overlay_col, overlay_rounding);
        overlay_draw_lists[overlay_n]->AddRect(
            draw_r_in.Min, draw_r_in.Max, overlay_col_lines, overlay_rounding);

        if (dir == ImGuiDir_Left || dir == ImGuiDir_Right)
          overlay_draw_lists[overlay_n]->AddLine(
              ImVec2(center.x, draw_r_in.Min.y),
              ImVec2(center.x, draw_r_in.Max.y), overlay_col_lines);
        if (dir == ImGuiDir_Up || dir == ImGuiDir_Down)
          overlay_draw_lists[overlay_n]->AddLine(
              ImVec2(draw_r_in.Min.x, center.y),
              ImVec2(draw_r_in.Max.x, center.y), overlay_col_lines);
      }
    }

    if (dir == ImGuiDir_None && (host_node && (host_node->MergedFlags &
                                               ImGuiDockNodeFlags_NoSplit)) ||
        g.IO.ConfigDockingNoSplit)
      break;
  }
}

} // namespace ImGui

namespace Cherry {

Window::Window() : m_Name("empty") {}

Window::Window(const std::string &name, int width, int height,
               ApplicationSpecification specs, bool cold_start)
    : m_Name(name), m_Width(specs.Width), m_Height(specs.Height) {

  m_Specifications = specs;
  SDL_WindowFlags window_flags;

#ifdef CHERRY_ENABLE_LINUXDRM
  window_flags = SDL_WindowFlags(SDL_WINDOW_VULKAN | SDL_WINDOW_FULLSCREEN);

  if (SDL_getenv("SDL_VIDEODRIVER") == nullptr) {
    SDL_setenv("SDL_VIDEODRIVER", "KMSDRM", 1);
  }

#else
  if (m_Specifications.DisableWindowManagerTitleBar) {
    window_flags =
        SDL_WindowFlags(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS);
  } else {
    window_flags = SDL_WindowFlags(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                                   SDL_WINDOW_ALLOW_HIGHDPI);
  }
#endif

  m_WindowHandler = SDL_CreateWindow(m_Name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, m_Width, m_Height,
                                     window_flags);

#ifdef CHERRY_ENABLE_LINUXDRM
  SDL_DisplayMode mode;
  if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
    m_Width = mode.w;
    m_Height = mode.h;
  } else {
    SDL_GetWindowSize(m_WindowHandler, &m_Width, &m_Height);
  }
#else
  SDL_GetWindowSize(m_WindowHandler, &m_Width, &m_Height);
#endif

  // Setup Vulkan
  uint32_t extensions_count = 0;
  SDL_Vulkan_GetInstanceExtensions(m_WindowHandler, &extensions_count, nullptr);
  const char **extensions = new const char *[extensions_count];
  SDL_Vulkan_GetInstanceExtensions(m_WindowHandler, &extensions_count,
                                   extensions);
  Application::SetupVulkan(extensions, extensions_count);
  delete[] extensions;

  VkResult err;

#ifdef CHERRY_ENABLE_LINUXDRM
  if (!SDL_Vulkan_CreateSurface(m_WindowHandler, Application::GetInstance(),
                                &m_Surface)) {
    printf("Warning: SDL Vulkan surface creation failed on DRM. "
           "You may need to use VK_KHR_display directly.\n");
    m_Surface = VK_NULL_HANDLE;
  }
#else
  if (SDL_Vulkan_CreateSurface(m_WindowHandler, Application::GetInstance(),
                               &m_Surface) == 0) {
    printf("Failed to create Vulkan surface.\n");
    return;
  }
#endif

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  m_ImGuiContext = ImGui::CreateContext();
  ImGui::SetCurrentContext(m_ImGuiContext);

  // Create Framebuffers
  int w, h;
  SDL_GetWindowSize(m_WindowHandler, &w, &h);
  ImGui_ImplVulkanH_Window *wd = &this->m_WinData;
  Application::SetupVulkanWindow(wd, m_Surface, w, h, this);
  this->s_AllocatedCommandBuffers.resize(wd->ImageCount);
  s_ResourceFreeQueue.resize(wd->ImageCount);

  // Create the ImGui context
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.IniFilename = NULL;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;           // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows
  io.FontGlobalScale = specs.FontGlobalScale;

  // Setup Dear ImGui style
  UI::SetTheme();
  // ImGui::StyleColorsClassic();
  // Style
  ImGuiStyle &style = ImGui::GetStyle();
  style.WindowPadding = ImVec2(10.0f, 10.0f);
  style.FramePadding = ImVec2(8.0f, 6.0f);
  style.ItemSpacing = ImVec2(6.0f, 6.0f);
  style.ChildRounding = 6.0f;
  style.PopupRounding = 6.0f;
  style.FrameRounding = 6.0f;
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  // Load default font
  ImFontConfig fontConfig;
  fontConfig.FontDataOwnedByAtlas = false;
  ImFont *robotoFont = io.Fonts->AddFontFromMemoryTTF(
      (void *)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
  ImFont *hackFont = io.Fonts->AddFontFromMemoryTTF(
      (void *)g_HackRegular, sizeof(g_HackRegular), 20.0f, &fontConfig);

  ImFontConfig fontConfigExtra;
  fontConfigExtra.FontDataOwnedByAtlas = false;
  fontConfigExtra.GlyphExtraSpacing.x = 1.3f;
  ImFont *inconsolatas = io.Fonts->AddFontFromMemoryTTF(
      (void *)g_Inconsolatas, sizeof(g_Inconsolatas), 20.0f, &fontConfigExtra);

  Application::GetFontList()["Default"] = hackFont;
  Application::GetFontList()["Inconsolatas"] = inconsolatas;
  Application::GetFontList()["Bold"] = io.Fonts->AddFontFromMemoryTTF(
      (void *)g_RobotoBold, sizeof(g_RobotoBold), 20.0f, &fontConfig);
  Application::GetFontList()["Italic"] = io.Fonts->AddFontFromMemoryTTF(
      (void *)g_RobotoItalic, sizeof(g_RobotoItalic), 20.0f, &fontConfig);
  Application::GetFontList()["HackRegular"] = io.Fonts->AddFontFromMemoryTTF(
      (void *)g_HackRegular, sizeof(g_HackRegular), 20.0f, &fontConfig);

  for (auto &font : Application::Get().GetCustomFonts()) {
    Application::GetFontList()[font.first] = io.Fonts->AddFontFromFileTTF(
        font.second.first.c_str(), font.second.second, &fontConfig);
  }

  io.FontDefault = hackFont;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForVulkan(m_WindowHandler);
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = Application::GetInstance();
  init_info.PhysicalDevice = Application::GetPhysicalDevice();
  init_info.Device = Application::GetDevice();
  init_info.QueueFamily = Application::GetQueueFamily();
  init_info.Queue = Application::GetVkQueue();
  init_info.PipelineCache = Application::GetVkPipelineCache();
  init_info.DescriptorPool = Application::GetVkDescriptorPool();
  init_info.Subpass = 0;
  init_info.MinImageCount = Application::GetMinImageCount();
  init_info.ImageCount = wd->ImageCount;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = &Application::GetVkAllocationCallbacks();
  init_info.CheckVkResultFn = check_vk_result;
  ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

  {
    // Use any command queue
    VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
    VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

    err = vkResetCommandPool(Application::GetDevice(), command_pool, 0);
    check_vk_result(err);
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(command_buffer, &begin_info);
    check_vk_result(err);

    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &command_buffer;
    err = vkEndCommandBuffer(command_buffer);
    check_vk_result(err);
    err =
        vkQueueSubmit(Application::GetVkQueue(), 1, &end_info, VK_NULL_HANDLE);
    check_vk_result(err);

    err = vkDeviceWaitIdle(Application::GetDevice());
    check_vk_result(err);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
  }

  Application::IncrementWindowNumber();
  Application::SetMasterSwapChainRebuild(true);

  drag_dropstate = std::make_shared<WindowDragDropState>();
}

Window::~Window() {
  if (!m_IsClosing) {
    if (m_Surface != VK_NULL_HANDLE) {
      vkDestroySurfaceKHR(Application::GetInstance(), m_Surface, nullptr);
      m_Surface = VK_NULL_HANDLE;
    }

    if (Application::GetDevice() != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(Application::GetDevice());
    }

    if (m_WindowHandler) {
      SDL_DestroyWindow(m_WindowHandler);
      m_WindowHandler = nullptr;
    }

    for (auto &frameQueue : s_ResourceFreeQueue) {
      for (auto &func : frameQueue) {
        func();
      }
    }
    s_ResourceFreeQueue.clear();

    m_CommandBuffers.clear();
  }
}

void Window::ShowDockingPreview(
    ImGuiID dockspaceID, Window *win,
    const std::shared_ptr<Cherry::WindowDragDropState> &dragState,
    const std::shared_ptr<AppWindow> appwin) {
  ImGuiContext *ctx = ImGui::GetCurrentContext();
  if (ctx == nullptr)
    return;

  ImGuiDockNode *dock_node = ImGui::DockBuilderGetNode(dockspaceID);

  if (dock_node == nullptr || !dock_node->IsVisible) {
    return;
  }

  int winX, winY, winW, winH;
  SDL_GetWindowPosition(win->GetWindowHandle(), &winX, &winY);
  SDL_GetWindowSize(win->GetWindowHandle(), &winW, &winH);
  int mouseX = dragState->mouseX;
  int mouseY = dragState->mouseY;
  if (mouseX < winX || mouseX > winX + winW || mouseY < winY ||
      mouseY > winY + winH) {
    return;
  }

  // TODO If future "AllowAllRedocks", disable the following safety feature.
  if (appwin) {
    if (dragState->LastDraggingAppWindowHost == appwin->m_Name) {
      // return;
    }
  }

  auto ShowDropZones = [&](ImGuiDockNode *node) {
    if (node == nullptr || !node->IsVisible)
      return;

    ImGuiDockPreviewData preview_data;
    preview_data.IsDropAllowed = true;
    preview_data.IsCenterAvailable = true;
    preview_data.IsSidesAvailable = true;

    ImRect dock_rect = node->Rect();
    ImVec2 c = ImFloor(ImVec2((dock_rect.Min.x + dock_rect.Max.x) * 0.5f,
                              (dock_rect.Min.y + dock_rect.Max.y) * 0.5f));

    float hs_w = 30.0f;
    float hs_h = 20.0f;
    ImVec2 off(hs_w + hs_h, hs_w + hs_h);

    preview_data.DropRectsDraw[ImGuiDir_None] =
        ImRect(c.x - hs_w, c.y - hs_w, c.x + hs_w, c.y + hs_w);
    preview_data.DropRectsDraw[ImGuiDir_Up] =
        ImRect(c.x - hs_w, c.y - off.y - hs_h, c.x + hs_w, c.y - off.y + hs_h);
    preview_data.DropRectsDraw[ImGuiDir_Down] =
        ImRect(c.x - hs_w, c.y + off.y - hs_h, c.x + hs_w, c.y + off.y + hs_h);
    preview_data.DropRectsDraw[ImGuiDir_Left] =
        ImRect(c.x - off.x - hs_h, c.y - hs_w, c.x - off.x + hs_h, c.y + hs_w);
    preview_data.DropRectsDraw[ImGuiDir_Right] =
        ImRect(c.x + off.x - hs_h, c.y - hs_w, c.x + off.x + hs_h, c.y + hs_w);

    ImGui::DockNodePreviewDockR(ImGui::GetCurrentWindow(), node,
                                ImGui::GetCurrentWindow(), &preview_data);

    ImDrawList *draw_list = ImGui::GetForegroundDrawList();

    for (int dir = ImGuiDir_None; dir < ImGuiDir_COUNT; dir++) {
      ImRect drop_rect = preview_data.DropRectsDraw[dir];

      if (drop_rect.Contains(
              ImVec2(Application::GetCurrentDragDropState()->mouseX,
                     Application::GetCurrentDragDropState()->mouseY)) &&
          preview_data.IsDropAllowed) {
        Application::SetValidDropZoneFounded(true);

        if (node->Windows.Size > 0) {
          Application::SetCurrentDragDropStateAppWindow(node->Windows[0]->Name);
          Application::SetCurrentDragDropStateWindow(win->GetName());
        } else {
          Application::SetCurrentDragDropStateAppWindow("none");
          Application::SetCurrentDragDropStateWindow(win->GetName());
        }

        ImRect preview_rect;
        if (dir == ImGuiDir_None) {
          Application::SetCurrentDragDropStateDraggingPlace(
              Cherry::DockEmplacement::DockFull);
          preview_rect = dock_rect;
        } else {
          switch (dir) {
          case ImGuiDir_Left:
            Application::SetCurrentDragDropStateDraggingPlace(
                Cherry::DockEmplacement::DockLeft);
            preview_rect =
                ImRect(dock_rect.Min.x, dock_rect.Min.y, c.x, dock_rect.Max.y);
            break;
          case ImGuiDir_Up:
            Application::SetCurrentDragDropStateDraggingPlace(
                Cherry::DockEmplacement::DockUp);
            preview_rect =
                ImRect(dock_rect.Min.x, dock_rect.Min.y, dock_rect.Max.x, c.y);
            break;
          case ImGuiDir_Right:
            Application::SetCurrentDragDropStateDraggingPlace(
                Cherry::DockEmplacement::DockRight);
            preview_rect =
                ImRect(c.x, dock_rect.Min.y, dock_rect.Max.x, dock_rect.Max.y);
            break;
          case ImGuiDir_Down:
            Application::SetCurrentDragDropStateDraggingPlace(
                Cherry::DockEmplacement::DockDown);
            preview_rect =
                ImRect(dock_rect.Min.x, c.y, dock_rect.Max.x, dock_rect.Max.y);
            break;
          default:
            Application::SetCurrentDragDropStateDraggingPlace(
                Cherry::DockEmplacement::DockBlank);
            break;
          }
        }

        draw_list->AddRectFilled(preview_rect.Min, preview_rect.Max,
                                 IM_COL32(177, 255, 49, 128));
        draw_list->AddRect(preview_rect.Min, preview_rect.Max,
                           IM_COL32(177, 255, 49, 255));

        break;
      }
    }

    if (!Application::GetValidDropZoneFounded()) {
      Application::SetCurrentDragDropStateDraggingPlace(
          Cherry::DockEmplacement::DockBlank);
      Application::SetCurrentDragDropStateWindow(win->GetName());
    }
  };

  std::function<void(ImGuiDockNode *)> TraverseAndShowDropZones;
  TraverseAndShowDropZones = [&](ImGuiDockNode *node) {
    if (node == nullptr)
      return;

    if (node->ChildNodes[0] || node->ChildNodes[1]) {
      if (node->ChildNodes[0])
        TraverseAndShowDropZones(node->ChildNodes[0]);
      if (node->ChildNodes[1])
        TraverseAndShowDropZones(node->ChildNodes[1]);
    } else {
      ShowDropZones(node);
    }
  };

  TraverseAndShowDropZones(dock_node);
}

void Window::UI_DrawTitlebar(float &outTitlebarHeight) {
  ImGui::SetCurrentContext(this->m_ImGuiContext);

  float titlebarVerticalOffset = 0.0f;
  const float titlebarHeight = 58.0f;
  const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

  ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
  ImVec2 titlebarMax = {titlebarMin.x + ImGui::GetWindowWidth() -
                            windowPadding.y * 2.0f,
                        titlebarMin.y + titlebarHeight};

  if (ImGui::IsMouseHoveringRect(titlebarMin, titlebarMax) &&
      ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    this->isMoving = true;
    ImVec2 mousePos = ImGui::GetMousePos();
    this->clickOffset =
        ImVec2(mousePos.x - titlebarMin.x, mousePos.y - titlebarMin.y);
  }

  if (this->isMoving && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    if (!m_Resizing) {
      ImVec2 mousePos = ImGui::GetMousePos();
      SDL_Window *sdlWindow = this->GetWindowHandle();
      SDL_SetWindowPosition(sdlWindow,
                            static_cast<int>(mousePos.x - this->clickOffset.x),
                            static_cast<int>(mousePos.y - this->clickOffset.y));
    }
  }

  if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    this->isMoving = false;
  }

  auto *fgDrawList = ImGui::GetForegroundDrawList();
  auto *bgDrawList = ImGui::GetBackgroundDrawList();

  bgDrawList->AddRectFilled(titlebarMin, titlebarMax,
                            UI::Colors::Theme::titlebar);
  // DEBUG TITLEBAR BOUNDS
  // fgDrawList->AddRect(titlebarMin, titlebarMax,
  // UI::Colors::Theme::invalidPrefab); Logo
  if (!m_Specifications.DisableLogo && !m_Specifications.IconPath.empty()) {
    const int logoWidth = 48;  // Largeur du logo
    const int logoHeight = 48; // Hauteur du logo
    const ImVec2 logoOffset(16.0f + windowPadding.x,
                            5.0f + windowPadding.y + titlebarVerticalOffset);
    const ImVec2 logoRectStart = {titlebarMin.x + logoOffset.x,
                                  titlebarMin.y + logoOffset.y};
    const ImVec2 logoRectMax = {logoRectStart.x + logoWidth,
                                logoRectStart.y + logoHeight};

    ImTextureID logo = this->get_texture(this->m_Specifications.IconPath);
    fgDrawList->AddImage(logo, logoRectStart, logoRectMax);
  }

  ImGui::SetItemAllowOverlap();
  ImGui::BeginHorizontal("Titlebar",
                         {ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
                          ImGui::GetFrameHeightWithSpacing()});

  const float w = ImGui::GetContentRegionAvail().x;
  const float buttonsAreaWidth = 94;

  // Draw Menubar
  if (Application::Get().m_MenubarCallback) {
    ImGui::SuspendLayout();
    {
      ImGui::SetItemAllowOverlap();
      const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
      ImGui::SetCursorPos(
          ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
      UI_DrawMenubar();
    }

    ImGui::ResumeLayout();
  }

  if (!m_Specifications.DisableTitle) {
    // Centered Window title
    ImVec2 currentCursorPos = ImGui::GetCursorPos();
    ImVec2 textSize = ImGui::CalcTextSize(m_Specifications.Name.c_str());
    ImGui::SetCursorPos(
        ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f,
               2.0f + windowPadding.y + 6.0f));
    ImGui::Text("%s", m_Specifications.Name.c_str()); // Draw title
    ImGui::SetCursorPos(currentCursorPos);
  }

  // Window buttons
  const ImU32 buttonColN =
      UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 0.9f);
  const ImU32 buttonColH =
      UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.2f);
  const ImU32 buttonColP = UI::Colors::Theme::textDarker;
  const float buttonWidth = 11.0f;
  const float buttonHeight = 11.0f;

  ImGui::Spring();
  UI::ShiftCursorY(8.0f);

  if (m_Specifications.FramebarCallback) {
    m_Specifications.FramebarCallback();
  }

  if (m_Specifications.CustomTitlebar) {
    {
      ImGui::Spring(-1.0f, 70.0f);
      UI::ShiftCursorY(8.0f);

      if (!m_Specifications.WindowOnlyClosable) {
        const int iconWidth =
            this->get(g_WindowMinimizeIcon, "Minimize")->GetWidth();
        const int iconHeight =
            this->get(g_WindowMinimizeIcon, "Minimize")->GetHeight();
        const float padY = (buttonHeight - (float)iconHeight) / 2.0f;

        std::string label = "Minimize###" + this->GetName();
        if (ImGui::InvisibleButton("Minimize",
                                   ImVec2(buttonWidth, buttonHeight))) {
          if (this->GetWindowHandle()) {
            SDL_MinimizeWindow(this->GetWindowHandle());
          }
        }

        UI::DrawButtonImage(this->get(g_WindowMinimizeIcon, "Minimize"),
                            buttonColN, buttonColH, buttonColP,
                            UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
      }
    }
    {
      // Maximize / Restore Button
      ImGui::Spring(-1.0f, 17.0f);
      UI::ShiftCursorY(8.0f);

      if (!m_Specifications.WindowOnlyClosable) {
        const int iconWidth =
            this->get(g_WindowMaximizeIcon, "Maximize")->GetWidth();
        const int iconHeight =
            this->get(g_WindowMaximizeIcon, "Maximize")->GetHeight();

        SDL_Window *sdlWindow = this->GetWindowHandle();
        bool isMaximized = SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_MAXIMIZED;

        std::string label = "Maximize###" + this->GetName();
        if (ImGui::InvisibleButton("Maximize",
                                   ImVec2(buttonWidth, buttonHeight))) {
          if (isMaximized) {
            SDL_RestoreWindow(sdlWindow);
          } else {
            SDL_MaximizeWindow(sdlWindow);
          }
        }

        UI::DrawButtonImage(isMaximized
                                ? this->get(g_WindowMaximizeIcon, "Maximize")
                                : this->get(g_WindowRestoreIcon, "Restore"),
                            buttonColN, buttonColH, buttonColP);
      }
    }
    {
      // Close Button
      ImGui::Spring(-1.0f, 15.0f);
      UI::ShiftCursorY(8.0f);
      {
        const int iconWidth =
            this->get(g_WindowCloseIcon, "Maximize")->GetWidth();
        const int iconHeight =
            this->get(g_WindowCloseIcon, "Maximize")->GetHeight();

        std::string label = "Close###" + this->GetName();
        if (ImGui::InvisibleButton("Close",
                                   ImVec2(buttonWidth, buttonHeight))) {
          m_ClosePending = true;
        }

        UI::DrawButtonImage(
            this->get(g_WindowCloseIcon, "Close"), UI::Colors::Theme::text,
            UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.4f),
            buttonColP);
      }

      ImGui::Spring(-1.0f, 18.0f);
    }

    if (m_ClosePending) {
      if (m_Specifications.UsingCloseCallback) {
        if (m_Specifications.CloseCallback) {
          m_Specifications.CloseCallback();
        }
        m_ClosePending = false;
      } else {
        if (Application::Get().m_CloseCallback) {
          Application::Get().m_CloseCallback();
        } else {
          Application::Get().Close();
        }
      }
    }
  }
  ImGui::EndHorizontal();

  outTitlebarHeight = titlebarHeight;
}

void Window::UI_DrawMenubar() {
  if (!Application::Get().m_MenubarCallback)
    return;

  if (m_Specifications.CustomTitlebar) {
    const ImRect menuBarRect = {
        ImGui::GetCursorPos(),
        {ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x,
         ImGui::GetFrameHeightWithSpacing()}};

    ImGui::BeginGroup();
    if (UI::BeginMenubar(menuBarRect)) {
      float oldsize = ImGui::GetFont()->Scale;
      ImGui::GetFont()->Scale *= m_Specifications.FontGlobalScale;
      ImGui::PushFont(ImGui::GetFont());

      if (this->m_Specifications.MenubarCallback) {
        this->m_Specifications.MenubarCallback();
      } else {
        Application::Get().m_MenubarCallback();
      }

      ImGui::GetFont()->Scale = oldsize;
      ImGui::PopFont();
    }

    UI::EndMenubar();
    ImGui::EndGroup();
  } else {
    if (ImGui::BeginMenuBar()) {
      Application::Get().m_MenubarCallback();
      ImGui::EndMenuBar();
    }
  }
}

void Window::RequestResize(int width, int height) {
  m_ResizePending = true;
  g_SwapChainRebuild = true;
  m_PendingWidth = width;
  m_PendingHeight = height;
}

void Window::RequestMove(int x, int y) {
  m_MovePending = true;
  g_SwapChainRebuild = true;
  m_PendingX = x;
  m_PendingY = y;
}

template <typename Func> void Window::QueueEvent(Func &&func) {
  m_EventQueue.push(func);
}

void Window::Render() {
  ImGuiIO &io = ImGui::GetIO();
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  static int counter = 0;
  ImGui::Begin("Hello, world!");

  ImGui::Text("This is some useful text.");

  if (ImGui::Button("Button"))
    counter++;
  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  ImGui::Render();
  ImDrawData *main_draw_data = ImGui::GetDrawData();
  ImGui_ImplVulkanH_Window *wd = &this->m_WinData;
  const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f ||
                                  main_draw_data->DisplaySize.y <= 0.0f);
  wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
  wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
  wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
  wd->ClearValue.color.float32[3] = clear_color.w;

  if (!main_is_minimized)
    Application::FrameRender(wd, this, main_draw_data);

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  if (!main_is_minimized)
    Application::FramePresent(wd, this);
}

std::shared_ptr<Cherry::Image> Window::add(const std::string &path) {
  if (path.empty() || path == "none") {
    return nullptr;
  }

  auto it = m_ImageMap.find(path);
  if (it != m_ImageMap.end()) {
    return it->second;
  }

  uint32_t w = 0, h = 0;
  std::vector<uint8_t> hexTable = Application::LoadPngHexa(path);
  const uint8_t *hexData = hexTable.empty() ? g_NotFoundIcon : hexTable.data();

  size_t dataSize = hexTable.empty() ? sizeof(g_NotFoundIcon) : hexTable.size();
  void *data = Cherry::Image::Decode(hexData, dataSize, w, h);

  if (!data) {
    return nullptr;
  }

  std::shared_ptr<Cherry::Image> _icon = std::make_shared<Cherry::Image>(
      w, h, Cherry::ImageFormat::RGBA, this->GetName(), data);
  m_ImageMap[path] = _icon;

  IM_FREE(data);

  return _icon;
}

std::shared_ptr<Cherry::Image> Window::get(const std::string &path) {
  if (path.empty() || path == "none") {
    return nullptr;
  }

  auto it = m_ImageMap.find(path);
  if (it != m_ImageMap.end()) {
    return it->second;
  }

  return this->add(path);
}

std::shared_ptr<Cherry::Image> Window::add(const uint8_t data[],
                                           const std::string &name) {
  auto it = m_HexImageMap.find(name);
  if (it != m_HexImageMap.end()) {
    return it->second;
  }

  uint32_t w = 0, h = 0;
  const size_t dataSize = sizeof(g_NotFoundIcon);

  void *icondata = Cherry::Image::Decode(data, dataSize, w, h);
  if (!icondata) {
    return nullptr;
  }

  std::shared_ptr<Cherry::Image> _icon = std::make_shared<Cherry::Image>(
      w, h, Cherry::ImageFormat::RGBA, this->GetName(), icondata);
  m_HexImageMap[name] = _icon;

  IM_FREE(icondata);

  return _icon;
}

std::shared_ptr<Cherry::Image> Window::get(const uint8_t data[],
                                           const std::string &name) {
  auto it = m_HexImageMap.find(name);
  if (it != m_HexImageMap.end()) {
    return it->second;
  }

  return this->add(data, name);
}

ImTextureID Window::get_texture(const std::string &path) {
  if (path.empty() || path == "none") {
    return nullptr;
  }

  auto it = m_TextureCache.find(path);
  if (it != m_TextureCache.end()) {
    return it->second;
  }

  std::shared_ptr<Cherry::Image> image = this->get(path);
  if (image) {
    ImTextureID texture = image->GetImGuiTextureID();
    m_TextureCache[path] = texture;
    return texture;
  }

  return nullptr;
}

VkDescriptorSet Window::get_texture_descriptor(const std::string &path) {
  if (path.empty() || path == "none") {
    return nullptr;
  }

  std::shared_ptr<Cherry::Image> image = this->get(path);
  if (image) {
    return image->GetDescriptorSet();
  }

  return nullptr;
}

ImVec2 Window::get_texture_size(const std::string &path) {
  if (path.empty() || path == "none") {
    return ImVec2(0, 0);
  }

  std::shared_ptr<Cherry::Image> image = this->get(path);
  if (image) {
    return ImVec2(image->GetWidth(), image->GetHeight());
  }

  return ImVec2(0, 0);
}

void Window::LoadTheme() {
  ImGui::PushStyleColor(ImGuiCol_Header,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_header"))); // color_header
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_header_hovered"))); // color_header_hovered
  ImGui::PushStyleColor(ImGuiCol_HeaderActive,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_header_active"))); // color_header_active

  ImGui::PushStyleColor(ImGuiCol_Button,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "button_color_border"))); // color_button
  ImGui::PushStyleColor(
      ImGuiCol_ButtonHovered,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "button_color_border_hovered"))); // color_button_hovered
  ImGui::PushStyleColor(
      ImGuiCol_ButtonActive,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "button_color_border_pressed"))); // color_button_active

  ImGui::PushStyleColor(ImGuiCol_FrameBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_framebg"))); // color_framebg
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_framebg_hovered"))); // color_framebg_hovered
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_framebg_active"))); // color_framebg_active

  ImGui::PushStyleColor(
      ImGuiCol_Text,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty("color_text")));

  ImGui::PushStyleColor(ImGuiCol_Tab,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_tab"))); // color_tab
  ImGui::PushStyleColor(ImGuiCol_TabHovered,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_tab_hovered"))); // color_tab_hovered
  ImGui::PushStyleColor(ImGuiCol_TabActive,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_tab_active"))); // color_tab_active
  ImGui::PushStyleColor(ImGuiCol_TabUnfocused,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_tab_unfocused"))); // color_tab_unfocused
  ImGui::PushStyleColor(
      ImGuiCol_TabUnfocusedActive,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_tab_unfocused_active"))); // color_tab_unfocused_active

  ImGui::PushStyleColor(ImGuiCol_TitleBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_titlebg"))); // color_titlebg
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_titlebg_active"))); // color_titlebg_active
  ImGui::PushStyleColor(
      ImGuiCol_TitleBgCollapsed,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_titlebg_collapsed"))); // color_titlebg_collapsed

  ImGui::PushStyleColor(ImGuiCol_ResizeGrip,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_resizegrip"))); // color_resizegrip
  ImGui::PushStyleColor(
      ImGuiCol_ResizeGripHovered,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_resizegrip_hovered"))); // color_resizegrip_hovered
  ImGui::PushStyleColor(
      ImGuiCol_ResizeGripActive,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_resizegrip_active"))); // color_resizegrip_active

  ImGui::PushStyleColor(ImGuiCol_ScrollbarBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_scrollbar_bg"))); // color_scrollbar_bg

  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_scrollbar_grab"))); // color_scrollbar_grab
  ImGui::PushStyleColor(
      ImGuiCol_ScrollbarGrabHovered,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_scrollbar_grab_hovered"))); // color_scrollbar_grab_hovered
  ImGui::PushStyleColor(
      ImGuiCol_ScrollbarGrabActive,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_scrollbar_grab_active"))); // color_scrollbar_grab_active

  ImGui::PushStyleColor(ImGuiCol_CheckMark,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_checkmark"))); // color_checkmark

  ImGui::PushStyleColor(ImGuiCol_SliderGrab,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_slidergrab"))); // color_slidergrab
  ImGui::PushStyleColor(
      ImGuiCol_SliderGrabActive,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_slidergrab_active"))); // color_slidergrab_active

  ImGui::PushStyleColor(ImGuiCol_Separator,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_separator"))); // color_separator

  ImGui::PushStyleColor(
      ImGuiCol_SeparatorActive,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_separator_active"))); // color_separator_active
  ImGui::PushStyleColor(
      ImGuiCol_SeparatorHovered,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_separator_hovered"))); // color_separator_hovered

  ImGui::PushStyleColor(ImGuiCol_WindowBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_window_bg"))); // color_window_bg
  ImGui::PushStyleColor(ImGuiCol_ChildBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_child_bg"))); // color_child_bg
  ImGui::PushStyleColor(ImGuiCol_PopupBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_popup_bg"))); // color_popup_bg
  ImGui::PushStyleColor(ImGuiCol_Border,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_border"))); // color_border

  ImGui::PushStyleColor(ImGuiCol_TableHeaderBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_table_header_bg"))); // color_table_header_bg
  ImGui::PushStyleColor(
      ImGuiCol_TableBorderLight,
      Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
          "color_table_border_light"))); // color_table_border_light

  ImGui::PushStyleColor(ImGuiCol_MenuBarBg,
                        Cherry::HexToRGBA(CherryApp.GetActiveThemeProperty(
                            "color_menubar_bg"))); // color_menubar_bg

  auto &style = ImGui::GetStyle();
  style.FrameRounding = 5.0f;   // rounding_frame
  style.FrameBorderSize = 1.0f; // size_frame_border
  style.IndentSpacing = 11.0f;  // spacing_ident
  style.ScrollbarSize = 11.0f;  // size_scrollbar

  style.WindowPadding = ImVec2(20.0f, 20.0f); // padding_window
  style.FramePadding = ImVec2(15.0f, 5.0f);   // padding_frame
}

void Window::UnloadTheme() { ImGui::PopStyleColor(38); }

void Window::CleanupVulkanWindow() {
  ImGui_ImplVulkanH_DestroyWindow(Application::GetInstance(),
                                  Application::GetDevice(), &m_WinData,
                                  &Application::GetVkAllocationCallbacks());
}

VkCommandBuffer Window::GetCommandBuffer(bool begin) {
  ImGui_ImplVulkanH_Window *wd = &this->m_WinData;

  VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

  VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
  cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmdBufAllocateInfo.commandPool = command_pool;
  cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmdBufAllocateInfo.commandBufferCount = 1;

  VkCommandBuffer &command_buffer =
      this->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
  auto err = vkAllocateCommandBuffers(Application::GetDevice(),
                                      &cmdBufAllocateInfo, &command_buffer);

  VkCommandBufferBeginInfo begin_info = {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  err = vkBeginCommandBuffer(command_buffer, &begin_info);
  check_vk_result(err);

  return command_buffer;
}

void Window::LoadTTFFont(const std::string &ttf_font_path) {
  ImFont *font = Application::GetFontList()[ttf_font_path];

  if (font) {
    ImGui::PushFont(font);
    m_FontLoaded = true;
  }
}

void Window::RestoreTTFFont() {
  if (m_FontLoaded) {
    m_FontLoaded = false;
    ImGui::PopFont();
  }
}

std::string Window::GetThemeProperty(const std::string &key) {
  if (m_SelectedTheme == "undefined") {
    return "undefined";
  } else {
    auto prop = CherryApp.GetThemeProperty(m_SelectedTheme, key);
    if (prop == "undefined") {
      return "undefined";
    } else {
      return prop;
    }
  }
}

void Window::SetTheme(const std::string &theme_name) {
  m_SelectedTheme = theme_name;
}

void Window::RemoveTheme() { m_SelectedTheme = "undefined"; }

} // namespace Cherry