//
//  window.hpp
//  Headers for Cherry windows (desktop environment
//  window implementation and rendering)
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include <lib/sdl2/include/SDL.h>
#include <lib/sdl2/include/SDL_vulkan.h>
#include <main/engine/imgui/imgui_theme.h>

#include <filesystem>
#include <functional>
#include <iostream>
#include <main/core/color/color.hpp>
#include <main/engine/app/app.hpp>
#include <main/engine/app_window/app_window.hpp>
#include <main/engine/base.hpp>
#include <main/engine/image/image.hpp>
#include <main/engine/ui/ui.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "vulkan/vulkan.h"
#include "window_helpers.hpp"

namespace Cherry {
  class Application;

  class CHERRY_API Window {
   public:
    Window(const std::string &name, int width, int height, ApplicationSpecification specs, bool cold_start = true);
    Window();

    ~Window();

    void ProcessMouseEvents();

    VkCommandBuffer GetCommandBuffer(bool begin);
    SDL_Window *GetWindowHandle() const;
    static void ShowDockingPreview(
        ImGuiID dockspaceID,
        Window *win,
        const std::shared_ptr<Cherry::WindowDragDropState> &dragState,
        const std::shared_ptr<AppWindow> appwin = nullptr);

    // void OnWindowResize(GLFWwindow *windowHandle, int width, int height);
    // void OnWindowMove(int xpos, int ypos);

    void BeginFrame();
    void EndFrame();

    void OnUpdate();

    const std::string &GetName() const;

    void CreateImage(std::shared_ptr<Cherry::Image> image, void *data);

    void UI_DrawTitlebar(float &outTitlebarHeight);
    void UI_DrawMenubar();

    void CleanupVulkanWindow();

    template<typename Func>
    void QueueEvent(Func &&func);

    void RequestResize(int width, int height);
    void RequestMove(int x, int y);

    std::string GetThemeProperty(const std::string &key);

    void LoadTTFFont(const std::string &ttf_font_path);

    void RestoreTTFFont();

    void PutUniqueAppwindow(const std::shared_ptr<AppWindow> &appwindow);
    void ApplyPendingResize();
    void SetFavIcon(const std::string &path);

    void Render();

    // Different than the default theme !
    // When the user specify this, the runtime will search level 2 theme at
    // GetThemeProperty(theme, key) "manually", and not from the active theme.
    void SetTheme(const std::string &theme_name);
    void RemoveTheme();

    ImGuiContext *&GetImGuiContext();
    std::string &GetSelectedTheme();
    ApplicationSpecification &GetSpecifications();
    bool &GetNeedToRebuildFontMap();
    bool &GetFontLoaded();
    std::shared_ptr<AppWindow> &GetUniqueAppWindow();
    ImFont *&GetFontToRestore();
    std::vector<char> &GetFontBuffer();
    void SetImGuiContext(ImGuiContext *ctx);
    void SetSelectedTheme(const std::string &theme);
    void SetSpecifications(const ApplicationSpecification &specs);
    void SetNeedToRebuildFontMap(bool v);
    void SetFontLoaded(bool v);
    void SetUniqueAppWindow(std::shared_ptr<AppWindow> appWindow);
    void SetFontToRestore(ImFont *font);
    void SetFontBuffer(std::vector<char> buffer);

    std::unordered_map<std::string, ImFont *> &GetFonts();
    std::unordered_map<std::string, ImTextureID> &GetTextureCache();
    std::unordered_map<std::string, std::shared_ptr<Cherry::Image>> &GetImageMap();
    std::unordered_map<std::string, std::shared_ptr<Cherry::Image>> &GetHexImageMap();
    std::unordered_map<std::string, ImFont *> &GetFontMap();

    std::mutex &GetEventQueueMutex();
    std::queue<std::function<void()>> &GetEventQueue();
    bool &GetSwapChainRebuild();
    VkSwapchainKHR &GetSwapchain();
    std::vector<VkImage> &GetSwapchainImages();
    std::vector<VkImageView> &GetSwapchainImageViews();
    VkFormat &GetSwapchainImageFormat();
    std::vector<std::vector<std::function<void()>>> &GetResourceFreeQueue();
    uint32_t &GetCurrentFrameIndex();
    std::vector<VkCommandBuffer> &GetCommandBuffers();
    ImGuiWindow *&GetImGuiWindow();

    void LoadTheme();
    void UnloadTheme();

    std::shared_ptr<Cherry::Image> add(const std::string &path);
    std::shared_ptr<Cherry::Image> add(const uint8_t data[], const std::string &name);
    std::shared_ptr<Cherry::Image> get(const std::string &path);
    std::shared_ptr<Cherry::Image> get(const uint8_t data[], const std::string &name);
    ImTextureID get_texture(const std::string &path);
    ImVec2 get_texture_size(const std::string &path);
    VkDescriptorSet get_texture_descriptor(const std::string &path);

    ImGui_ImplVulkanH_Window *GetWinData();
    const ImDrawData &GetDrawData() const;
    ImDrawData *GetDrawData();
    std::vector<std::vector<VkCommandBuffer>> &GetAllocatedCommandBuffers();

    bool &GetResizePending();
    bool &GetMovePending();
    int &GetPendingWidth();
    int &GetPendingHeight();
    int &GetPendingX();
    int &GetPendingY();
    int &GetPreviousWidth();
    int &GetPreviousHeight();
    int &GetPreviousX();
    int &GetPreviousY();
    int &GetPosX();
    int &GetPosY();
    bool &GetIsClosing();
    bool &GetIsMoving();
    ImVec2 &GetClickOffset();
    bool &GetIsDraggingAppWindow();
    bool &GetClosePending();
    bool &GetResizing();

    void SetResizePending(bool v);
    void SetMovePending(bool v);
    void SetPendingWidth(int v);
    void SetPendingHeight(int v);
    void SetPendingX(int v);
    void SetPendingY(int v);
    void SetPreviousWidth(int v);
    void SetPreviousHeight(int v);
    void SetPreviousX(int v);
    void SetPreviousY(int v);
    void SetPosX(int v);
    void SetPosY(int v);
    void SetIsClosing(bool v);
    void SetIsMoving(bool v);
    void SetClickOffset(ImVec2 v);
    void SetIsDraggingAppWindow(bool v);
    void SetClosePending(bool v);
    void SetResizing(bool v);

    VkSurfaceKHR GetSurface() const;
    int GetWinID() const;
    std::shared_ptr<WindowClickEvent> GetPendingClick() const;
    std::shared_ptr<WindowMoveEvent> GetPendingMove() const;
    std::shared_ptr<WindowDragDropState> GetDragDropState() const;

    void SetSurface(VkSurfaceKHR surface);
    void SetWinID(int id);

    void SetPendingClick(std::shared_ptr<WindowClickEvent> click);
    void SetPendingMove(std::shared_ptr<WindowMoveEvent> move);
    void SetDragDropState(std::shared_ptr<WindowDragDropState> state);

   private:
    bool m_ResizePending = false;
    bool m_MovePending = false;
    int m_PendingWidth = 0;
    int m_PendingHeight = 0;
    int m_PendingX = 0;
    int m_PendingY = 0;
    int m_PreviousWidth = 0;
    int m_PreviousHeight = 0;
    int m_PreviousX = 0;
    int m_PreviousY = 0;
    int m_PosX = 0;
    int m_PosY = 0;
    bool m_IsClosing = false;
    bool isMoving = false;
    ImVec2 clickOffset = { 0.0f, 0.0f };
    bool m_IsDraggingAppWindow = false;
    bool m_ClosePending = false;
    bool m_Resizing = false;

    ImGuiContext *m_ImGuiContext;
    std::string m_SelectedTheme = "undefined";
    ApplicationSpecification m_Specifications;
    bool m_NeedToRebuildFontMap = false;
    bool m_FontLoaded = false;
    std::shared_ptr<AppWindow> m_UniqueAppWindow;
    ImFont *m_FontToRestore = nullptr;  // To store the font to restore
    std::vector<char> fontBuffer;       // Keep the font buffer in memory

    VkSurfaceKHR m_Surface;
    std::shared_ptr<WindowClickEvent> m_PendingClick = nullptr;
    std::shared_ptr<WindowMoveEvent> m_PendingMove = nullptr;
    std::shared_ptr<WindowDragDropState> drag_dropstate;
    int WinID;
    std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
    ImDrawData m_DrawData;
    int m_Width, m_Height;

    std::unordered_map<std::string, ImFont *> s_Fonts;
    std::unordered_map<std::string, ImTextureID> m_TextureCache;
    std::unordered_map<std::string, std::shared_ptr<Cherry::Image>> m_ImageMap;
    std::unordered_map<std::string, std::shared_ptr<Cherry::Image>> m_HexImageMap;
    std::unordered_map<std::string, ImFont *> m_FontMap;

    std::mutex m_EventQueueMutex;
    std::queue<std::function<void()>> m_EventQueue;
    bool g_SwapChainRebuild = false;
    VkSwapchainKHR g_Swapchain;
    std::vector<VkImage> g_SwapchainImages;
    std::vector<VkImageView> g_SwapchainImageViews;
    VkFormat g_SwapchainImageFormat;
    std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;
    uint32_t s_CurrentFrameIndex = 0;
    std::vector<VkCommandBuffer> m_CommandBuffers;
    ImGuiWindow *m_ImGuiWindow;
    std::function<void()> m_MenubarCallback;
    std::string m_Name;
    SDL_Window *m_WindowHandler;

    ImGui_ImplVulkanH_Window m_WinData;

    VkPipelineLayout pipelineLayout;
    ImGuiViewport *m_Viewport = nullptr;
    bool m_IsDragging = false;
    ImVec2 m_InitialMousePos;
    ImVec2 m_InitialWindowPos;
  };
}  // namespace Cherry