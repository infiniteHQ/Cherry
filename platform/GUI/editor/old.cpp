#include "ApplicationGUI.h"
#include "../../../src/core/Log.h"

//
// Adapted from Dear ImGui Vulkan example
//

/*
TODO :

Move, Resize, Click (click callback, and link to imgui ctx ?)
Window factory to spawn window when undock
Cool widgets & ui elements !

*/

#include <stdio.h>	// printf, fprintf
#include <thread>	// thread
#include <stdlib.h> // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

#include <iostream>

// Emedded font
#include "ImGui/Roboto-Regular.embed"
#include "ImGui/Roboto-Bold.embed"
#include "ImGui/Roboto-Italic.embed"
#include "ImGui/Hack-Regular.embed"
#include "ImGui/Inconsolatas.embed"

extern bool g_ApplicationRunning;

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// #define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

static VkAllocationCallbacks *g_Allocator = NULL;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = (uint32_t)-1;
static VkQueue g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
static std::mutex m_EventQueueMutex;
static std::queue<std::function<void()>> m_EventQueue;
static int g_MinImageCount = 2;
static bool g_SwapChainRebuild = false;
static VkSwapchainKHR g_Swapchain;
static std::vector<VkImage> g_SwapchainImages;
static std::vector<VkImageView> g_SwapchainImageViews;
static VkFormat g_SwapchainImageFormat;

static int WinIDCount = 0;

// Per-frame-in-flight
static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;

// Unlike g_MainWindowData.FrameIndex, this is not the the swapchain image index
// and is always guaranteed to increase (eg. 0, 1, 2, 0, 1, 2)
static uint32_t s_CurrentFrameIndex = 0;

static std::unordered_map<std::string, ImFont *> s_Fonts;

static UIKit::Application *s_Instance = nullptr;

void check_vk_result(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData)
{
	(void)flags;
	(void)object;
	(void)location;
	(void)messageCode;
	(void)pUserData;
	(void)pLayerPrefix; // Unused arguments
						//	fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
	return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT


ImGuiContext *m_ImGuiContext;

namespace ImGui
{
	
const ImGuiID           IMGUI_VIEWPORT_DEFAULT_ID = 0x11111111; // Using an arbitrary constant instead of e.g. ImHashStr("ViewportDefault", 0); so it's easier to spot in the debugger. The exact value doesn't matter.

// When using CTRL+TAB (or Gamepad Square+L/R) we delay the visual a little in order to reduce visual noise doing a fast switch.
static const float NAV_WINDOWING_HIGHLIGHT_DELAY            = 0.20f;    // Time before the highlight and screen dimming starts fading in
static const float NAV_WINDOWING_LIST_APPEAR_DELAY          = 0.15f;    // Time before the window list starts to appear

// Window resizing from edges (when io.ConfigWindowsResizeFromEdges = true and ImGuiBackendFlags_HasMouseCursors is set in io.BackendFlags by backend)
static const float WINDOWS_HOVER_PADDING                    = 4.0f;     // Extend outside window for hovering/resizing (maxxed with TouchPadding) and inside windows for borders. Affect FindHoveredWindow().
static const float WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TIMER = 0.04f;    // Reduce visual noise by only highlighting the border after a certain time.
static const float WINDOWS_MOUSE_WHEEL_SCROLL_LOCK_TIMER    = 2.00f;    // Lock scrolled window (so it doesn't pick child windows that are scrolling through) for a certain time, unless mouse moved.

// Docking
static const float DOCKING_TRANSPARENT_PAYLOAD_ALPHA        = 0.50f;    // For use with io.ConfigDockingTransparentPayload. Apply to Viewport _or_ WindowBg in host viewport.
static const float DOCKING_SPLITTER_SIZE                    = 2.0f;


// Debug options
#define IMGUI_DEBUG_NAV_SCORING     0   // Display navigation scoring preview when hovering items. Display last moving direction matches when holding CTRL
#define IMGUI_DEBUG_NAV_RECTS       0   // Display the reference navigation rectangle for each window
#define IMGUI_DEBUG_INI_SETTINGS    0   // Save additional comments in .ini file (particularly helps for Docking, but makes saving slower)

// Search for the monitor with the largest intersection area with the given rectangle
// We generally try to avoid searching loops but the monitor count should be very small here
// FIXME-OPT: We could test the last monitor used for that viewport first, and early
static int UIKITFindPlatformMonitorForRect(const ImRect& rect)
{
    ImGuiContext& g = *GImGui;

    const int monitor_count = g.PlatformIO.Monitors.Size;
    if (monitor_count <= 1)
        return monitor_count - 1;

    // Use a minimum threshold of 1.0f so a zero-sized rect won't false positive, and will still find the correct monitor given its position.
    // This is necessary for tooltips which always resize down to zero at first.
    const float surface_threshold = ImMax(rect.GetWidth() * rect.GetHeight() * 0.5f, 1.0f);
    int best_monitor_n = -1;
    float best_monitor_surface = 0.001f;

    for (int monitor_n = 0; monitor_n < g.PlatformIO.Monitors.Size && best_monitor_surface < surface_threshold; monitor_n++)
    {
        const ImGuiPlatformMonitor& monitor = g.PlatformIO.Monitors[monitor_n];
        const ImRect monitor_rect = ImRect(monitor.MainPos, monitor.MainPos);
        if (monitor_rect.Contains(rect))
            return monitor_n;
        ImRect overlapping_rect = rect;
        overlapping_rect.ClipWithFull(monitor_rect);
        float overlapping_surface = overlapping_rect.GetWidth() * overlapping_rect.GetHeight();
        if (overlapping_surface < best_monitor_surface)
            continue;
        best_monitor_surface = overlapping_surface;
        best_monitor_n = monitor_n;
    }
    return best_monitor_n;
}


// Update monitor from viewport rectangle (we'll use this info to clamp windows and save windows lost in a removed monitor)
static void UIKITUpdateViewportPlatformMonitor(ImGuiViewportP* viewport)
{
    viewport->PlatformMonitor = (short)UIKITFindPlatformMonitorForRect(viewport->GetMainRect());
}

// FIXME: We should ideally refactor the system to call this every frame (we currently don't)
ImGuiViewportP* UIKITAddUpdateViewport(ImGuiWindow* window, ImGuiID id, const ImVec2& pos, const ImVec2& size, ImGuiViewportFlags flags)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(id != 0);

    flags |= ImGuiViewportFlags_IsPlatformWindow;
    if (window != NULL)
    {
        if (g.MovingWindow && g.MovingWindow->RootWindowDockTree == window)
            flags |= ImGuiViewportFlags_NoInputs | ImGuiViewportFlags_NoFocusOnAppearing;
        if ((window->Flags & ImGuiWindowFlags_NoMouseInputs) && (window->Flags & ImGuiWindowFlags_NoNavInputs))
            flags |= ImGuiViewportFlags_NoInputs;
        if (window->Flags & ImGuiWindowFlags_NoFocusOnAppearing)
            flags |= ImGuiViewportFlags_NoFocusOnAppearing;
    }

    ImGuiViewportP* viewport = (ImGuiViewportP*)FindViewportByID(id);
    if (viewport)
    {
        // Always update for main viewport as we are already pulling correct platform pos/size (see #4900)
        if (!viewport->PlatformRequestMove || viewport->ID == IMGUI_VIEWPORT_DEFAULT_ID)
            viewport->Pos = pos;
        if (!viewport->PlatformRequestResize || viewport->ID == IMGUI_VIEWPORT_DEFAULT_ID)
            viewport->Size = size;
        viewport->Flags = flags | (viewport->Flags & ImGuiViewportFlags_Minimized); // Preserve existing flags
    }
    else
    {
        // New viewport
        viewport = IM_NEW(ImGuiViewportP)();
        viewport->ID = id;
        viewport->Idx = g.Viewports.Size;
        viewport->Pos = viewport->LastPos = pos;
        viewport->Size = size;
        viewport->Flags = flags;
        UIKITUpdateViewportPlatformMonitor(viewport);
        g.Viewports.push_back(viewport);
        IMGUI_DEBUG_LOG_VIEWPORT("Add Viewport %08X (%s)\n", id, window->Name);

        // We normally setup for all viewports in NewFrame() but here need to handle the mid-frame creation of a new viewport.
        // We need to extend the fullscreen clip rect so the OverlayDrawList clip is correct for that the first frame
        g.DrawListSharedData.ClipRectFullscreen.x = ImMin(g.DrawListSharedData.ClipRectFullscreen.x, viewport->Pos.x);
        g.DrawListSharedData.ClipRectFullscreen.y = ImMin(g.DrawListSharedData.ClipRectFullscreen.y, viewport->Pos.y);
        g.DrawListSharedData.ClipRectFullscreen.z = ImMax(g.DrawListSharedData.ClipRectFullscreen.z, viewport->Pos.x + viewport->Size.x);
        g.DrawListSharedData.ClipRectFullscreen.w = ImMax(g.DrawListSharedData.ClipRectFullscreen.w, viewport->Pos.y + viewport->Size.y);

        // Store initial DpiScale before the OS platform window creation, based on expected monitor data.
        // This is so we can select an appropriate font size on the first frame of our window lifetime
        if (viewport->PlatformMonitor != -1)
            viewport->DpiScale = g.PlatformIO.Monitors[viewport->PlatformMonitor].DpiScale;
    }

    viewport->Window = window;
    viewport->LastFrameActive = g.FrameCount;
    viewport->UpdateWorkRect();
    IM_ASSERT(window == NULL || viewport->ID == window->ID);

    if (window != NULL)
        window->ViewportOwned = true;

    return viewport;
}

static void UIKITDestroyViewport(ImGuiViewportP* viewport)
{
    // Clear references to this viewport in windows (window->ViewportId becomes the master data)
    ImGuiContext& g = *GImGui;
    for (int window_n = 0; window_n < g.Windows.Size; window_n++)
    {
        ImGuiWindow* window = g.Windows[window_n];
        if (window->Viewport != viewport)
            continue;
        window->Viewport = NULL;
        window->ViewportOwned = false;
    }
    if (viewport == g.MouseLastHoveredViewport)
        g.MouseLastHoveredViewport = NULL;

    // Destroy
    IMGUI_DEBUG_LOG_VIEWPORT("Delete Viewport %08X (%s)\n", viewport->ID, viewport->Window ? viewport->Window->Name : "n/a");
    DestroyPlatformWindow(viewport); // In most circumstances the platform window will already be destroyed here.
    IM_ASSERT(g.PlatformIO.Viewports.contains(viewport) == false);
    IM_ASSERT(g.Viewports[viewport->Idx] == viewport);
    g.Viewports.erase(g.Viewports.Data + viewport->Idx);
    IM_DELETE(viewport);
}

static void UIKIt_UpdateViewportsNewFrame()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.PlatformIO.Viewports.Size <= g.Viewports.Size);

    // Update Minimized status (we need it first in order to decide if we'll apply Pos/Size of the main viewport)
    const bool viewports_enabled = (g.ConfigFlagsCurrFrame & ImGuiConfigFlags_ViewportsEnable) != 0;
    if (viewports_enabled)
    {
        for (int n = 0; n < g.Viewports.Size; n++)
        {
            ImGuiViewportP* viewport = g.Viewports[n];
            const bool platform_funcs_available = viewport->PlatformWindowCreated;
            if (g.PlatformIO.Platform_GetWindowMinimized && platform_funcs_available)
            {
                bool minimized = g.PlatformIO.Platform_GetWindowMinimized(viewport);
                if (minimized)
                    viewport->Flags |= ImGuiViewportFlags_Minimized;
                else
                    viewport->Flags &= ~ImGuiViewportFlags_Minimized;
            }
        }
    }

    // Create/update main viewport with current platform position.
    // FIXME-VIEWPORT: Size is driven by backend/user code for backward-compatibility but we should aim to make this more consistent.
    ImGuiViewportP* main_viewport = g.Viewports[0];
    IM_ASSERT(main_viewport->ID == IMGUI_VIEWPORT_DEFAULT_ID);
    IM_ASSERT(main_viewport->Window == NULL);
    ImVec2 main_viewport_pos = viewports_enabled ? g.PlatformIO.Platform_GetWindowPos(main_viewport) : ImVec2(0.0f, 0.0f);
    ImVec2 main_viewport_size = g.IO.DisplaySize;
    if (viewports_enabled && (main_viewport->Flags & ImGuiViewportFlags_Minimized))
    {
        main_viewport_pos = main_viewport->Pos;    // Preserve last pos/size when minimized (FIXME: We don't do the same for Size outside of the viewport path)
        main_viewport_size = main_viewport->Size;
    }
    UIKITAddUpdateViewport(NULL, IMGUI_VIEWPORT_DEFAULT_ID, main_viewport_pos, main_viewport_size, ImGuiViewportFlags_OwnedByApp | ImGuiViewportFlags_CanHostOtherWindows);

    g.CurrentDpiScale = 0.0f;
    g.CurrentViewport = NULL;
    g.MouseViewport = NULL;
    for (int n = 0; n < g.Viewports.Size; n++)
    {
        ImGuiViewportP* viewport = g.Viewports[n];
        viewport->Idx = n;

        // Erase unused viewports
        if (n > 0 && viewport->LastFrameActive < g.FrameCount - 2)
        {
            UIKITDestroyViewport(viewport);
            n--;
            continue;
        }

        const bool platform_funcs_available = viewport->PlatformWindowCreated;
        if (viewports_enabled)
        {
            // Update Position and Size (from Platform Window to ImGui) if requested.
            // We do it early in the frame instead of waiting for UpdatePlatformWindows() to avoid a frame of lag when moving/resizing using OS facilities.
            if (!(viewport->Flags & ImGuiViewportFlags_Minimized) && platform_funcs_available)
            {
                // Viewport->WorkPos and WorkSize will be updated below
                if (viewport->PlatformRequestMove)
                    viewport->Pos = viewport->LastPlatformPos = g.PlatformIO.Platform_GetWindowPos(viewport);
                if (viewport->PlatformRequestResize)
                    viewport->Size = viewport->LastPlatformSize = g.PlatformIO.Platform_GetWindowSize(viewport);
            }
        }

        // Update/copy monitor info
        UIKITUpdateViewportPlatformMonitor(viewport);

        // Lock down space taken by menu bars and status bars, reset the offset for functions like BeginMainMenuBar() to alter them again.
        viewport->WorkOffsetMin = viewport->BuildWorkOffsetMin;
        viewport->WorkOffsetMax = viewport->BuildWorkOffsetMax;
        viewport->BuildWorkOffsetMin = viewport->BuildWorkOffsetMax = ImVec2(0.0f, 0.0f);
        viewport->UpdateWorkRect();

        // Reset alpha every frame. Users of transparency (docking) needs to request a lower alpha back.
        viewport->Alpha = 1.0f;

        // Translate Dear ImGui windows when a Host Viewport has been moved
        // (This additionally keeps windows at the same place when ImGuiConfigFlags_ViewportsEnable is toggled!)
        const ImVec2 viewport_delta_pos = viewport->Pos;
        if ((viewport->Flags & ImGuiViewportFlags_CanHostOtherWindows) && (viewport_delta_pos.x != 0.0f || viewport_delta_pos.y != 0.0f))
            TranslateWindowsInViewport(viewport, viewport->LastPos, viewport->Pos);

        // Update DPI scale
        float new_dpi_scale;
        if (g.PlatformIO.Platform_GetWindowDpiScale && platform_funcs_available)
            new_dpi_scale = g.PlatformIO.Platform_GetWindowDpiScale(viewport);
        else if (viewport->PlatformMonitor != -1)
            new_dpi_scale = g.PlatformIO.Monitors[viewport->PlatformMonitor].DpiScale;
        else
            new_dpi_scale = (viewport->DpiScale != 0.0f) ? viewport->DpiScale : 1.0f;
        if (viewport->DpiScale != 0.0f && new_dpi_scale != viewport->DpiScale)
        {
            float scale_factor = new_dpi_scale / viewport->DpiScale;
            if (g.IO.ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
                ScaleWindowsInViewport(viewport, scale_factor);
            //if (viewport == GetMainViewport())
            //    g.PlatformInterface.SetWindowSize(viewport, viewport->Size * scale_factor);

            // Scale our window moving pivot so that the window will rescale roughly around the mouse position.
            // FIXME-VIEWPORT: This currently creates a resizing feedback loop when a window is straddling a DPI transition border.
            // (Minor: since our sizes do not perfectly linearly scale, deferring the click offset scale until we know the actual window scale ratio may get us slightly more precise mouse positioning.)
            //if (g.MovingWindow != NULL && g.MovingWindow->Viewport == viewport)
            //    g.ActiveIdClickOffset = ImFloor(g.ActiveIdClickOffset * scale_factor);
        }
        viewport->DpiScale = new_dpi_scale;
    }

    // Update fallback monitor
    if (g.PlatformIO.Monitors.Size == 0)
    {
        ImGuiPlatformMonitor* monitor = &g.FallbackMonitor;
        monitor->MainPos = main_viewport->Pos;
        monitor->MainSize = main_viewport->Size;
        monitor->WorkPos = main_viewport->WorkPos;
        monitor->WorkSize = main_viewport->WorkSize;
        monitor->DpiScale = main_viewport->DpiScale;
    }

    if (!viewports_enabled)
    {
        g.MouseViewport = main_viewport;
        return;
    }

    // Mouse handling: decide on the actual mouse viewport for this frame between the active/focused viewport and the hovered viewport.
    // Note that 'viewport_hovered' should skip over any viewport that has the ImGuiViewportFlags_NoInputs flags set.
    ImGuiViewportP* viewport_hovered = NULL;
    if (g.IO.BackendFlags & ImGuiBackendFlags_HasMouseHoveredViewport)
    {
        viewport_hovered = g.IO.MouseHoveredViewport ? (ImGuiViewportP*)FindViewportByID(g.IO.MouseHoveredViewport) : NULL;
        if (viewport_hovered && (viewport_hovered->Flags & ImGuiViewportFlags_NoInputs))
            viewport_hovered = FindHoveredViewportFromPlatformWindowStack(g.IO.MousePos); // Backend failed to handle _NoInputs viewport: revert to our fallback.
    }
    else
    {
        // If the backend doesn't know how to honor ImGuiViewportFlags_NoInputs, we do a search ourselves. Note that this search:
        // A) won't take account of the possibility that non-imgui windows may be in-between our dragged window and our target window.
        // B) won't take account of how the backend apply parent<>child relationship to secondary viewports, which affects their Z order.
        // C) uses LastFrameAsRefViewport as a flawed replacement for the last time a window was focused (we could/should fix that by introducing Focus functions in PlatformIO)
        viewport_hovered = FindHoveredViewportFromPlatformWindowStack(g.IO.MousePos);
    }
    if (viewport_hovered != NULL)
        g.MouseLastHoveredViewport = viewport_hovered;
    else if (g.MouseLastHoveredViewport == NULL)
        g.MouseLastHoveredViewport = g.Viewports[0];

    // Update mouse reference viewport
    // (when moving a window we aim at its viewport, but this will be overwritten below if we go in drag and drop mode)
    // (MovingViewport->Viewport will be NULL in the rare situation where the window disappared while moving, set UpdateMouseMovingWindowNewFrame() for details)
    if (g.MovingWindow && g.MovingWindow->Viewport)
        g.MouseViewport = g.MovingWindow->Viewport;
    else
        g.MouseViewport = g.MouseLastHoveredViewport;

    // When dragging something, always refer to the last hovered viewport.
    // - when releasing a moving window we will revert to aiming behind (at viewport_hovered)
    // - when we are between viewports, our dragged preview will tend to show in the last viewport _even_ if we don't have tooltips in their viewports (when lacking monitor info)
    // - consider the case of holding on a menu item to browse child menus: even thou a mouse button is held, there's no active id because menu items only react on mouse release.
    // FIXME-VIEWPORT: This is essentially broken, when ImGuiBackendFlags_HasMouseHoveredViewport is set we want to trust when viewport_hovered==NULL and use that.
    const bool is_mouse_dragging_with_an_expected_destination = g.DragDropActive;
    if (is_mouse_dragging_with_an_expected_destination && viewport_hovered == NULL)
        viewport_hovered = g.MouseLastHoveredViewport;
    if (is_mouse_dragging_with_an_expected_destination || g.ActiveId == 0 || !IsAnyMouseDown())
        if (viewport_hovered != NULL && viewport_hovered != g.MouseViewport && !(viewport_hovered->Flags & ImGuiViewportFlags_NoInputs))
            g.MouseViewport = viewport_hovered;

    IM_ASSERT(g.MouseViewport != NULL);
}


// Restore the last focused child.
// Call when we are expected to land on the Main Layer (0) after FocusWindow()
static ImGuiWindow* NavRestoreLastChildNavWindow(ImGuiWindow* window)
{
    if (window->NavLastChildNavWindow && window->NavLastChildNavWindow->WasActive)
        return window->NavLastChildNavWindow;
    if (window->DockNodeAsHost && window->DockNodeAsHost->TabBar)
        if (ImGuiTabItem* tab = TabBarFindMostRecentlySelectedTabForActiveWindow(window->DockNodeAsHost->TabBar))
            return tab->Window;
    return window;
}


void NavRestoreLayer(ImGuiNavLayer layer)
{
    ImGuiContext& g = *GImGui;
    if (layer == ImGuiNavLayer_Main)
        g.NavWindow = NavRestoreLastChildNavWindow(g.NavWindow);
    ImGuiWindow* window = g.NavWindow;
    if (window->NavLastIds[layer] != 0)
    {
        SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);
    }
    else
    {
        g.NavLayer = layer;
        NavInitWindow(window, true);
    }
}


static void ErrorCheckNewFrameSanityChecks()
{
    ImGuiContext& g = *GImGui;

    // Check user IM_ASSERT macro
    // (IF YOU GET A WARNING OR COMPILE ERROR HERE: it means your assert macro is incorrectly defined!
    //  If your macro uses multiple statements, it NEEDS to be surrounded by a 'do { ... } while (0)' block.
    //  This is a common C/C++ idiom to allow multiple statements macros to be used in control flow blocks.)
    // #define IM_ASSERT(EXPR)   if (SomeCode(EXPR)) SomeMoreCode();                    // Wrong!
    // #define IM_ASSERT(EXPR)   do { if (SomeCode(EXPR)) SomeMoreCode(); } while (0)   // Correct!
    if (true) IM_ASSERT(1); else IM_ASSERT(0);

    // Check user data
    // (We pass an error message in the assert expression to make it visible to programmers who are not using a debugger, as most assert handlers display their argument)
    IM_ASSERT(g.Initialized);
    IM_ASSERT((g.IO.DeltaTime > 0.0f || g.FrameCount == 0)              && "Need a positive DeltaTime!");
    IM_ASSERT((g.FrameCount == 0 || g.FrameCountEnded == g.FrameCount)  && "Forgot to call Render() or EndFrame() at the end of the previous frame?");
    IM_ASSERT(g.IO.DisplaySize.x >= 0.0f && g.IO.DisplaySize.y >= 0.0f  && "Invalid DisplaySize value!");
    IM_ASSERT(g.IO.Fonts->IsBuilt()                                     && "Font Atlas not built! Make sure you called ImGui_ImplXXXX_NewFrame() function for renderer backend, which should call io.Fonts->GetTexDataAsRGBA32() / GetTexDataAsAlpha8()");
    IM_ASSERT(g.Style.CurveTessellationTol > 0.0f                       && "Invalid style setting!");
    IM_ASSERT(g.Style.CircleTessellationMaxError  > 0.0f                && "Invalid style setting!");
    IM_ASSERT(g.Style.Alpha >= 0.0f && g.Style.Alpha <= 1.0f            && "Invalid style setting!"); // Allows us to avoid a few clamps in color computations
    IM_ASSERT(g.Style.WindowMinSize.x >= 1.0f && g.Style.WindowMinSize.y >= 1.0f && "Invalid style setting.");
    IM_ASSERT(g.Style.WindowMenuButtonPosition == ImGuiDir_None || g.Style.WindowMenuButtonPosition == ImGuiDir_Left || g.Style.WindowMenuButtonPosition == ImGuiDir_Right);
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    for (int n = ImGuiKey_NamedKey_BEGIN; n < ImGuiKey_COUNT; n++)
        IM_ASSERT(g.IO.KeyMap[n] >= -1 && g.IO.KeyMap[n] < IM_ARRAYSIZE(g.IO.KeysDown) && "io.KeyMap[] contains an out of bound value (need to be 0..511, or -1 for unmapped key)");

    // Check: required key mapping (we intentionally do NOT check all keys to not pressure user into setting up everything, but Space is required and was only added in 1.60 WIP)
    if ((g.IO.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) && g.IO.BackendUsingLegacyKeyArrays == 1)
        IM_ASSERT(g.IO.KeyMap[ImGuiKey_Space] != -1 && "ImGuiKey_Space is not mapped, required for keyboard navigation.");
#endif

    // Check: the io.ConfigWindowsResizeFromEdges option requires backend to honor mouse cursor changes and set the ImGuiBackendFlags_HasMouseCursors flag accordingly.
    if (g.IO.ConfigWindowsResizeFromEdges && !(g.IO.BackendFlags & ImGuiBackendFlags_HasMouseCursors))
        g.IO.ConfigWindowsResizeFromEdges = false;

    // Perform simple check: error if Docking or Viewport are enabled _exactly_ on frame 1 (instead of frame 0 or later), which is a common error leading to loss of .ini data.
    if (g.FrameCount == 1 && (g.IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) && (g.ConfigFlagsLastFrame & ImGuiConfigFlags_DockingEnable) == 0)
        IM_ASSERT(0 && "Please set DockingEnable before the first call to NewFrame()! Otherwise you will lose your .ini settings!");
    if (g.FrameCount == 1 && (g.IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) && (g.ConfigFlagsLastFrame & ImGuiConfigFlags_ViewportsEnable) == 0)
        IM_ASSERT(0 && "Please set ViewportsEnable before the first call to NewFrame()! Otherwise you will lose your .ini settings!");

    // Perform simple checks: multi-viewport and platform windows support
    if (g.IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        if ((g.IO.BackendFlags & ImGuiBackendFlags_PlatformHasViewports) && (g.IO.BackendFlags & ImGuiBackendFlags_RendererHasViewports))
        {
            IM_ASSERT((g.FrameCount == 0 || g.FrameCount == g.FrameCountPlatformEnded) && "Forgot to call UpdatePlatformWindows() in main loop after EndFrame()? Check examples/ applications for reference.");
            IM_ASSERT(g.PlatformIO.Platform_CreateWindow  != NULL && "Platform init didn't install handlers?");
            IM_ASSERT(g.PlatformIO.Platform_DestroyWindow != NULL && "Platform init didn't install handlers?");
            IM_ASSERT(g.PlatformIO.Platform_GetWindowPos  != NULL && "Platform init didn't install handlers?");
            IM_ASSERT(g.PlatformIO.Platform_SetWindowPos  != NULL && "Platform init didn't install handlers?");
            IM_ASSERT(g.PlatformIO.Platform_GetWindowSize != NULL && "Platform init didn't install handlers?");
            IM_ASSERT(g.PlatformIO.Platform_SetWindowSize != NULL && "Platform init didn't install handlers?");
            IM_ASSERT(g.PlatformIO.Monitors.Size > 0 && "Platform init didn't setup Monitors list?");
            IM_ASSERT((g.Viewports[0]->PlatformUserData != NULL || g.Viewports[0]->PlatformHandle != NULL) && "Platform init didn't setup main viewport.");
            if (g.IO.ConfigDockingTransparentPayload && (g.IO.ConfigFlags & ImGuiConfigFlags_DockingEnable))
                IM_ASSERT(g.PlatformIO.Platform_SetWindowAlpha != NULL && "Platform_SetWindowAlpha handler is required to use io.ConfigDockingTransparent!");
        }
        else
        {
            // Disable feature, our backends do not support it
            g.IO.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
        }

        // Perform simple checks on platform monitor data + compute a total bounding box for quick early outs
        for (int monitor_n = 0; monitor_n < g.PlatformIO.Monitors.Size; monitor_n++)
        {
            ImGuiPlatformMonitor& mon = g.PlatformIO.Monitors[monitor_n];
            IM_UNUSED(mon);
            IM_ASSERT(mon.MainSize.x > 0.0f && mon.MainSize.y > 0.0f && "Monitor main bounds not setup properly.");
            //IM_ASSERT(ImRect(mon.MainPos, mon.MainPos + mon.MainSize).Contains(ImRect(mon.WorkPos, mon.WorkPos + mon.WorkSize)) && "Monitor work bounds not setup properly. If you don't have work area information, just copy MainPos/MainSize into them.");
            IM_ASSERT(mon.DpiScale != 0.0f);
        }
    }
}


void UpdateSettings()
{
    // Load settings on first frame (if not explicitly loaded manually before)
    ImGuiContext& g = *GImGui;
    if (!g.SettingsLoaded)
    {
        IM_ASSERT(g.SettingsWindows.empty());
        if (g.IO.IniFilename)
            LoadIniSettingsFromDisk(g.IO.IniFilename);
        g.SettingsLoaded = true;
    }

    // Save settings (with a delay after the last modification, so we don't spam disk too much)
    if (g.SettingsDirtyTimer > 0.0f)
    {
        g.SettingsDirtyTimer -= g.IO.DeltaTime;
        if (g.SettingsDirtyTimer <= 0.0f)
        {
            if (g.IO.IniFilename != NULL)
                SaveIniSettingsToDisk(g.IO.IniFilename);
            else
                g.IO.WantSaveIniSettings = true;  // Let user know they can call SaveIniSettingsToMemory(). user will need to clear io.WantSaveIniSettings themselves.
            g.SettingsDirtyTimer = 0.0f;
        }
    }
}

// [DEBUG] Item picker tool - start with DebugStartItemPicker() - useful to visually select an item and break into its call-stack.
void UpdateDebugToolItemPicker()
{
    ImGuiContext& g = *GImGui;
    g.DebugItemPickerBreakId = 0;
    if (!g.DebugItemPickerActive)
        return;

    const ImGuiID hovered_id = g.HoveredIdPreviousFrame;
    SetMouseCursor(ImGuiMouseCursor_Hand);
    if (IsKeyPressed(ImGuiKey_Escape))
        g.DebugItemPickerActive = false;
    if (IsMouseClicked(0) && hovered_id)
    {
        g.DebugItemPickerBreakId = hovered_id;
        g.DebugItemPickerActive = false;
    }
    SetNextWindowBgAlpha(0.60f);
    BeginTooltip();
    Text("HoveredId: 0x%08X", hovered_id);
    Text("Press ESC to abort picking.");
    TextColored(GetStyleColorVec4(hovered_id ? ImGuiCol_Text : ImGuiCol_TextDisabled), "Click to break in debugger!");
    EndTooltip();
}

// [DEBUG] Stack Tool: update queries. Called by NewFrame()
void UpdateDebugToolStackQueries()
{
    ImGuiContext& g = *GImGui;
    ImGuiStackTool* tool = &g.DebugStackTool;

    // Clear hook when stack tool is not visible
    g.DebugHookIdInfo = 0;
    if (g.FrameCount != tool->LastActiveFrame + 1)
        return;

    // Update queries. The steps are: -1: query Stack, >= 0: query each stack item
    // We can only perform 1 ID Info query every frame. This is designed so the GetID() tests are cheap and constant-time
    const ImGuiID query_id = g.HoveredIdPreviousFrame ? g.HoveredIdPreviousFrame : g.ActiveId;
    if (tool->QueryId != query_id)
    {
        tool->QueryId = query_id;
        tool->StackLevel = -1;
        tool->Results.resize(0);
    }
    if (query_id == 0)
        return;

    // Advance to next stack level when we got our result, or after 2 frames (in case we never get a result)
    int stack_level = tool->StackLevel;
    if (stack_level >= 0 && stack_level < tool->Results.Size)
        if (tool->Results[stack_level].QuerySuccess || tool->Results[stack_level].QueryFrameCount > 2)
            tool->StackLevel++;

    // Update hook
    stack_level = tool->StackLevel;
    if (stack_level == -1)
        g.DebugHookIdInfo = query_id;
    if (stack_level >= 0 && stack_level < tool->Results.Size)
    {
        g.DebugHookIdInfo = tool->Results[stack_level].ID;
        tool->Results[stack_level].QueryFrameCount++;
    }
}

static ImGuiWindow* FindWindowNavFocusable(int i_start, int i_stop, int dir) // FIXME-OPT O(N)
{
    ImGuiContext& g = *GImGui;
    for (int i = i_start; i >= 0 && i < g.WindowsFocusOrder.Size && i != i_stop; i += dir)
        if (IsWindowNavFocusable(g.WindowsFocusOrder[i]))
            return g.WindowsFocusOrder[i];
    return NULL;
}


void NavRestoreHighlightAfterMove()
{
    ImGuiContext& g = *GImGui;
    g.NavDisableHighlight = false;
    g.NavDisableMouseHover = g.NavMousePosDirty = true;
}


static int FindWindowFocusIndex(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    IM_UNUSED(g);
    int order = window->FocusOrder;
    IM_ASSERT(window->RootWindow == window); // No child window (not testing _ChildWindow because of docking)
    IM_ASSERT(g.WindowsFocusOrder[order] == window);
    return order;
}

static void NavUpdateWindowingHighlightWindow(int focus_change_dir)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.NavWindowingTarget);
    if (g.NavWindowingTarget->Flags & ImGuiWindowFlags_Modal)
        return;

    const int i_current = FindWindowFocusIndex(g.NavWindowingTarget);
    ImGuiWindow* window_target = FindWindowNavFocusable(i_current + focus_change_dir, -INT_MAX, focus_change_dir);
    if (!window_target)
        window_target = FindWindowNavFocusable((focus_change_dir < 0) ? (g.WindowsFocusOrder.Size - 1) : 0, i_current, focus_change_dir);
    if (window_target) // Don't reset windowing target if there's a single window in the list
        g.NavWindowingTarget = g.NavWindowingTargetAnim = window_target;
    g.NavWindowingToggleLayer = false;
}


// FIXME: This could be replaced by updating a frame number in each window when (window == NavWindow) and (NavLayer == 0).
// This way we could find the last focused window among our children. It would be much less confusing this way?
static void NavSaveLastChildNavWindowIntoParent(ImGuiWindow* nav_window)
{
    ImGuiWindow* parent = nav_window;
    while (parent && parent->RootWindow != parent && (parent->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) == 0)
        parent = parent->ParentWindow;
    if (parent && parent != nav_window)
        parent->NavLastChildNavWindow = nav_window;
}

// Handle PageUp/PageDown/Home/End keys
// Called from NavUpdateCreateMoveRequest() which will use our output to create a move request
// FIXME-NAV: This doesn't work properly with NavFlattened siblings as we use NavWindow rectangle for reference
// FIXME-NAV: how to get Home/End to aim at the beginning/end of a 2D grid?
static float NavUpdatePageUpPageDown()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.NavWindow;
    if ((window->Flags & ImGuiWindowFlags_NoNavInputs) || g.NavWindowingTarget != NULL)
        return 0.0f;

    const bool page_up_held = IsKeyDown(ImGuiKey_PageUp) && !IsActiveIdUsingKey(ImGuiKey_PageUp);
    const bool page_down_held = IsKeyDown(ImGuiKey_PageDown) && !IsActiveIdUsingKey(ImGuiKey_PageDown);
    const bool home_pressed = IsKeyPressed(ImGuiKey_Home) && !IsActiveIdUsingKey(ImGuiKey_Home);
    const bool end_pressed = IsKeyPressed(ImGuiKey_End) && !IsActiveIdUsingKey(ImGuiKey_End);
    if (page_up_held == page_down_held && home_pressed == end_pressed) // Proceed if either (not both) are pressed, otherwise early out
        return 0.0f;

    if (g.NavLayer != ImGuiNavLayer_Main)
        NavRestoreLayer(ImGuiNavLayer_Main);

    if (window->DC.NavLayersActiveMask == 0x00 && window->DC.NavHasScroll)
    {
        // Fallback manual-scroll when window has no navigable item
        if (IsKeyPressed(ImGuiKey_PageUp, true))
            SetScrollY(window, window->Scroll.y - window->InnerRect.GetHeight());
        else if (IsKeyPressed(ImGuiKey_PageDown, true))
            SetScrollY(window, window->Scroll.y + window->InnerRect.GetHeight());
        else if (home_pressed)
            SetScrollY(window, 0.0f);
        else if (end_pressed)
            SetScrollY(window, window->ScrollMax.y);
    }
    else
    {
        ImRect& nav_rect_rel = window->NavRectRel[g.NavLayer];
        const float page_offset_y = ImMax(0.0f, window->InnerRect.GetHeight() - window->CalcFontSize() * 1.0f + nav_rect_rel.GetHeight());
        float nav_scoring_rect_offset_y = 0.0f;
        if (IsKeyPressed(ImGuiKey_PageUp, true))
        {
            nav_scoring_rect_offset_y = -page_offset_y;
            g.NavMoveDir = ImGuiDir_Down; // Because our scoring rect is offset up, we request the down direction (so we can always land on the last item)
            g.NavMoveClipDir = ImGuiDir_Up;
            g.NavMoveFlags = ImGuiNavMoveFlags_AllowCurrentNavId | ImGuiNavMoveFlags_AlsoScoreVisibleSet;
        }
        else if (IsKeyPressed(ImGuiKey_PageDown, true))
        {
            nav_scoring_rect_offset_y = +page_offset_y;
            g.NavMoveDir = ImGuiDir_Up; // Because our scoring rect is offset down, we request the up direction (so we can always land on the last item)
            g.NavMoveClipDir = ImGuiDir_Down;
            g.NavMoveFlags = ImGuiNavMoveFlags_AllowCurrentNavId | ImGuiNavMoveFlags_AlsoScoreVisibleSet;
        }
        else if (home_pressed)
        {
            // FIXME-NAV: handling of Home/End is assuming that the top/bottom most item will be visible with Scroll.y == 0/ScrollMax.y
            // Scrolling will be handled via the ImGuiNavMoveFlags_ScrollToEdgeY flag, we don't scroll immediately to avoid scrolling happening before nav result.
            // Preserve current horizontal position if we have any.
            nav_rect_rel.Min.y = nav_rect_rel.Max.y = 0.0f;
            if (nav_rect_rel.IsInverted())
                nav_rect_rel.Min.x = nav_rect_rel.Max.x = 0.0f;
            g.NavMoveDir = ImGuiDir_Down;
            g.NavMoveFlags = ImGuiNavMoveFlags_AllowCurrentNavId | ImGuiNavMoveFlags_ScrollToEdgeY;
            // FIXME-NAV: MoveClipDir left to _None, intentional?
        }
        else if (end_pressed)
        {
            nav_rect_rel.Min.y = nav_rect_rel.Max.y = window->ContentSize.y;
            if (nav_rect_rel.IsInverted())
                nav_rect_rel.Min.x = nav_rect_rel.Max.x = 0.0f;
            g.NavMoveDir = ImGuiDir_Up;
            g.NavMoveFlags = ImGuiNavMoveFlags_AllowCurrentNavId | ImGuiNavMoveFlags_ScrollToEdgeY;
            // FIXME-NAV: MoveClipDir left to _None, intentional?
        }
        return nav_scoring_rect_offset_y;
    }
    return 0.0f;
}


static float CalcScrollEdgeSnap(float target, float snap_min, float snap_max, float snap_threshold, float center_ratio)
{
    if (target <= snap_min + snap_threshold)
        return ImLerp(snap_min, target, center_ratio);
    if (target >= snap_max - snap_threshold)
        return ImLerp(target, snap_max, center_ratio);
    return target;
}

static ImVec2 CalcNextScrollFromScrollTargetAndClamp(ImGuiWindow* window)
{
    ImVec2 scroll = window->Scroll;
    if (window->ScrollTarget.x < FLT_MAX)
    {
        float decoration_total_width = window->ScrollbarSizes.x;
        float center_x_ratio = window->ScrollTargetCenterRatio.x;
        float scroll_target_x = window->ScrollTarget.x;
        if (window->ScrollTargetEdgeSnapDist.x > 0.0f)
        {
            float snap_x_min = 0.0f;
            float snap_x_max = window->ScrollMax.x + window->SizeFull.x - decoration_total_width;
            scroll_target_x = CalcScrollEdgeSnap(scroll_target_x, snap_x_min, snap_x_max, window->ScrollTargetEdgeSnapDist.x, center_x_ratio);
        }
        scroll.x = scroll_target_x - center_x_ratio * (window->SizeFull.x - decoration_total_width);
    }
    if (window->ScrollTarget.y < FLT_MAX)
    {
        float decoration_total_height = window->TitleBarHeight() + window->MenuBarHeight() + window->ScrollbarSizes.y;
        float center_y_ratio = window->ScrollTargetCenterRatio.y;
        float scroll_target_y = window->ScrollTarget.y;
        if (window->ScrollTargetEdgeSnapDist.y > 0.0f)
        {
            float snap_y_min = 0.0f;
            float snap_y_max = window->ScrollMax.y + window->SizeFull.y - decoration_total_height;
            scroll_target_y = CalcScrollEdgeSnap(scroll_target_y, snap_y_min, snap_y_max, window->ScrollTargetEdgeSnapDist.y, center_y_ratio);
        }
        scroll.y = scroll_target_y - center_y_ratio * (window->SizeFull.y - decoration_total_height);
    }
    scroll.x = IM_FLOOR(ImMax(scroll.x, 0.0f));
    scroll.y = IM_FLOOR(ImMax(scroll.y, 0.0f));
    if (!window->Collapsed && !window->SkipItems)
    {
        scroll.x = ImMin(scroll.x, window->ScrollMax.x);
        scroll.y = ImMin(scroll.y, window->ScrollMax.y);
    }
    return scroll;
}

static ImVec2 NavCalcPreferredRefPos()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.NavWindow;
    if (g.NavDisableHighlight || !g.NavDisableMouseHover || !window)
    {
        // Mouse (we need a fallback in case the mouse becomes invalid after being used)
        // The +1.0f offset when stored by OpenPopupEx() allows reopening this or another popup (same or another mouse button) while not moving the mouse, it is pretty standard.
        // In theory we could move that +1.0f offset in OpenPopupEx()
        ImVec2 p = IsMousePosValid(&g.IO.MousePos) ? g.IO.MousePos : g.MouseLastValidPos;
        return ImVec2(p.x + 1.0f, p.y);
    }
    else
    {
        // When navigation is active and mouse is disabled, pick a position around the bottom left of the currently navigated item
        // Take account of upcoming scrolling (maybe set mouse pos should be done in EndFrame?)
        ImRect rect_rel = WindowRectRelToAbs(window, window->NavRectRel[g.NavLayer]);
        if (window->LastFrameActive != g.FrameCount && (window->ScrollTarget.x != FLT_MAX || window->ScrollTarget.y != FLT_MAX))
        {
            ImVec2 next_scroll = CalcNextScrollFromScrollTargetAndClamp(window);
            // TODO rect_rel.Translate(window->Scroll - next_scroll);

            rect_rel.Translate(window->Scroll);
        }
        ImVec2 pos = ImVec2(rect_rel.Min.x + ImMin(g.Style.FramePadding.x * 4, rect_rel.GetWidth()), rect_rel.Max.y - ImMin(g.Style.FramePadding.y, rect_rel.GetHeight()));
        ImGuiViewport* viewport = window->Viewport;
		// TODO         return ImFloor(ImClamp(pos, viewport->Pos, viewport->Pos + viewport->Size)); // ImFloor() is important because non-integer mouse position application in backend might be lossy and result in undesirable non-zero delta.

        return ImFloor(ImClamp(pos, viewport->Pos, viewport->Pos)); // ImFloor() is important because non-integer mouse position application in backend might be lossy and result in undesirable non-zero delta.
    }
}

void NavUpdateCreateMoveRequest()
{
    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    ImGuiWindow* window = g.NavWindow;

    if (g.NavMoveForwardToNextFrame && window != NULL)
    {
        // Forwarding previous request (which has been modified, e.g. wrap around menus rewrite the requests with a starting rectangle at the other side of the window)
        // (preserve most state, which were already set by the NavMoveRequestForward() function)
        IM_ASSERT(g.NavMoveDir != ImGuiDir_None && g.NavMoveClipDir != ImGuiDir_None);
        IM_ASSERT(g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded);
        IMGUI_DEBUG_LOG_NAV("[nav] NavMoveRequestForward %d\n", g.NavMoveDir);
    }
    else
    {
        // Initiate directional inputs request
        g.NavMoveDir = ImGuiDir_None;
        g.NavMoveFlags = ImGuiNavMoveFlags_None;
        g.NavMoveScrollFlags = ImGuiScrollFlags_None;
        if (window && !g.NavWindowingTarget && !(window->Flags & ImGuiWindowFlags_NoNavInputs))
        {
            const ImGuiInputReadMode read_mode = ImGuiInputReadMode_Repeat;
            if (!IsActiveIdUsingNavDir(ImGuiDir_Left)  && (IsNavInputTest(ImGuiNavInput_DpadLeft,  read_mode) || IsNavInputTest(ImGuiNavInput_KeyLeft_,  read_mode))) { g.NavMoveDir = ImGuiDir_Left; }
            if (!IsActiveIdUsingNavDir(ImGuiDir_Right) && (IsNavInputTest(ImGuiNavInput_DpadRight, read_mode) || IsNavInputTest(ImGuiNavInput_KeyRight_, read_mode))) { g.NavMoveDir = ImGuiDir_Right; }
            if (!IsActiveIdUsingNavDir(ImGuiDir_Up)    && (IsNavInputTest(ImGuiNavInput_DpadUp,    read_mode) || IsNavInputTest(ImGuiNavInput_KeyUp_,    read_mode))) { g.NavMoveDir = ImGuiDir_Up; }
            if (!IsActiveIdUsingNavDir(ImGuiDir_Down)  && (IsNavInputTest(ImGuiNavInput_DpadDown,  read_mode) || IsNavInputTest(ImGuiNavInput_KeyDown_,  read_mode))) { g.NavMoveDir = ImGuiDir_Down; }
        }
        g.NavMoveClipDir = g.NavMoveDir;
        g.NavScoringNoClipRect = ImRect(+FLT_MAX, +FLT_MAX, -FLT_MAX, -FLT_MAX);
    }

    // Update PageUp/PageDown/Home/End scroll
    // FIXME-NAV: Consider enabling those keys even without the master ImGuiConfigFlags_NavEnableKeyboard flag?
    const bool nav_keyboard_active = (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) != 0;
    float scoring_rect_offset_y = 0.0f;
    if (window && g.NavMoveDir == ImGuiDir_None && nav_keyboard_active)
        scoring_rect_offset_y = NavUpdatePageUpPageDown();
    if (scoring_rect_offset_y != 0.0f)
    {
        g.NavScoringNoClipRect = window->InnerRect;
        g.NavScoringNoClipRect.TranslateY(scoring_rect_offset_y);
    }

    // [DEBUG] Always send a request
#if IMGUI_DEBUG_NAV_SCORING
    if (io.KeyCtrl && IsKeyPressed(ImGuiKey_C))
        g.NavMoveDirForDebug = (ImGuiDir)((g.NavMoveDirForDebug + 1) & 3);
    if (io.KeyCtrl && g.NavMoveDir == ImGuiDir_None)
    {
        g.NavMoveDir = g.NavMoveDirForDebug;
        g.NavMoveFlags |= ImGuiNavMoveFlags_DebugNoResult;
    }
#endif

    // Submit
    g.NavMoveForwardToNextFrame = false;
    if (g.NavMoveDir != ImGuiDir_None)
        NavMoveRequestSubmit(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags, g.NavMoveScrollFlags);

    // Moving with no reference triggers a init request (will be used as a fallback if the direction fails to find a match)
    if (g.NavMoveSubmitted && g.NavId == 0)
    {
        IMGUI_DEBUG_LOG_NAV("[nav] NavInitRequest: from move, window \"%s\", layer=%d\n", g.NavWindow->Name, g.NavLayer);
        g.NavInitRequest = g.NavInitRequestFromMove = true;
        g.NavInitResultId = 0;
        g.NavDisableHighlight = false;
    }

    // When using gamepad, we project the reference nav bounding box into window visible area.
    // This is to allow resuming navigation inside the visible area after doing a large amount of scrolling, since with gamepad every movements are relative
    // (can't focus a visible object like we can with the mouse).
    if (g.NavMoveSubmitted && g.NavInputSource == ImGuiInputSource_Gamepad && g.NavLayer == ImGuiNavLayer_Main && window != NULL)// && (g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded))
    {
        bool clamp_x = (g.NavMoveFlags & (ImGuiNavMoveFlags_LoopX | ImGuiNavMoveFlags_WrapX)) == 0;
        bool clamp_y = (g.NavMoveFlags & (ImGuiNavMoveFlags_LoopY | ImGuiNavMoveFlags_WrapY)) == 0;

        //TODO -> ImRect inner_rect_rel = WindowRectAbsToRel(window, ImRect(window->InnerRect.Min - ImVec2(1, 1), window->InnerRect.Max + ImVec2(1, 1)));
        ImRect inner_rect_rel = WindowRectAbsToRel(window, ImRect());
        if ((clamp_x || clamp_y) && !inner_rect_rel.Contains(window->NavRectRel[g.NavLayer]))
        {
            IMGUI_DEBUG_LOG_NAV("[nav] NavMoveRequest: clamp NavRectRel for gamepad move\n");
            float pad_x = ImMin(inner_rect_rel.GetWidth(), window->CalcFontSize() * 0.5f);
            float pad_y = ImMin(inner_rect_rel.GetHeight(), window->CalcFontSize() * 0.5f); // Terrible approximation for the intent of starting navigation from first fully visible item
            inner_rect_rel.Min.x = clamp_x ? (inner_rect_rel.Min.x + pad_x) : -FLT_MAX;
            inner_rect_rel.Max.x = clamp_x ? (inner_rect_rel.Max.x - pad_x) : +FLT_MAX;
            inner_rect_rel.Min.y = clamp_y ? (inner_rect_rel.Min.y + pad_y) : -FLT_MAX;
            inner_rect_rel.Max.y = clamp_y ? (inner_rect_rel.Max.y - pad_y) : +FLT_MAX;
            window->NavRectRel[g.NavLayer].ClipWithFull(inner_rect_rel);
            g.NavId = g.NavFocusScopeId = 0;
        }
    }

    // For scoring we use a single segment on the left side our current item bounding box (not touching the edge to avoid box overlap with zero-spaced items)
    ImRect scoring_rect;
    if (window != NULL)
    {
        ImRect nav_rect_rel = !window->NavRectRel[g.NavLayer].IsInverted() ? window->NavRectRel[g.NavLayer] : ImRect(0, 0, 0, 0);
        scoring_rect = WindowRectRelToAbs(window, nav_rect_rel);
        scoring_rect.TranslateY(scoring_rect_offset_y);
        scoring_rect.Min.x = ImMin(scoring_rect.Min.x + 1.0f, scoring_rect.Max.x);
        scoring_rect.Max.x = scoring_rect.Min.x;
        IM_ASSERT(!scoring_rect.IsInverted()); // Ensure if we have a finite, non-inverted bounding box here will allows us to remove extraneous ImFabs() calls in NavScoreItem().
        //GetForegroundDrawList()->AddRect(scoring_rect.Min, scoring_rect.Max, IM_COL32(255,200,0,255)); // [DEBUG]
        //if (!g.NavScoringNoClipRect.IsInverted()) { GetForegroundDrawList()->AddRect(g.NavScoringNoClipRect.Min, g.NavScoringNoClipRect.Max, IM_COL32(255, 200, 0, 255)); } // [DEBUG]
    }
    g.NavScoringRect = scoring_rect;
    g.NavScoringNoClipRect.Add(scoring_rect);
}

void NavUpdateCreateTabbingRequest()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.NavWindow;
    IM_ASSERT(g.NavMoveDir == ImGuiDir_None);
    if (window == NULL || g.NavWindowingTarget != NULL || (window->Flags & ImGuiWindowFlags_NoNavInputs))
        return;

    const bool tab_pressed = IsKeyPressed(ImGuiKey_Tab, true) && !IsActiveIdUsingKey(ImGuiKey_Tab) && !g.IO.KeyCtrl && !g.IO.KeyAlt;
    if (!tab_pressed)
        return;

    // Initiate tabbing request
    // (this is ALWAYS ENABLED, regardless of ImGuiConfigFlags_NavEnableKeyboard flag!)
    // Initially this was designed to use counters and modulo arithmetic, but that could not work with unsubmitted items (list clipper). Instead we use a strategy close to other move requests.
    // See NavProcessItemForTabbingRequest() for a description of the various forward/backward tabbing cases with and without wrapping.
    //// FIXME: We use (g.ActiveId == 0) but (g.NavDisableHighlight == false) might be righter once we can tab through anything
    g.NavTabbingDir = g.IO.KeyShift ? -1 : (g.ActiveId == 0) ? 0 : +1;
    ImGuiScrollFlags scroll_flags = window->Appearing ? ImGuiScrollFlags_KeepVisibleEdgeX | ImGuiScrollFlags_AlwaysCenterY : ImGuiScrollFlags_KeepVisibleEdgeX | ImGuiScrollFlags_KeepVisibleEdgeY;
    ImGuiDir clip_dir = (g.NavTabbingDir < 0) ? ImGuiDir_Up : ImGuiDir_Down;
    NavMoveRequestSubmit(ImGuiDir_None, clip_dir, ImGuiNavMoveFlags_Tabbing, scroll_flags); // FIXME-NAV: Once we refactor tabbing, add LegacyApi flag to not activate non-inputable.
    g.NavTabbingResultFirst.Clear();
    g.NavTabbingCounter = -1;
}



// Windowing management mode
// Keyboard: CTRL+Tab (change focus/move/resize), Alt (toggle menu layer)
// Gamepad:  Hold Menu/Square (change focus/move/resize), Tap Menu/Square (toggle menu layer)
static void NavUpdateWindowing()
{
    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;

    ImGuiWindow* apply_focus_window = NULL;
    bool apply_toggle_layer = false;

    ImGuiWindow* modal_window = GetTopMostPopupModal();
    bool allow_windowing = (modal_window == NULL);
    if (!allow_windowing)
        g.NavWindowingTarget = NULL;

    // Fade out
    if (g.NavWindowingTargetAnim && g.NavWindowingTarget == NULL)
    {
        g.NavWindowingHighlightAlpha = ImMax(g.NavWindowingHighlightAlpha - io.DeltaTime * 10.0f, 0.0f);
        if (g.DimBgRatio <= 0.0f && g.NavWindowingHighlightAlpha <= 0.0f)
            g.NavWindowingTargetAnim = NULL;
    }

    // Start CTRL+Tab or Square+L/R window selection
    const bool start_windowing_with_gamepad = allow_windowing && !g.NavWindowingTarget && IsNavInputTest(ImGuiNavInput_Menu, ImGuiInputReadMode_Pressed);
    const bool start_windowing_with_keyboard = allow_windowing && !g.NavWindowingTarget && io.KeyCtrl && IsKeyPressed(ImGuiKey_Tab);
    if (start_windowing_with_gamepad || start_windowing_with_keyboard)
        if (ImGuiWindow* window = g.NavWindow ? g.NavWindow : FindWindowNavFocusable(g.WindowsFocusOrder.Size - 1, -INT_MAX, -1))
        {
            g.NavWindowingTarget = g.NavWindowingTargetAnim = window->RootWindow;
            g.NavWindowingTimer = g.NavWindowingHighlightAlpha = 0.0f;
            g.NavWindowingToggleLayer = start_windowing_with_gamepad ? true : false; // Gamepad starts toggling layer
            g.NavInputSource = start_windowing_with_keyboard ? ImGuiInputSource_Keyboard : ImGuiInputSource_Gamepad;
        }

    // Gamepad update
    g.NavWindowingTimer += io.DeltaTime;
    if (g.NavWindowingTarget && g.NavInputSource == ImGuiInputSource_Gamepad)
    {
        // Highlight only appears after a brief time holding the button, so that a fast tap on PadMenu (to toggle NavLayer) doesn't add visual noise
        g.NavWindowingHighlightAlpha = ImMax(g.NavWindowingHighlightAlpha, ImSaturate((g.NavWindowingTimer - NAV_WINDOWING_HIGHLIGHT_DELAY) / 0.05f));

        // Select window to focus
        const int focus_change_dir = (int)IsNavInputTest(ImGuiNavInput_FocusPrev, ImGuiInputReadMode_RepeatSlow) - (int)IsNavInputTest(ImGuiNavInput_FocusNext, ImGuiInputReadMode_RepeatSlow);
        if (focus_change_dir != 0)
        {
            NavUpdateWindowingHighlightWindow(focus_change_dir);
            g.NavWindowingHighlightAlpha = 1.0f;
        }

        // Single press toggles NavLayer, long press with L/R apply actual focus on release (until then the window was merely rendered top-most)
        if (!IsNavInputDown(ImGuiNavInput_Menu))
        {
            g.NavWindowingToggleLayer &= (g.NavWindowingHighlightAlpha < 1.0f); // Once button was held long enough we don't consider it a tap-to-toggle-layer press anymore.
            if (g.NavWindowingToggleLayer && g.NavWindow)
                apply_toggle_layer = true;
            else if (!g.NavWindowingToggleLayer)
                apply_focus_window = g.NavWindowingTarget;
            g.NavWindowingTarget = NULL;
        }
    }

    // Keyboard: Focus
    if (g.NavWindowingTarget && g.NavInputSource == ImGuiInputSource_Keyboard)
    {
        // Visuals only appears after a brief time after pressing TAB the first time, so that a fast CTRL+TAB doesn't add visual noise
        g.NavWindowingHighlightAlpha = ImMax(g.NavWindowingHighlightAlpha, ImSaturate((g.NavWindowingTimer - NAV_WINDOWING_HIGHLIGHT_DELAY) / 0.05f)); // 1.0f
        if (IsKeyPressed(ImGuiKey_Tab, true))
            NavUpdateWindowingHighlightWindow(io.KeyShift ? +1 : -1);
        if (!io.KeyCtrl)
            apply_focus_window = g.NavWindowingTarget;
    }

    // Keyboard: Press and Release ALT to toggle menu layer
    // - Testing that only Alt is tested prevents Alt+Shift or AltGR from toggling menu layer.
    // - AltGR is normally Alt+Ctrl but we can't reliably detect it (not all backends/systems/layout emit it as Alt+Ctrl). But even on keyboards without AltGR we don't want Alt+Ctrl to open menu anyway.
	const bool nav_keyboard_active = (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) != 0;
    if (nav_keyboard_active && io.KeyMods == ImGuiKeyModFlags_Alt && (io.KeyModsPrev & ImGuiKeyModFlags_Alt) == 0)
    {
        g.NavWindowingToggleLayer = true;
        g.NavInputSource = ImGuiInputSource_Keyboard;
    }
    if (g.NavWindowingToggleLayer && g.NavInputSource == ImGuiInputSource_Keyboard)
    {
        // We cancel toggling nav layer when any text has been typed (generally while holding Alt). (See #370)
        // We cancel toggling nav layer when other modifiers are pressed. (See #4439)
        if (io.InputQueueCharacters.Size > 0 || io.KeyCtrl || io.KeyShift || io.KeySuper)
            g.NavWindowingToggleLayer = false;

        // Apply layer toggle on release
        // Important: we don't assume that Alt was previously held in order to handle loss of focus when backend calls io.AddFocusEvent(false)
        // Important: as before version <18314 we lacked an explicit IO event for focus gain/loss, we also compare mouse validity to detect old backends clearing mouse pos on focus loss.
        if (!(io.KeyMods & ImGuiKeyModFlags_Alt) && (io.KeyModsPrev & ImGuiKeyModFlags_Alt) && g.NavWindowingToggleLayer)
            if (g.ActiveId == 0 || g.ActiveIdAllowOverlap)
                if (IsMousePosValid(&io.MousePos) == IsMousePosValid(&io.MousePosPrev))
                    apply_toggle_layer = true;
        if (!io.KeyAlt)
            g.NavWindowingToggleLayer = false;
    }

    // Move window
    if (g.NavWindowingTarget && !(g.NavWindowingTarget->Flags & ImGuiWindowFlags_NoMove))
    {
        ImVec2 move_delta;
        if (g.NavInputSource == ImGuiInputSource_Keyboard && !io.KeyShift)
            move_delta = GetNavInputAmount2d(ImGuiNavDirSourceFlags_RawKeyboard, ImGuiInputReadMode_Down);
        if (g.NavInputSource == ImGuiInputSource_Gamepad)
            move_delta = GetNavInputAmount2d(ImGuiNavDirSourceFlags_PadLStick, ImGuiInputReadMode_Down);
        if (move_delta.x != 0.0f || move_delta.y != 0.0f)
        {
            const float NAV_MOVE_SPEED = 800.0f;
            const float move_speed = ImFloor(NAV_MOVE_SPEED * io.DeltaTime * ImMin(io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y)); // FIXME: Doesn't handle variable framerate very well
            ImGuiWindow* moving_window = g.NavWindowingTarget->RootWindowDockTree;
            //TODO SetWindowPos(moving_window, moving_window->Pos + move_delta * move_speed, ImGuiCond_Always);
            SetWindowPos(moving_window, moving_window->Pos, ImGuiCond_Always);
            MarkIniSettingsDirty(moving_window);
            g.NavDisableMouseHover = true;
        }
    }

    // Apply final focus
    if (apply_focus_window && (g.NavWindow == NULL || apply_focus_window != g.NavWindow->RootWindow))
    {
        ImGuiViewport* previous_viewport = g.NavWindow ? g.NavWindow->Viewport : NULL;
        ClearActiveID();
        NavRestoreHighlightAfterMove();
        apply_focus_window = NavRestoreLastChildNavWindow(apply_focus_window);
        ClosePopupsOverWindow(apply_focus_window, false);
        FocusWindow(apply_focus_window);
        if (apply_focus_window->NavLastIds[0] == 0)
            NavInitWindow(apply_focus_window, false);

        // If the window has ONLY a menu layer (no main layer), select it directly
        // Use NavLayersActiveMaskNext since windows didn't have a chance to be Begin()-ed on this frame,
        // so CTRL+Tab where the keys are only held for 1 frame will be able to use correct layers mask since
        // the target window as already been previewed once.
        // FIXME-NAV: This should be done in NavInit.. or in FocusWindow... However in both of those cases,
        // we won't have a guarantee that windows has been visible before and therefore NavLayersActiveMask*
        // won't be valid.
        if (apply_focus_window->DC.NavLayersActiveMaskNext == (1 << ImGuiNavLayer_Menu))
            g.NavLayer = ImGuiNavLayer_Menu;

        // Request OS level focus
        if (apply_focus_window->Viewport != previous_viewport && g.PlatformIO.Platform_SetWindowFocus)
            g.PlatformIO.Platform_SetWindowFocus(apply_focus_window->Viewport);
    }
    if (apply_focus_window)
        g.NavWindowingTarget = NULL;

    // Apply menu/layer toggle
    if (apply_toggle_layer && g.NavWindow)
    {
        ClearActiveID();

        // Move to parent menu if necessary
        ImGuiWindow* new_nav_window = g.NavWindow;
        while (new_nav_window->ParentWindow
            && (new_nav_window->DC.NavLayersActiveMask & (1 << ImGuiNavLayer_Menu)) == 0
            && (new_nav_window->Flags & ImGuiWindowFlags_ChildWindow) != 0
            && (new_nav_window->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_ChildMenu)) == 0)
            new_nav_window = new_nav_window->ParentWindow;
        if (new_nav_window != g.NavWindow)
        {
            ImGuiWindow* old_nav_window = g.NavWindow;
            FocusWindow(new_nav_window);
            new_nav_window->NavLastChildNavWindow = old_nav_window;
        }

        // Toggle layer
        const ImGuiNavLayer new_nav_layer = (g.NavWindow->DC.NavLayersActiveMask & (1 << ImGuiNavLayer_Menu)) ? (ImGuiNavLayer)((int)g.NavLayer ^ 1) : ImGuiNavLayer_Main;
        if (new_nav_layer != g.NavLayer)
        {
            // Reinitialize navigation when entering menu bar with the Alt key (FIXME: could be a properly of the layer?)
            const bool preserve_layer_1_nav_id = (new_nav_window->DockNodeAsHost != NULL);
            if (new_nav_layer == ImGuiNavLayer_Menu && !preserve_layer_1_nav_id)
                g.NavWindow->NavLastIds[new_nav_layer] = 0;
            NavRestoreLayer(new_nav_layer);
            NavRestoreHighlightAfterMove();
        }
    }
}


static inline void NavUpdateAnyRequestFlag()
{
    ImGuiContext& g = *GImGui;
    g.NavAnyRequest = g.NavMoveScoringItems || g.NavInitRequest || (IMGUI_DEBUG_NAV_SCORING && g.NavWindow != NULL);
    if (g.NavAnyRequest)
        IM_ASSERT(g.NavWindow != NULL);
}

// Process NavCancel input (to close a popup, get back to parent, clear focus)
// FIXME: In order to support e.g. Escape to clear a selection we'll need:
// - either to store the equivalent of ActiveIdUsingKeyInputMask for a FocusScope and test for it.
// - either to move most/all of those tests to the epilogue/end functions of the scope they are dealing with (e.g. exit child window in EndChild()) or in EndFrame(), to allow an earlier intercept
static void NavUpdateCancelRequest()
{
    ImGuiContext& g = *GImGui;
    if (!IsNavInputTest(ImGuiNavInput_Cancel, ImGuiInputReadMode_Pressed))
        return;

    IMGUI_DEBUG_LOG_NAV("[nav] ImGuiNavInput_Cancel\n");
    if (g.ActiveId != 0)
    {
        if (!IsActiveIdUsingNavInput(ImGuiNavInput_Cancel))
            ClearActiveID();
    }
    else if (g.NavLayer != ImGuiNavLayer_Main)
    {
        // Leave the "menu" layer
        NavRestoreLayer(ImGuiNavLayer_Main);
        NavRestoreHighlightAfterMove();
    }
    else if (g.NavWindow && g.NavWindow != g.NavWindow->RootWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_Popup) && g.NavWindow->ParentWindow)
    {
        // Exit child window
        ImGuiWindow* child_window = g.NavWindow;
        ImGuiWindow* parent_window = g.NavWindow->ParentWindow;
        IM_ASSERT(child_window->ChildId != 0);
        ImRect child_rect = child_window->Rect();
        FocusWindow(parent_window);
        SetNavID(child_window->ChildId, ImGuiNavLayer_Main, 0, WindowRectAbsToRel(parent_window, child_rect));
        NavRestoreHighlightAfterMove();
    }
    else if (g.OpenPopupStack.Size > 0)
    {
        // Close open popup/menu
        if (!(g.OpenPopupStack.back().Window->Flags & ImGuiWindowFlags_Modal))
            ClosePopupToLevel(g.OpenPopupStack.Size - 1, true);
    }
    else
    {
        // Clear NavLastId for popups but keep it for regular child window so we can leave one and come back where we were
        if (g.NavWindow && ((g.NavWindow->Flags & ImGuiWindowFlags_Popup) || !(g.NavWindow->Flags & ImGuiWindowFlags_ChildWindow)))
            g.NavWindow->NavLastIds[0] = 0;
        g.NavId = g.NavFocusScopeId = 0;
    }
}


static void NavUpdate()
{
    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;

    io.WantSetMousePos = false;
    //if (g.NavScoringDebugCount > 0) IMGUI_DEBUG_LOG("NavScoringDebugCount %d for '%s' layer %d (Init:%d, Move:%d)\n", g.NavScoringDebugCount, g.NavWindow ? g.NavWindow->Name : "NULL", g.NavLayer, g.NavInitRequest || g.NavInitResultId != 0, g.NavMoveRequest);

    // Update Gamepad->Nav inputs mapping
    // Set input source as Gamepad when buttons are pressed (as some features differs when used with Gamepad vs Keyboard)
    const bool nav_gamepad_active = (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) != 0 && (io.BackendFlags & ImGuiBackendFlags_HasGamepad) != 0;
    if (nav_gamepad_active && g.IO.BackendUsingLegacyNavInputArray == false)
    {
        for (int n = 0; n < ImGuiNavInput_COUNT; n++)
            IM_ASSERT(io.NavInputs[n] == 0.0f && "Backend needs to either only use io.AddKeyEvent()/io.AddKeyAnalogEvent(), either only fill legacy io.NavInputs[]. Not both!");
        #define NAV_MAP_KEY(_KEY, _NAV_INPUT, _ACTIVATE_NAV)  do { io.NavInputs[_NAV_INPUT] = io.KeysData[_KEY - ImGuiKey_KeysData_OFFSET].AnalogValue; if (_ACTIVATE_NAV && io.NavInputs[_NAV_INPUT] > 0.0f) { g.NavInputSource = ImGuiInputSource_Gamepad; } } while (0)
        NAV_MAP_KEY(ImGuiKey_GamepadFaceDown, ImGuiNavInput_Activate, true);
        NAV_MAP_KEY(ImGuiKey_GamepadFaceRight, ImGuiNavInput_Cancel, true);
        NAV_MAP_KEY(ImGuiKey_GamepadFaceLeft, ImGuiNavInput_Menu, true);
        NAV_MAP_KEY(ImGuiKey_GamepadFaceUp, ImGuiNavInput_Input, true);
        NAV_MAP_KEY(ImGuiKey_GamepadDpadLeft, ImGuiNavInput_DpadLeft, true);
        NAV_MAP_KEY(ImGuiKey_GamepadDpadRight, ImGuiNavInput_DpadRight, true);
        NAV_MAP_KEY(ImGuiKey_GamepadDpadUp, ImGuiNavInput_DpadUp, true);
        NAV_MAP_KEY(ImGuiKey_GamepadDpadDown, ImGuiNavInput_DpadDown, true);
        NAV_MAP_KEY(ImGuiKey_GamepadL1, ImGuiNavInput_FocusPrev, false);
        NAV_MAP_KEY(ImGuiKey_GamepadR1, ImGuiNavInput_FocusNext, false);
        NAV_MAP_KEY(ImGuiKey_GamepadL1, ImGuiNavInput_TweakSlow, false);
        NAV_MAP_KEY(ImGuiKey_GamepadR1, ImGuiNavInput_TweakFast, false);
        NAV_MAP_KEY(ImGuiKey_GamepadLStickLeft, ImGuiNavInput_LStickLeft, false);
        NAV_MAP_KEY(ImGuiKey_GamepadLStickRight, ImGuiNavInput_LStickRight, false);
        NAV_MAP_KEY(ImGuiKey_GamepadLStickUp, ImGuiNavInput_LStickUp, false);
        NAV_MAP_KEY(ImGuiKey_GamepadLStickDown, ImGuiNavInput_LStickDown, false);
        #undef NAV_MAP_KEY
    }

    // Update Keyboard->Nav inputs mapping
    const bool nav_keyboard_active = (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) != 0;
    if (nav_keyboard_active)
    {
        #define NAV_MAP_KEY(_KEY, _NAV_INPUT)  do { if (IsKeyDown(_KEY)) { io.NavInputs[_NAV_INPUT] = 1.0f; g.NavInputSource = ImGuiInputSource_Keyboard; } } while (0)
        NAV_MAP_KEY(ImGuiKey_Space,     ImGuiNavInput_Activate );
        NAV_MAP_KEY(ImGuiKey_Enter,     ImGuiNavInput_Input    );
        NAV_MAP_KEY(ImGuiKey_Escape,    ImGuiNavInput_Cancel   );
        NAV_MAP_KEY(ImGuiKey_LeftArrow, ImGuiNavInput_KeyLeft_ );
        NAV_MAP_KEY(ImGuiKey_RightArrow,ImGuiNavInput_KeyRight_);
        NAV_MAP_KEY(ImGuiKey_UpArrow,   ImGuiNavInput_KeyUp_   );
        NAV_MAP_KEY(ImGuiKey_DownArrow, ImGuiNavInput_KeyDown_ );
        if (io.KeyCtrl)
            io.NavInputs[ImGuiNavInput_TweakSlow] = 1.0f;
        if (io.KeyShift)
            io.NavInputs[ImGuiNavInput_TweakFast] = 1.0f;
        #undef NAV_MAP_KEY
    }
    memcpy(io.NavInputsDownDurationPrev, io.NavInputsDownDuration, sizeof(io.NavInputsDownDuration));
    for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++)
        io.NavInputsDownDuration[i] = (io.NavInputs[i] > 0.0f) ? (io.NavInputsDownDuration[i] < 0.0f ? 0.0f : io.NavInputsDownDuration[i] + io.DeltaTime) : -1.0f;

    // Process navigation init request (select first/default focus)
    if (g.NavInitResultId != 0)
        NavInitRequestApplyResult();
    g.NavInitRequest = false;
    g.NavInitRequestFromMove = false;
    g.NavInitResultId = 0;
    g.NavJustMovedToId = 0;

    // Process navigation move request
    if (g.NavMoveSubmitted)
        NavMoveRequestApplyResult();
    g.NavTabbingCounter = 0;
    g.NavMoveSubmitted = g.NavMoveScoringItems = false;

    // Schedule mouse position update (will be done at the bottom of this function, after 1) processing all move requests and 2) updating scrolling)
    bool set_mouse_pos = false;
    if (g.NavMousePosDirty && g.NavIdIsAlive)
        if (!g.NavDisableHighlight && g.NavDisableMouseHover && g.NavWindow)
            set_mouse_pos = true;
    g.NavMousePosDirty = false;
    IM_ASSERT(g.NavLayer == ImGuiNavLayer_Main || g.NavLayer == ImGuiNavLayer_Menu);

    // Store our return window (for returning from Menu Layer to Main Layer) and clear it as soon as we step back in our own Layer 0
    if (g.NavWindow)
        NavSaveLastChildNavWindowIntoParent(g.NavWindow);
    if (g.NavWindow && g.NavWindow->NavLastChildNavWindow != NULL && g.NavLayer == ImGuiNavLayer_Main)
        g.NavWindow->NavLastChildNavWindow = NULL;

    // Update CTRL+TAB and Windowing features (hold Square to move/resize/etc.)
    NavUpdateWindowing();

    // Set output flags for user application
    io.NavActive = (nav_keyboard_active || nav_gamepad_active) && g.NavWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs);
    io.NavVisible = (io.NavActive && g.NavId != 0 && !g.NavDisableHighlight) || (g.NavWindowingTarget != NULL);

    // Process NavCancel input (to close a popup, get back to parent, clear focus)
    NavUpdateCancelRequest();

    // Process manual activation request
    g.NavActivateId = g.NavActivateDownId = g.NavActivatePressedId = g.NavActivateInputId = 0;
    g.NavActivateFlags = ImGuiActivateFlags_None;
    if (g.NavId != 0 && !g.NavDisableHighlight && !g.NavWindowingTarget && g.NavWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs))
    {
        bool activate_down = IsNavInputDown(ImGuiNavInput_Activate);
        bool input_down = IsNavInputDown(ImGuiNavInput_Input);
        bool activate_pressed = activate_down && IsNavInputTest(ImGuiNavInput_Activate, ImGuiInputReadMode_Pressed);
        bool input_pressed = input_down && IsNavInputTest(ImGuiNavInput_Input, ImGuiInputReadMode_Pressed);
        if (g.ActiveId == 0 && activate_pressed)
        {
            g.NavActivateId = g.NavId;
            g.NavActivateFlags = ImGuiActivateFlags_PreferTweak;
        }
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId) && input_pressed)
        {
            g.NavActivateInputId = g.NavId;
            g.NavActivateFlags = ImGuiActivateFlags_PreferInput;
        }
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId) && activate_down)
            g.NavActivateDownId = g.NavId;
        if ((g.ActiveId == 0 || g.ActiveId == g.NavId) && activate_pressed)
            g.NavActivatePressedId = g.NavId;
    }
    if (g.NavWindow && (g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs))
        g.NavDisableHighlight = true;
    if (g.NavActivateId != 0)
        IM_ASSERT(g.NavActivateDownId == g.NavActivateId);

    // Process programmatic activation request
    // FIXME-NAV: Those should eventually be queued (unlike focus they don't cancel each others)
    if (g.NavNextActivateId != 0)
    {
        if (g.NavNextActivateFlags & ImGuiActivateFlags_PreferInput)
            g.NavActivateInputId = g.NavNextActivateId;
        else
            g.NavActivateId = g.NavActivateDownId = g.NavActivatePressedId = g.NavNextActivateId;
        g.NavActivateFlags = g.NavNextActivateFlags;
    }
    g.NavNextActivateId = 0;

    // Process move requests
    NavUpdateCreateMoveRequest();
    if (g.NavMoveDir == ImGuiDir_None)
        NavUpdateCreateTabbingRequest();
    NavUpdateAnyRequestFlag();
    g.NavIdIsAlive = false;

    // Scrolling
    if (g.NavWindow && !(g.NavWindow->Flags & ImGuiWindowFlags_NoNavInputs) && !g.NavWindowingTarget)
    {
        // *Fallback* manual-scroll with Nav directional keys when window has no navigable item
        ImGuiWindow* window = g.NavWindow;
        const float scroll_speed = IM_ROUND(window->CalcFontSize() * 100 * io.DeltaTime); // We need round the scrolling speed because sub-pixel scroll isn't reliably supported.
        const ImGuiDir move_dir = g.NavMoveDir;
        if (window->DC.NavLayersActiveMask == 0x00 && window->DC.NavHasScroll && move_dir != ImGuiDir_None)
        {
            if (move_dir == ImGuiDir_Left || move_dir == ImGuiDir_Right)
                SetScrollX(window, ImFloor(window->Scroll.x + ((move_dir == ImGuiDir_Left) ? -1.0f : +1.0f) * scroll_speed));
            if (move_dir == ImGuiDir_Up || move_dir == ImGuiDir_Down)
                SetScrollY(window, ImFloor(window->Scroll.y + ((move_dir == ImGuiDir_Up) ? -1.0f : +1.0f) * scroll_speed));
        }

        // *Normal* Manual scroll with NavScrollXXX keys
        // Next movement request will clamp the NavId reference rectangle to the visible area, so navigation will resume within those bounds.
        ImVec2 scroll_dir = GetNavInputAmount2d(ImGuiNavDirSourceFlags_PadLStick, ImGuiInputReadMode_Down, 1.0f / 10.0f, 10.0f);
        if (scroll_dir.x != 0.0f && window->ScrollbarX)
            SetScrollX(window, ImFloor(window->Scroll.x + scroll_dir.x * scroll_speed));
        if (scroll_dir.y != 0.0f)
            SetScrollY(window, ImFloor(window->Scroll.y + scroll_dir.y * scroll_speed));
    }

    // Always prioritize mouse highlight if navigation is disabled
    if (!nav_keyboard_active && !nav_gamepad_active)
    {
        g.NavDisableHighlight = true;
        g.NavDisableMouseHover = set_mouse_pos = false;
    }

    // Update mouse position if requested
    // (This will take into account the possibility that a Scroll was queued in the window to offset our absolute mouse position before scroll has been applied)
    if (set_mouse_pos && (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos) && (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos))
    {
        io.MousePos = io.MousePosPrev = NavCalcPreferredRefPos();
        io.WantSetMousePos = true;
        //IMGUI_DEBUG_LOG("SetMousePos: (%.1f,%.1f)\n", io.MousePos.x, io.MousePos.y);
    }

    // [DEBUG]
    g.NavScoringDebugCount = 0;
#if IMGUI_DEBUG_NAV_RECTS
    if (g.NavWindow)
    {
        ImDrawList* draw_list = GetForegroundDrawList(g.NavWindow);
        if (1) { for (int layer = 0; layer < 2; layer++) { ImRect r = WindowRectRelToAbs(g.NavWindow, g.NavWindow->NavRectRel[layer]); draw_list->AddRect(r.Min, r.Max, IM_COL32(255,200,0,255)); } } // [DEBUG]
        if (1) { ImU32 col = (!g.NavWindow->Hidden) ? IM_COL32(255,0,255,255) : IM_COL32(255,0,0,255); ImVec2 p = NavCalcPreferredRefPos(); char buf[32]; ImFormatString(buf, 32, "%d", g.NavLayer); draw_list->AddCircleFilled(p, 3.0f, col); draw_list->AddText(NULL, 13.0f, p + ImVec2(8,-4), col, buf); }
    }
#endif
}


static void StartLockWheelingWindow(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    if (g.WheelingWindow == window)
        return;
    g.WheelingWindow = window;
    g.WheelingWindowRefMousePos = g.IO.MousePos;
    g.WheelingWindowTimer = WINDOWS_MOUSE_WHEEL_SCROLL_LOCK_TIMER;
}

void UpdateMouseWheel()
{
    ImGuiContext& g = *GImGui;

    // Reset the locked window if we move the mouse or after the timer elapses
    if (g.WheelingWindow != NULL)
    {
        g.WheelingWindowTimer -= g.IO.DeltaTime;
        //TODO if (IsMousePosValid() && ImLengthSqr(g.IO.MousePos - g.WheelingWindowRefMousePos) > g.IO.MouseDragThreshold * g.IO.MouseDragThreshold)
        if (IsMousePosValid() && ImLengthSqr(g.IO.MousePos) > g.IO.MouseDragThreshold * g.IO.MouseDragThreshold)
            g.WheelingWindowTimer = 0.0f;
        if (g.WheelingWindowTimer <= 0.0f)
        {
            g.WheelingWindow = NULL;
            g.WheelingWindowTimer = 0.0f;
        }
    }

    if (g.IO.MouseWheel == 0.0f && g.IO.MouseWheelH == 0.0f)
        return;

    if ((g.ActiveId != 0 && g.ActiveIdUsingMouseWheel) || (g.HoveredIdPreviousFrame != 0 && g.HoveredIdPreviousFrameUsingMouseWheel))
        return;

    ImGuiWindow* window = g.WheelingWindow ? g.WheelingWindow : g.HoveredWindow;
    if (!window || window->Collapsed)
        return;

    // Zoom / Scale window
    // FIXME-OBSOLETE: This is an old feature, it still works but pretty much nobody is using it and may be best redesigned.
    if (g.IO.MouseWheel != 0.0f && g.IO.KeyCtrl && g.IO.FontAllowUserScaling)
    {
        StartLockWheelingWindow(window);
        const float new_font_scale = ImClamp(window->FontWindowScale + g.IO.MouseWheel * 0.10f, 0.50f, 2.50f);
        const float scale = new_font_scale / window->FontWindowScale;
        window->FontWindowScale = new_font_scale;
        if (window == window->RootWindow)
        {
           /*TODO const ImVec2 offset = window->Size * (1.0f - scale) * (g.IO.MousePos - window->Pos) / window->Size;
            SetWindowPos(window, window->Pos + offset, 0);
            window->Size = ImFloor(window->Size * scale);
            window->SizeFull = ImFloor(window->SizeFull * scale);*/
        }
        return;
    }

    // Mouse wheel scrolling
    // If a child window has the ImGuiWindowFlags_NoScrollWithMouse flag, we give a chance to scroll its parent
    if (g.IO.KeyCtrl)
        return;

    // As a standard behavior holding SHIFT while using Vertical Mouse Wheel triggers Horizontal scroll instead
    // (we avoid doing it on OSX as it the OS input layer handles this already)
    const bool swap_axis = g.IO.KeyShift && !g.IO.ConfigMacOSXBehaviors;
    const float wheel_y = swap_axis ? 0.0f : g.IO.MouseWheel;
    const float wheel_x = swap_axis ? g.IO.MouseWheel : g.IO.MouseWheelH;

    // Vertical Mouse Wheel scrolling
    if (wheel_y != 0.0f)
    {
        StartLockWheelingWindow(window);
        while ((window->Flags & ImGuiWindowFlags_ChildWindow) && ((window->ScrollMax.y == 0.0f) || ((window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(window->Flags & ImGuiWindowFlags_NoMouseInputs))))
            window = window->ParentWindow;
        if (!(window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(window->Flags & ImGuiWindowFlags_NoMouseInputs))
        {
            float max_step = window->InnerRect.GetHeight() * 0.67f;
            float scroll_step = ImFloor(ImMin(5 * window->CalcFontSize(), max_step));
            SetScrollY(window, window->Scroll.y - wheel_y * scroll_step);
        }
    }

    // Horizontal Mouse Wheel scrolling, or Vertical Mouse Wheel w/ Shift held
    if (wheel_x != 0.0f)
    {
        StartLockWheelingWindow(window);
        while ((window->Flags & ImGuiWindowFlags_ChildWindow) && ((window->ScrollMax.x == 0.0f) || ((window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(window->Flags & ImGuiWindowFlags_NoMouseInputs))))
            window = window->ParentWindow;
        if (!(window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(window->Flags & ImGuiWindowFlags_NoMouseInputs))
        {
            float max_step = window->InnerRect.GetWidth() * 0.67f;
            float scroll_step = ImFloor(ImMin(2 * window->CalcFontSize(), max_step));
            SetScrollX(window, window->Scroll.x - wheel_x * scroll_step);
        }
    }
}


static void UpdateKeyboardInputs()
{
    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;

    // Synchronize io.KeyMods with individual modifiers io.KeyXXX bools
    io.KeyMods = GetMergedKeyModFlags();

    // Import legacy keys or verify they are not used
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    if (io.BackendUsingLegacyKeyArrays == 0)
    {
        // Backend used new io.AddKeyEvent() API: Good! Verify that old arrays are never written too.
        for (int n = 0; n < IM_ARRAYSIZE(io.KeysDown); n++)
            IM_ASSERT(io.KeysDown[n] == false && "Backend needs to either only use io.AddKeyEvent(), either only fill legacy io.KeysDown[] + io.KeyMap[]. Not both!");
    }
    else
    {
        if (g.FrameCount == 0)
            for (int n = ImGuiKey_LegacyNativeKey_BEGIN; n < ImGuiKey_LegacyNativeKey_END; n++)
                IM_ASSERT(g.IO.KeyMap[n] == -1 && "Backend is not allowed to write to io.KeyMap[0..511]!");

        // Build reverse KeyMap (Named -> Legacy)
        for (int n = ImGuiKey_NamedKey_BEGIN; n < ImGuiKey_NamedKey_END; n++)
            if (io.KeyMap[n] != -1)
            {
                IM_ASSERT(IsLegacyKey((ImGuiKey)io.KeyMap[n]));
                io.KeyMap[io.KeyMap[n]] = n;
            }

        // Import legacy keys into new ones
        for (int n = ImGuiKey_LegacyNativeKey_BEGIN; n < ImGuiKey_LegacyNativeKey_END; n++)
            if (io.KeysDown[n] || io.BackendUsingLegacyKeyArrays == 1)
            {
                const ImGuiKey key = (ImGuiKey)(io.KeyMap[n] != -1 ? io.KeyMap[n] : n);
                IM_ASSERT(io.KeyMap[n] == -1 || IsNamedKey(key));
                io.KeysData[key].Down = io.KeysDown[n];
                io.BackendUsingLegacyKeyArrays = 1;
            }
        if (io.BackendUsingLegacyKeyArrays == 1)
        {
            io.KeysData[ImGuiKey_ModCtrl].Down = io.KeyCtrl;
            io.KeysData[ImGuiKey_ModShift].Down = io.KeyShift;
            io.KeysData[ImGuiKey_ModAlt].Down = io.KeyAlt;
            io.KeysData[ImGuiKey_ModSuper].Down = io.KeySuper;
        }
    }
#endif

    // Clear gamepad data if disabled
    if ((io.BackendFlags & ImGuiBackendFlags_HasGamepad) == 0)
        for (int i = ImGuiKey_Gamepad_BEGIN; i < ImGuiKey_Gamepad_END; i++)
        {
            io.KeysData[i - ImGuiKey_KeysData_OFFSET].Down = false;
            io.KeysData[i - ImGuiKey_KeysData_OFFSET].AnalogValue = 0.0f;
        }

    // Update keys
    for (int i = 0; i < IM_ARRAYSIZE(io.KeysData); i++)
    {
        ImGuiKeyData& key_data = io.KeysData[i];
        key_data.DownDurationPrev = key_data.DownDuration;
        key_data.DownDuration = key_data.Down ? (key_data.DownDuration < 0.0f ? 0.0f : key_data.DownDuration + io.DeltaTime) : -1.0f;
    }
}

static void UpdateMouseInputs()
{
    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;

    // Round mouse position to avoid spreading non-rounded position (e.g. UpdateManualResize doesn't support them well)
    if (IsMousePosValid(&io.MousePos))
        io.MousePos = g.MouseLastValidPos = ImFloorSigned(io.MousePos);

    // If mouse just appeared or disappeared (usually denoted by -FLT_MAX components) we cancel out movement in MouseDelta
    if (IsMousePosValid(&io.MousePos) && IsMousePosValid(&io.MousePosPrev))
        //TODOio.MouseDelta = io.MousePos - io.MousePosPrev;
        io.MouseDelta = io.MousePos ;
    else
        io.MouseDelta = ImVec2(0.0f, 0.0f);

    // If mouse moved we re-enable mouse hovering in case it was disabled by gamepad/keyboard. In theory should use a >0.0f threshold but would need to reset in everywhere we set this to true.
    if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)
        g.NavDisableMouseHover = false;

    io.MousePosPrev = io.MousePos;
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
    {
        io.MouseClicked[i] = io.MouseDown[i] && io.MouseDownDuration[i] < 0.0f;
        io.MouseClickedCount[i] = 0; // Will be filled below
        io.MouseReleased[i] = !io.MouseDown[i] && io.MouseDownDuration[i] >= 0.0f;
        io.MouseDownDurationPrev[i] = io.MouseDownDuration[i];
        io.MouseDownDuration[i] = io.MouseDown[i] ? (io.MouseDownDuration[i] < 0.0f ? 0.0f : io.MouseDownDuration[i] + io.DeltaTime) : -1.0f;
        if (io.MouseClicked[i])
        {
            bool is_repeated_click = false;
            if ((float)(g.Time - io.MouseClickedTime[i]) < io.MouseDoubleClickTime)
            {
                //TODO ImVec2 delta_from_click_pos = IsMousePosValid(&io.MousePos) ? (io.MousePos - io.MouseClickedPos[i]) : ImVec2(0.0f, 0.0f);
                ImVec2 delta_from_click_pos = IsMousePosValid(&io.MousePos) ? (io.MousePos) : ImVec2(0.0f, 0.0f);
                if (ImLengthSqr(delta_from_click_pos) < io.MouseDoubleClickMaxDist * io.MouseDoubleClickMaxDist)
                    is_repeated_click = true;
            }
            if (is_repeated_click)
                io.MouseClickedLastCount[i]++;
            else
                io.MouseClickedLastCount[i] = 1;
            io.MouseClickedTime[i] = g.Time;
            io.MouseClickedPos[i] = io.MousePos;
            io.MouseClickedCount[i] = io.MouseClickedLastCount[i];
            io.MouseDragMaxDistanceAbs[i] = ImVec2(0.0f, 0.0f);
            io.MouseDragMaxDistanceSqr[i] = 0.0f;
        }
        else if (io.MouseDown[i])
        {
            // Maintain the maximum distance we reaching from the initial click position, which is used with dragging threshold
            //TODO ImVec2 delta_from_click_pos = IsMousePosValid(&io.MousePos) ? (io.MousePos - io.MouseClickedPos[i]) : ImVec2(0.0f, 0.0f);
            ImVec2 delta_from_click_pos = IsMousePosValid(&io.MousePos) ? (io.MousePos) : ImVec2(0.0f, 0.0f);
            io.MouseDragMaxDistanceSqr[i] = ImMax(io.MouseDragMaxDistanceSqr[i], ImLengthSqr(delta_from_click_pos));
            io.MouseDragMaxDistanceAbs[i].x = ImMax(io.MouseDragMaxDistanceAbs[i].x, delta_from_click_pos.x < 0.0f ? -delta_from_click_pos.x : delta_from_click_pos.x);
            io.MouseDragMaxDistanceAbs[i].y = ImMax(io.MouseDragMaxDistanceAbs[i].y, delta_from_click_pos.y < 0.0f ? -delta_from_click_pos.y : delta_from_click_pos.y);
        }

        // We provide io.MouseDoubleClicked[] as a legacy service
        io.MouseDoubleClicked[i] = (io.MouseClickedCount[i] == 2);

        // Clicking any mouse button reactivate mouse hovering which may have been deactivated by gamepad/keyboard navigation
        if (io.MouseClicked[i])
            g.NavDisableMouseHover = false;
    }
}
static void UIKIT_UpdateFrame()
{
    IM_ASSERT(GImGui != NULL && "No current context. Did you call ImGui::CreateContext() and ImGui::SetCurrentContext() ?");
    ImGuiContext& g = *GImGui;

    // Remove pending delete hooks before frame start.
    // This deferred removal avoids issues of removal while iterating the hook vector
    // CallContextHooks(&g, ImGuiContextHookType_NewFramePre); // Commented out to prevent new frame hooks

    // ErrorCheckNewFrameSanityChecks(); // Commented out to skip frame sanity checks

    // UpdateSettings(); // Commented out to prevent settings update

    g.TooltipOverrideCount = 0;
    g.WindowsActiveCount = 0;
    g.MenusIdSubmittedThisFrame.resize(0);

    // Frame-rate calculation and settings update
    // Commented out to avoid recalculating frame-rate
    /*
    g.FramerateSecPerFrameAccum += g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
    g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
    g.FramerateSecPerFrameIdx = (g.FramerateSecPerFrameIdx + 1) % IM_ARRAYSIZE(g.FramerateSecPerFrame);
    g.FramerateSecPerFrameCount = ImMin(g.FramerateSecPerFrameCount + 1, IM_ARRAYSIZE(g.FramerateSecPerFrame));
    g.IO.Framerate = (g.FramerateSecPerFrameAccum > 0.0f) ? (1.0f / (g.FramerateSecPerFrameAccum / (float)g.FramerateSecPerFrameCount)) : FLT_MAX;
    */

    // Setup current font and draw list shared data
    g.IO.Fonts->Locked = true;
    SetCurrentFont(GetDefaultFont());
    IM_ASSERT(g.Font->IsLoaded());
    ImRect virtual_space(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int n = 0; n < g.Viewports.Size; n++)
        virtual_space.Add(g.Viewports[n]->GetMainRect());
    g.DrawListSharedData.ClipRectFullscreen = virtual_space.ToVec4();
    g.DrawListSharedData.CurveTessellationTol = g.Style.CurveTessellationTol;
    g.DrawListSharedData.SetCircleTessellationMaxError(g.Style.CircleTessellationMaxError);
    g.DrawListSharedData.InitialFlags = ImDrawListFlags_None;
    if (g.Style.AntiAliasedLines)
        g.DrawListSharedData.InitialFlags |= ImDrawListFlags_AntiAliasedLines;
    if (g.Style.AntiAliasedLinesUseTex && !(g.Font->ContainerAtlas->Flags & ImFontAtlasFlags_NoBakedLines))
        g.DrawListSharedData.InitialFlags |= ImDrawListFlags_AntiAliasedLinesUseTex;
    if (g.Style.AntiAliasedFill)
        g.DrawListSharedData.InitialFlags |= ImDrawListFlags_AntiAliasedFill;
    if (g.IO.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset)
        g.DrawListSharedData.InitialFlags |= ImDrawListFlags_AllowVtxOffset;

    // Mark rendering data as invalid to prevent user who may have a handle on it to use it.
    for (int n = 0; n < g.Viewports.Size; n++)
    {
        ImGuiViewportP* viewport = g.Viewports[n];
        viewport->DrawData = NULL;
        viewport->DrawDataP.Clear();
    }

    // Drag and drop keep the source ID alive so even if the source disappear our state is consistent
    if (g.DragDropActive && g.DragDropPayload.SourceId == g.ActiveId)
        KeepAliveID(g.DragDropPayload.SourceId);

    // Update HoveredId data
    if (!g.HoveredIdPreviousFrame)
        g.HoveredIdTimer = 0.0f;
    if (!g.HoveredIdPreviousFrame || (g.HoveredId && g.ActiveId == g.HoveredId))
        g.HoveredIdNotActiveTimer = 0.0f;
    if (g.HoveredId)
        g.HoveredIdTimer += g.IO.DeltaTime;
    if (g.HoveredId && g.ActiveId != g.HoveredId)
        g.HoveredIdNotActiveTimer += g.IO.DeltaTime;
    g.HoveredIdPreviousFrame = g.HoveredId;
    g.HoveredIdPreviousFrameUsingMouseWheel = g.HoveredIdUsingMouseWheel;
    g.HoveredId = 0;
    g.HoveredIdAllowOverlap = false;
    g.HoveredIdUsingMouseWheel = false;
    g.HoveredIdDisabled = false;

    // Update ActiveId data (clear reference to active widget if the widget isn't alive anymore)
    if (g.ActiveIdIsAlive != g.ActiveId && g.ActiveIdPreviousFrame == g.ActiveId && g.ActiveId != 0)
        ClearActiveID();
    if (g.ActiveId)
        g.ActiveIdTimer += g.IO.DeltaTime;
    g.LastActiveIdTimer += g.IO.DeltaTime;
    g.ActiveIdPreviousFrame = g.ActiveId;
    g.ActiveIdPreviousFrameWindow = g.ActiveIdWindow;
    g.ActiveIdPreviousFrameHasBeenEditedBefore = g.ActiveIdHasBeenEditedBefore;
    g.ActiveIdIsAlive = 0;
    g.ActiveIdHasBeenEditedThisFrame = false;
    g.ActiveIdPreviousFrameIsAlive = false;
    g.ActiveIdIsJustActivated = false;
    if (g.TempInputId != 0 && g.ActiveId != g.TempInputId)
        g.TempInputId = 0;
    if (g.ActiveId == 0)
    {
        g.ActiveIdUsingNavDirMask = 0x00;
        g.ActiveIdUsingNavInputMask = 0x00;
        g.ActiveIdUsingKeyInputMask.ClearAllBits();
    }

    // Drag and drop
    g.DragDropAcceptIdPrev = g.DragDropAcceptIdCurr;
    g.DragDropAcceptIdCurr = 0;
    g.DragDropAcceptIdCurrRectSurface = FLT_MAX;
    g.DragDropWithinSource = false;
    g.DragDropWithinTarget = false;
    g.DragDropHoldJustPressedId = 0;

    // Close popups on focus lost (currently wip/opt-in)
    if (g.IO.AppFocusLost)
        ClosePopupsExceptModals();


    // Background darkening/whitening
    if (GetTopMostPopupModal() != NULL || (g.NavWindowingTarget != NULL && g.NavWindowingHighlightAlpha > 0.0f))
        g.DimBgRatio = ImMin(g.DimBgRatio + g.IO.DeltaTime * 6.0f, 1.0f);
    else
        g.DimBgRatio = ImMax(g.DimBgRatio - g.IO.DeltaTime * 10.0f, 0.0f);

    g.MouseCursor = ImGuiMouseCursor_Arrow;
    g.WantCaptureMouseNextFrame = g.WantCaptureKeyboardNextFrame = g.WantTextInputNextFrame = -1;

    // Platform IME data: reset for the frame
    g.PlatformImeDataPrev = g.PlatformImeData;
    g.PlatformImeData.WantVisible = false;

    // Mouse wheel scrolling, scale
    UpdateMouseWheel();

    // Mark all windows as not visible and compact unused memory.
    IM_ASSERT(g.WindowsFocusOrder.Size <= g.Windows.Size);
    const float memory_compact_start_time = (g.GcCompactAll || g.IO.ConfigMemoryCompactTimer < 0.0f) ? FLT_MAX : (float)g.Time - g.IO.ConfigMemoryCompactTimer;
    for (int i = 0; i != g.Windows.Size; i++)
    {
        ImGuiWindow* window = g.Windows[i];
        window->WasActive = window->Active;
        window->BeginCount = 0;
        window->Active = false;
        window->WriteAccessed = false;

        // Garbage collect transient buffers of recently unused windows
        if (!window->WasActive && !window->MemoryCompacted && window->LastTimeActive < memory_compact_start_time)
            GcCompactTransientWindowBuffers(window);
    }

    // Garbage collect transient buffers of recently unused tables
    for (int i = 0; i < g.TablesLastTimeActive.Size; i++)
        if (g.TablesLastTimeActive[i] >= 0.0f && g.TablesLastTimeActive[i] < memory_compact_start_time)
            TableGcCompactTransientBuffers(g.Tables.GetByIndex(i));
    for (int i = 0; i < g.TablesTempData.Size; i++)
        if (g.TablesTempData[i].LastTimeActive >= 0.0f && g.TablesTempData[i].LastTimeActive < memory_compact_start_time)
            TableGcCompactTransientBuffers(&g.TablesTempData[i]);
    if (g.GcCompactAll)
        GcCompactTransientMiscBuffers();
    g.GcCompactAll = false;

    // Closing the focused window restore focus to the first active root window in descending z-order
    if (g.NavWindow && !g.NavWindow->WasActive)
        FocusTopMostWindowUnderOne(NULL, NULL);

    // No window should be open at the beginning of the frame.
    // But in order to allow the user to call NewFrame() multiple times without calling Render(), we are doing an explicit clear.
    g.CurrentWindowStack.resize(0);
    g.BeginPopupStack.resize(0);
    g.ItemFlagsStack.resize(0);
    g.ItemFlagsStack.push_back(ImGuiItemFlags_None);
    g.GroupStack.resize(0);

    // Docking
    DockContextNewFrameUpdateDocking(&g);

    // [DEBUG] Update debug features
    UpdateDebugToolItemPicker();
    UpdateDebugToolStackQueries();

    // Create implicit/fallback window - which we will only render it if the user has added something to it.
    // We don't use "Debug" to avoid colliding with user trying to create a "Debug" window with custom flags.
    // This fallback is particularly important as it avoid ImGui:: calls from crashing.
    g.WithinFrameScopeWithImplicitWindow = true;
    SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    Begin("Debug##Default");
    IM_ASSERT(g.CurrentWindow->IsFallbackWindow == true);
}

}
static void SetupVulkan(const char **extensions, uint32_t extensions_count)
{
	VkResult err;

	// Create Vulkan Instance
	{
		VkInstanceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.enabledExtensionCount = extensions_count;
		create_info.ppEnabledExtensionNames = extensions;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
		// Enabling validation layers
		const char *layers[] = {"VK_LAYER_KHRONOS_validation"};
		create_info.enabledLayerCount = 1;
		create_info.ppEnabledLayerNames = layers;

		// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
		const char **extensions_ext = (const char **)malloc(sizeof(const char *) * (extensions_count + 1));
		memcpy(extensions_ext, extensions, extensions_count * sizeof(const char *));
		extensions_ext[extensions_count] = "VK_EXT_debug_report";
		create_info.enabledExtensionCount = extensions_count + 1;
		create_info.ppEnabledExtensionNames = extensions_ext;

		// Create Vulkan Instance
		err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
		check_vk_result(err);
		free(extensions_ext);

		// Get the function pointer (required for any extensions)
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
		IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

		// Setup the debug report callback
		VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
		debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		debug_report_ci.pfnCallback = debug_report;
		debug_report_ci.pUserData = NULL;
		err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
		check_vk_result(err);
#else
		// Create Vulkan Instance without any debug feature
		err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
		check_vk_result(err);
		IM_UNUSED(g_DebugReport);
#endif
	}

	// Select GPU
	{
		uint32_t gpu_count;
		err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, NULL);
		check_vk_result(err);
		IM_ASSERT(gpu_count > 0);

		VkPhysicalDevice *gpus = (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * gpu_count);
		err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus);
		check_vk_result(err);

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		int use_gpu = 0;
		for (int i = 0; i < (int)gpu_count; i++)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(gpus[i], &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				use_gpu = i;
				break;
			}
		}

		g_PhysicalDevice = gpus[use_gpu];
		free(gpus);
	}

	// Select graphics queue family
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, NULL);
		VkQueueFamilyProperties *queues = (VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * count);
		vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
		for (uint32_t i = 0; i < count; i++)
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				g_QueueFamily = i;
				break;
			}
		free(queues);
		IM_ASSERT(g_QueueFamily != (uint32_t)-1);
	}

	// Create Logical Device (with 1 queue)
	{
		int device_extension_count = 1;
		const char *device_extensions[] = {"VK_KHR_swapchain"};
		const float queue_priority[] = {1.0f};
		VkDeviceQueueCreateInfo queue_info[1] = {};
		queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info[0].queueFamilyIndex = g_QueueFamily;
		queue_info[0].queueCount = 1;
		queue_info[0].pQueuePriorities = queue_priority;
		VkDeviceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
		create_info.pQueueCreateInfos = queue_info;
		create_info.enabledExtensionCount = device_extension_count;
		create_info.ppEnabledExtensionNames = device_extensions;
		err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
		check_vk_result(err);
		vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
	}

	// Create Descriptor Pool
	{
		VkDescriptorPoolSize pool_sizes[] =
			{
				{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
		check_vk_result(err);
	}
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, VkSurfaceKHR surface, int width, int height, UIKit::Window *win)
{
	wd->Surface = surface;

	// Check for WSI support
	VkBool32 res;
	vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
	if (res != VK_TRUE)
	{
		fprintf(stderr, "Error no WSI support on physical device 0\n");
		exit(-1);
	}

	// Select Surface Format
	const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

	// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
	VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR};
#else
	VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
	wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
	// printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);
	// Create SwapChain, RenderPass, Framebuffer, etc.
	IM_ASSERT(g_MinImageCount >= 2);
	ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
}

static void CleanupVulkan(UIKit::Window *win)
{
	vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
	// Remove the debug report callback
	auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
	vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

	vkDestroyDevice(g_Device, g_Allocator);
	vkDestroyInstance(g_Instance, g_Allocator);
}

static UIKit::Application *app;

static void CleanupVulkanWindow(UIKit::Window *win)
{
	ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &win->m_WinData, g_Allocator);
}

static void CleanupSpecificVulkanWindow(UIKit::Window *win)
{
	ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &win->m_WinData, g_Allocator);
}

static void FrameRender(ImGui_ImplVulkanH_Window *wd, UIKit::Window *win, ImDrawData *draw_data)
{
	VkResult err;

	VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);

	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		g_SwapChainRebuild = true;

		std::cout << "===== FRAME RENDER " + win->GetName() + " =====" << std::endl;
		std::cout << "ABORDING" << std::endl;
		std::cout << "=========================" << std::endl;

		return;
	}
	check_vk_result(err);

	ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];

	std::cout << "===== FRAME RENDER " + win->GetName() + " =====" << std::endl;
	std::cout << "wd : " << wd << std::endl;
	std::cout << "win : " << win << std::endl;
	std::cout << "fd : " << fd << std::endl;
	std::cout << "fd->CommandBuffer : " << fd->CommandBuffer << std::endl;
	std::cout << "draw_data : " << draw_data << std::endl;
	std::cout << "CmdListsCount : " << draw_data->CmdListsCount << std::endl;
	std::cout << "CmdListsCount : " << draw_data->CmdLists << std::endl;
	std::cout << "=========================" << std::endl;

	s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % win->m_WinData.ImageCount;

	{
		err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);
		check_vk_result(err);

		err = vkResetFences(g_Device, 1, &fd->Fence);
		check_vk_result(err);
	}

	{
		for (auto &func : s_ResourceFreeQueue[win->m_WinData.FrameIndex])
			func();
		s_ResourceFreeQueue[win->m_WinData.FrameIndex].clear();
	}
	{
		auto &allocatedCommandBuffers = win->s_AllocatedCommandBuffers[wd->FrameIndex];
		if (allocatedCommandBuffers.size() > 0)
		{
			vkFreeCommandBuffers(g_Device, fd->CommandPool, (uint32_t)allocatedCommandBuffers.size(), allocatedCommandBuffers.data());
			allocatedCommandBuffers.clear();
		}

		err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
		check_vk_result(err);
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		check_vk_result(err);
	}
	{
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = wd->RenderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = wd->Width;
		info.renderArea.extent.height = wd->Height;
		info.clearValueCount = 1;
		info.pClearValues = &wd->ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &image_acquired_semaphore;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &render_complete_semaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		check_vk_result(err);
		err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
		check_vk_result(err);
	}
}

static void FramePresent(ImGui_ImplVulkanH_Window *wd, UIKit::Window *win)
{
	std::cout << "===== FRAME PRESENT =====" << std::endl;
	std::cout << "wd : " << wd << std::endl;
	std::cout << "win : " << win << std::endl;
	std::cout << "=========================" << std::endl;
	if (g_SwapChainRebuild)
		return;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &render_complete_semaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &wd->Swapchain;
	info.pImageIndices = &wd->FrameIndex;
	VkResult err = vkQueuePresentKHR(g_Queue, &info);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		g_SwapChainRebuild = true;
		return;
	}
	check_vk_result(err);
	wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
}

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace UIKit
{

#include "Embed/UIKit-Icon.embed"
#include "Embed/WindowImages.embed"

	Application::Application(const ApplicationSpecification &specification)
		: m_Specification(specification)
	{
		s_Instance = this;

		Init();
	}

	Application::~Application()
	{
		Shutdown();

		s_Instance = nullptr;
	}

	Application &Application::Get()
	{
		return *s_Instance;
	}

	void Application::Init()
	{
		// Set main context application ptr
		app = &this->Get();

		// Intialize logging
		Log::Init();

		// Setup GLFW window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			std::cerr << "Could not initalize GLFW!\n";
			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Pas de contexte OpenGL

		// Check if custom titlebar is enabled
		bool customTitlebar = app->m_Specification.CustomTitlebar;
		glfwWindowHint(GLFW_DECORATED, customTitlebar ? GLFW_FALSE : GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		// Setup Vulkan instance, device, etc.
		uint32_t extensions_count = 2;
		const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
		SetupVulkan(extensions, extensions_count);

		// Create the ImGui context
		IMGUI_CHECKVERSION();
		if (!m_ImGuiContext)
		{
			m_ImGuiContext = ImGui::CreateContext();
		}
		ImGui::SetCurrentContext(m_ImGuiContext);

		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows
		io.FontGlobalScale = 0.83f;

		// Theme colors
		UI::SetHazelTheme();

		// Style
		ImGuiStyle &style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.FramePadding = ImVec2(8.0f, 6.0f);
		style.ItemSpacing = ImVec2(6.0f, 6.0f);
		style.ChildRounding = 6.0f;
		style.PopupRounding = 6.0f;
		style.FrameRounding = 6.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		this->m_Windows.push_back(std::make_shared<Window>("base", app->m_Specification.Width, app->m_Specification.Height));
		//this->m_Windows.push_back(std::make_shared<Window>("b22ase", 1800, 1000));
		//this->m_Windows.push_back(std::make_shared<Window>("b22a999se", 1800, 1000));
	}

	void Application::Shutdown()
	{
		for (auto &layer : m_LayerStack)
			layer->OnDetach();

		m_LayerStack.clear();

		// Release resources
		// NOTE(Yan): to avoid doing this manually, we shouldn't
		//            store resources in this Application class
		// m_AppHeaderIcon.reset();
		// m_IconClose.reset();
		// m_IconMinimize.reset();
		// m_IconMaximize.reset();
		// m_IconRestore.reset();

		// Cleanup
		for (auto win : app->m_Windows)
		{
			VkResult err = vkDeviceWaitIdle(g_Device);
			check_vk_result(err);

			// Free resources in queue
			for (auto &queue : s_ResourceFreeQueue)
			{
				for (auto &func : queue)
					func();
			}
			s_ResourceFreeQueue.clear();
		}

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Cleanup
		for (auto win : app->m_Windows)
		{
			CleanupSpecificVulkanWindow(win.get());
			CleanupVulkan(win.get());
		}

		// glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();

		g_ApplicationRunning = false;

		Log::Shutdown();
	}
	void Window::UI_DrawTitlebar(float &outTitlebarHeight)
	{
		const float titlebarHeight = 58.0f;

		const bool isMaximized = (bool)glfwGetWindowAttrib(this->GetWindowHandle(), GLFW_MAXIMIZED);
		;
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = {ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									ImGui::GetCursorScreenPos().y + titlebarHeight};
		auto *bgDrawList = ImGui::GetBackgroundDrawList();
		auto *fgDrawList = ImGui::GetForegroundDrawList();
		bgDrawList->AddRectFilled(titlebarMin, titlebarMax, UI::Colors::Theme::titlebar);
		// DEBUG TITLEBAR BOUNDS
		// fgDrawList->AddRect(titlebarMin, titlebarMax, UI::Colors::Theme::invalidPrefab);

		// Logo
		{
			const int logoWidth = 48;  // m_LogoTex->GetWidth();
			const int logoHeight = 48; // m_LogoTex->GetHeight();
			const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = {ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y};
			const ImVec2 logoRectMax = {logoRectStart.x + logoWidth, logoRectStart.y + logoHeight};
			fgDrawList->AddImage(m_AppHeaderIcon->GetDescriptorSet(), logoRectStart, logoRectMax);
		}

		ImGui::SetItemAllowOverlap();
		ImGui::BeginHorizontal("Titlebar", {ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing()});

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;

		// Draw Menubar
		if (app->m_MenubarCallback)
		{
			ImGui::SuspendLayout();
			{
				ImGui::SetItemAllowOverlap();
				const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
				ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
				UI_DrawMenubar();
			}

			ImGui::ResumeLayout();
		}

		{
			// Centered Window title
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize(app->m_Specification.Name.c_str());
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", app->m_Specification.Name.c_str()); // Draw title
			ImGui::SetCursorPos(currentCursorPos);
		}

		// Window buttons
		const ImU32 buttonColN = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 0.9f);
		const ImU32 buttonColH = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.2f);
		const ImU32 buttonColP = UI::Colors::Theme::textDarker;
		const float buttonWidth = 14.0f;
		const float buttonHeight = 14.0f;

		if (app->m_Specification.CustomTitlebar)
		{
			// Minimize Button
			ImGui::Spring();
			UI::ShiftCursorY(8.0f);

            if(!app->m_Specification.WindowOnlyClosable)
            {
			{
				const int iconWidth = this->m_IconMinimize->GetWidth();
				const int iconHeight = this->m_IconMinimize->GetHeight();
				const float padY = (buttonHeight - (float)iconHeight) / 2.0f;

				{
					std::string label = "Minimize###" + this->GetName();
					if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
					{
						// TODO: move this stuff to a better place, like Window class
						if (this->GetWindowHandle())
						{
							this->QueueEvent([windowHandle = this->GetWindowHandle()]()
											 { glfwIconifyWindow(windowHandle); });
						}
					}
				}

				UI::DrawButtonImage(this->m_IconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
			}

            }
			// Maximize Button
			ImGui::Spring(-1.0f, 17.0f);
			UI::ShiftCursorY(8.0f);
            if(!app->m_Specification.WindowOnlyClosable)
            {
			{
				const int iconWidth = this->m_IconMaximize->GetWidth();
				const int iconHeight = this->m_IconMaximize->GetHeight();

				const bool isMaximized = (bool)glfwGetWindowAttrib(this->GetWindowHandle(), GLFW_MAXIMIZED);

				{

					std::string label = "Maximize###" + this->GetName();
					if (ImGui::InvisibleButton(label.c_str(), ImVec2(buttonWidth, buttonHeight)))
					{
						this->QueueEvent([isMaximized, windowHandle = this->GetWindowHandle()]()
										 {
					if (isMaximized)
						glfwRestoreWindow(windowHandle);
					else
						glfwMaximizeWindow(windowHandle); });
					}
				}
				UI::DrawButtonImage(isMaximized ? this->m_IconRestore : this->m_IconMaximize, buttonColN, buttonColH, buttonColP);
			}
            }
			
            {
				// Close Button
				ImGui::Spring(-1.0f, 15.0f);
				UI::ShiftCursorY(8.0f);
				{
					const int iconWidth = this->m_IconClose->GetWidth();
					const int iconHeight = this->m_IconClose->GetHeight();
					std::string label = "Close###" + this->GetName();
					if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
						Application::Get().Close();

					UI::DrawButtonImage(this->m_IconClose, UI::Colors::Theme::text, UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.4f), buttonColP);
				}
			}
			ImGui::Spring(-1.0f, 18.0f);
		}
		ImGui::EndHorizontal();

		outTitlebarHeight = titlebarHeight;
	}

	// REndre la title bar moovable comme le reste, fix l'erreur de double main win et rendre la fenetre dco:mplete
	void Application::UI_DrawTitlebar(float &outTitlebarHeight, Window *window)
	{
		const float titlebarHeight = 58.0f;
		const bool isMaximized = (bool)glfwGetWindowAttrib(window->GetWindowHandle(), GLFW_MAXIMIZED);
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = {ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									ImGui::GetCursorScreenPos().y + titlebarHeight};
		auto *bgDrawList = ImGui::GetBackgroundDrawList();
		auto *fgDrawList = ImGui::GetForegroundDrawList();
		bgDrawList->AddRectFilled(titlebarMin, titlebarMax, UI::Colors::Theme::titlebar);
		// DEBUG TITLEBAR BOUNDS
		// fgDrawList->AddRect(titlebarMin, titlebarMax, UI::Colors::Theme::invalidPrefab);

		// Logo
		{
			const int logoWidth = 48;  // m_LogoTex->GetWidth();
			const int logoHeight = 48; // m_LogoTex->GetHeight();
			const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = {ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y};
			const ImVec2 logoRectMax = {logoRectStart.x + logoWidth, logoRectStart.y + logoHeight};

			fgDrawList->AddImage(window->m_AppHeaderIcon->GetDescriptorSet(), logoRectStart, logoRectMax);
		}

		ImGui::BeginHorizontal("Titlebar", {ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing()});

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;
		// Title bar drag area
		// On Windows we hook into the GLFW win32 window internals
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset)); // Reset cursor pos
		// DEBUG DRAG BOUNDS
		// fgDrawList->AddRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + w - buttonsAreaWidth, ImGui::GetCursorScreenPos().y + titlebarHeight), UI::Colors::Theme::invalidPrefab);
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		m_TitleBarHovered = ImGui::IsItemHovered();

		// Inside your Application class

		// Initialize a static variable to keep track of click state
		static bool isFirstClick = true;
		static double lastClickTime = -1;

		// Inside your ImGui rendering code

		// Initialize a static variable to keep track of dragging state
		static bool isDraggingWindow = false;
		static ImVec2 windowDragStartPos;

		int windowPosX, windowPosY;
		glfwGetWindowPos(window->GetWindowHandle(), &windowPosX, &windowPosY);
		static ImVec2 windowPos{static_cast<float>(windowPosX), static_cast<float>(windowPosY)};

		// Title bar drag area
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset)); // Reset cursor pos
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		m_TitleBarHovered = ImGui::IsItemHovered();

		if (isMaximized)
		{
			float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
			if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
				m_TitleBarHovered = true; // Account for the top-most pixels which don't register
		}

		// Draw Menubar
		if (m_MenubarCallback)
		{
			ImGui::SuspendLayout();
			{
				ImGui::SetItemAllowOverlap();
				const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
				ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
				UI_DrawMenubar();

				if (ImGui::IsItemHovered())
					m_TitleBarHovered = false;
			}

			ImGui::ResumeLayout();
		}

		{
			// Centered Window title
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize(m_Specification.Name.c_str());
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", m_Specification.Name.c_str()); // Draw title
			ImGui::SetCursorPos(currentCursorPos);
		}

		// Window buttons
		const ImU32 buttonColN = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 0.9f);
		const ImU32 buttonColH = UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.2f);
		const ImU32 buttonColP = UI::Colors::Theme::textDarker;
		const float buttonWidth = 14.0f;
		const float buttonHeight = 14.0f;

		if (m_Specification.CustomTitlebar)
		{
			// Minimize Button
			ImGui::Spring();
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = window->m_IconMinimize->GetWidth();
				const int iconHeight = window->m_IconMinimize->GetHeight();
				const float padY = (buttonHeight - (float)iconHeight) / 2.0f;
				if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
				{
					// TODO: move this stuff to a better place, like Window class
					if (window->GetWindowHandle())
					{
						window->QueueEvent([windowHandle = window->GetWindowHandle()]()
										   { glfwIconifyWindow(windowHandle); });
					}
				}

				UI::DrawButtonImage(window->m_IconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
			}

			// Maximize Button
			ImGui::Spring(-1.0f, 17.0f);
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = window->m_IconMaximize->GetWidth();
				const int iconHeight = window->m_IconMaximize->GetHeight();

				const bool isMaximized = (bool)glfwGetWindowAttrib(window->GetWindowHandle(), GLFW_MAXIMIZED);

				if (ImGui::InvisibleButton("Maximize", ImVec2(buttonWidth, buttonHeight)))
				{
					window->QueueEvent([isMaximized, windowHandle = window->GetWindowHandle()]()
									   {
					if (isMaximized)
						glfwRestoreWindow(windowHandle);
					else
						glfwMaximizeWindow(windowHandle); });
				}
				UI::DrawButtonImage(isMaximized ? window->m_IconRestore : window->m_IconMaximize, buttonColN, buttonColH, buttonColP);
			}

			// Close Button
			ImGui::Spring(-1.0f, 15.0f);
			UI::ShiftCursorY(8.0f);
			{
				const int iconWidth = window->m_IconClose->GetWidth();
				const int iconHeight = window->m_IconClose->GetHeight();
				if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
					Application::Get().Close();

				UI::DrawButtonImage(window->m_IconClose, UI::Colors::Theme::text, UI::Colors::ColorWithMultipliedValue(UI::Colors::Theme::text, 1.4f), buttonColP);
			}

			ImGui::Spring(-1.0f, 18.0f);
		}
		ImGui::EndHorizontal();

		outTitlebarHeight = titlebarHeight;
	}

	void Window::UI_DrawMenubar()
	{
		if (!app->m_MenubarCallback)
			return;

		if (app->m_Specification.CustomTitlebar)
		{
			const ImRect menuBarRect = {ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()}};

			ImGui::BeginGroup();
			if (UI::BeginMenubar(menuBarRect))
			{
				app->m_MenubarCallback();
			}

			UI::EndMenubar();
			ImGui::EndGroup();
		}
		else
		{
			if (ImGui::BeginMenuBar())
			{
				app->m_MenubarCallback();
				ImGui::EndMenuBar();
			}
		}
	}

	void Application::UI_DrawMenubar()
	{
		if (!m_MenubarCallback)
			return;

		if (m_Specification.CustomTitlebar)
		{
			const ImRect menuBarRect = {ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()}};

			ImGui::BeginGroup();
			if (UI::BeginMenubar(menuBarRect))
			{
				m_MenubarCallback();
			}

			UI::EndMenubar();
			ImGui::EndGroup();
		}
		else
		{
			if (ImGui::BeginMenuBar())
			{
				m_MenubarCallback();
				ImGui::EndMenuBar();
			}
		}
	}

	void CheckUndockedWindows()
	{
		ImGuiContext &g = *ImGui::GetCurrentContext();
		for (int n = 0; n < g.Windows.Size; n++)
		{
			ImGuiWindow *window = g.Windows[n];
			if (window->DockIsActive && window->DockNode == NULL) // Active window with no DockNode means it is undocked
			{
				std::cout << "Window '%s' is undocked\n"
						  << window->Name << std::endl;
			}
		}
	}

	void Window::RequestResize(int width, int height)
	{
		m_ResizePending = true;
		g_SwapChainRebuild = true;
		m_PendingWidth = width;
		m_PendingHeight = height;
	}

	void Window::RequestMove(int x, int y)
	{
		m_MovePending = true;
		g_SwapChainRebuild = true;
		m_PendingX = x;
		m_PendingY = y;
	}

	// Fonction de rappel pour la gestion du clic de souris
	void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		std::cout << "OKKK" << std::endl;

		Window* win = app->GetWindowByHandle(window);
		if(win)
		{
		std::cout << "Click registered from " << win->GetName() << " ! : )" << std::endl;

		}
		else{
		std::cout << "Click null" << std::endl;

		}

	}

	// Fonction de rappel pour la gestion du mouvement de souris
	void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
	{
		std::cout << "MOVE" << std::endl;
	}

	template <typename Func>
	void Window::QueueEvent(Func &&func)
	{
		m_EventQueue.push(func);
	}

	void Window::Render()
	{
		ImGuiIO &io = ImGui::GetIO();
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		// Ensure the current ImGui context is set for this window
		
		g_SwapChainRebuild = true;
		if (g_SwapChainRebuild)
		{
			int width, height;
			glfwGetFramebufferSize(this->GetWindowHandle(), &width, &height);
			if (width > 0 && height > 0)
			{
				ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
				ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &this->m_WinData, g_QueueFamily, g_Allocator, this->m_WinData.Width, this->m_WinData.Height, g_MinImageCount);
				this->m_WinData.FrameIndex = 0;

				// Clear allocated command buffers from here since entire pool is destroyed
				this->s_AllocatedCommandBuffers.clear();
				this->s_AllocatedCommandBuffers.resize(this->m_WinData.ImageCount);

				g_SwapChainRebuild = false;
			}
		}

		if (this->m_ResizePending)
		{
			glfwSetWindowSize(this->GetWindowHandle(), this->m_PendingWidth, this->m_PendingHeight);
			this->m_ResizePending = false;
		}

		{
			std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

			while (!m_EventQueue.empty())
			{
				auto &func = m_EventQueue.front();
				func();
				m_EventQueue.pop();
			}
		}

		app->RenderWindow(this);

		const bool main_is_minimized = (this->DrawData.DisplaySize.x <= 0.0f || this->DrawData.DisplaySize.y <= 0.0f);

		ImGui_ImplVulkanH_Window *wd = &this->m_WinData;
		wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		wd->ClearValue.color.float32[3] = clear_color.w;
		
		if (!main_is_minimized)
			FrameRender(wd, this, &this->DrawData);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		if (!main_is_minimized)
			FramePresent(wd, this);
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	void Application::Run()
	{
		for (auto &window : m_Windows)
		{
			window->LoadImages();
		}

		m_Running = true;

		while (m_Running)
		{
			// Update layers
			for (auto &layer : m_LayerStack)
			{
				layer->OnUpdate(m_TimeStep);
			}
			glfwPollEvents();
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			for (auto &window : m_Windows)
			{

				window->Render();
			}

			// Update time
			float time = GetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;
		}
	}

	Window::Window(const std::string &name, int width, int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
		m_WindowHandle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		if (!m_WindowHandle)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		this->WinID = WinIDCount;
		WinIDCount++;

		// Center the window if needed
		if (app->m_Specification.CenterWindow)
		{
			GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
			int monitorX, monitorY;
			glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
			glfwSetWindowPos(m_WindowHandle,
							 monitorX + (videoMode->width - width) / 2,
							 monitorY + (videoMode->height - height) / 2);
		}

		// Setup Vulkan
		if (!glfwVulkanSupported())
		{
			glfwDestroyWindow(m_WindowHandle);
			glfwTerminate();
			throw std::runtime_error("GLFW: Vulkan not supported!");
		}

		glfwSetMouseButtonCallback(this->GetWindowHandle(), MouseButtonCallback);

		// Set icon
		if (!app->m_Specification.IconPath.empty())
		{
			GLFWimage icon[1];
			int channels = 0;
			icon[0].pixels = stbi_load("/usr/include/vortex/icon.png", &icon[0].width, &icon[0].height, &channels, 4);
			glfwSetWindowIcon(m_WindowHandle, 1, icon);
			stbi_image_free(icon[0].pixels);
		}

		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(g_Instance, m_WindowHandle, g_Allocator, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}

		// Create Framebuffers
		int w, h;
		glfwGetFramebufferSize(m_WindowHandle, &w, &h);
		ImGui_ImplVulkanH_Window *wd = &m_WinData;
		SetupVulkanWindow(wd, surface, w, h, this);
		this->s_AllocatedCommandBuffers.resize(wd->ImageCount);
		s_ResourceFreeQueue.resize(wd->ImageCount);

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(m_WindowHandle, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = g_Instance;
		init_info.PhysicalDevice = g_PhysicalDevice;
		init_info.Device = g_Device;
		init_info.QueueFamily = g_QueueFamily;
		init_info.Queue = g_Queue;
		init_info.PipelineCache = g_PipelineCache;
		init_info.DescriptorPool = g_DescriptorPool;
		init_info.Subpass = 0;
		init_info.MinImageCount = g_MinImageCount;
		init_info.ImageCount = wd->ImageCount;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = g_Allocator;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

		ImGuiIO &io = ImGui::GetIO();
		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		ImFont *robotoFont = io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		ImFont *hackFont = io.Fonts->AddFontFromMemoryTTF((void *)g_HackRegular, sizeof(g_HackRegular), 20.0f, &fontConfig);

		ImFontConfig fontConfigExtra;
		fontConfigExtra.GlyphExtraSpacing.x = 1.3f;
		ImFont *inconsolatas = io.Fonts->AddFontFromMemoryTTF((void *)g_Inconsolatas, sizeof(g_Inconsolatas), 20.0f, &fontConfigExtra);

		s_Fonts["Default"] = hackFont;
		s_Fonts["Inconsolatas"] = inconsolatas;
		s_Fonts["Bold"] = io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoBold, sizeof(g_RobotoBold), 20.0f, &fontConfig);
		s_Fonts["Italic"] = io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoItalic, sizeof(g_RobotoItalic), 20.0f, &fontConfig);
		s_Fonts["HackRegular"] = io.Fonts->AddFontFromMemoryTTF((void *)g_HackRegular, sizeof(g_HackRegular), 20.0f, &fontConfig);
		io.FontDefault = hackFont;

		// Upload Fonts
		{
			VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
			VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

			VkResult err = vkResetCommandPool(g_Device, command_pool, 0);
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
			err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
			check_vk_result(err);

			err = vkDeviceWaitIdle(g_Device);
			check_vk_result(err);
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}
	}
    
	void Window::LoadImages()
	{
		{
			uint32_t w, h;
			void *data = Image::Decode(g_UIKitIcon, sizeof(g_UIKitIcon), w, h);
			this->m_AppHeaderIcon = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowMinimizeIcon, sizeof(g_WindowMinimizeIcon), w, h);
			this->m_IconMinimize = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowMaximizeIcon, sizeof(g_WindowMaximizeIcon), w, h);
			this->m_IconMaximize = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowRestoreIcon, sizeof(g_WindowRestoreIcon), w, h);
			this->m_IconRestore = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
		{
			uint32_t w, h;
			void *data = Image::Decode(g_WindowCloseIcon, sizeof(g_WindowCloseIcon), w, h);
			this->m_IconClose = std::make_shared<UIKit::Image>(w, h, ImageFormat::RGBA, this->GetName(), data);
			free(data);
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	std::shared_ptr<Image> Application::GetApplicationIcon(const std::string &window) const
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == window)
			{
				return win->m_AppHeaderIcon;
			}
		}
		return nullptr;
	}

	bool Application::IsMaximized() const
	{
		//	return (bool)glfwGetWindowAttrib(m_WindowHandle, GLFW_MAXIMIZED);
	}

	float Application::GetTime()
	{
		return (float)glfwGetTime();
	}

	VkInstance Application::GetInstance(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				return g_Instance;
			}
		}
		return nullptr;
	}

	VkPhysicalDevice Application::GetPhysicalDevice(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				return g_PhysicalDevice;
			}
		}
		return nullptr;
	}

	VkDevice Application::GetDevice(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				return g_Device;
			}
		}
		return nullptr;
	}

	VkCommandBuffer Window::GetCommandBuffer(bool begin)
	{
		ImGui_ImplVulkanH_Window *wd = &this->m_WinData;

		// Use any command queue
		VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

		VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
		cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufAllocateInfo.commandPool = command_pool;
		cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer &command_buffer = this->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
		auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(command_buffer, &begin_info);
		check_vk_result(err);

		return command_buffer;
	}

	VkCommandBuffer Application::GetCommandBuffer(const std::string &win_name, bool begin)
	{
		ImGui_ImplVulkanH_Window *wd = nullptr;

		for (auto window : app->m_Windows)
		{
			if (window->GetName() == win_name)
			{
				wd = &window->m_WinData;

				// Use any command queue
				VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

				VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
				cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmdBufAllocateInfo.commandPool = command_pool;
				cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmdBufAllocateInfo.commandBufferCount = 1;

				VkCommandBuffer &command_buffer = window->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
				auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

				VkCommandBufferBeginInfo begin_info = {};
				begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				err = vkBeginCommandBuffer(command_buffer, &begin_info);
				check_vk_result(err);

				return command_buffer;
			}
		}

		return nullptr;
	}

	VkCommandBuffer Application::GetCommandBufferOfWin(const std::string &win_name, bool begin)
	{
		ImGui_ImplVulkanH_Window *wd = nullptr;

		for (auto layer : app->m_LayerStack)
		{
			if (layer->ParentWindow == win_name)
			{
				for (auto win : app->m_Windows)
				{
					if (win->GetName() == layer->ParentWindow)
					{
						wd = &win->m_WinData;

						// Use any command queue
						VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

						VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
						cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
						cmdBufAllocateInfo.commandPool = command_pool;
						cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
						cmdBufAllocateInfo.commandBufferCount = 1;

						VkCommandBuffer &command_buffer = win->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
						auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

						VkCommandBufferBeginInfo begin_info = {};
						begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
						begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
						err = vkBeginCommandBuffer(command_buffer, &begin_info);
						check_vk_result(err);

						return command_buffer;
					}
				}
			}
		}

		return nullptr;
	}

	/*VkCommandBuffer Application::GetCommandBuffer(bool begin)
	{
		ImGui_ImplVulkanH_Window *wd = &g_MainWindowData;

		// Use any command queue
		VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

		VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
		cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufAllocateInfo.commandPool = command_pool;
		cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer &command_buffer = win->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
		auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(command_buffer, &begin_info);
		check_vk_result(err);

		return command_buffer;
	}*/

	VkCommandBuffer Application::GetCommandBuffer(bool begin, ImGui_ImplVulkanH_Window *wd, const std::string &winname)
	{

		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				// Use any command queue
				VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

				VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
				cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmdBufAllocateInfo.commandPool = command_pool;
				cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmdBufAllocateInfo.commandBufferCount = 1;

				VkCommandBuffer &command_buffer = win->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
				auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

				VkCommandBufferBeginInfo begin_info = {};
				begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				err = vkBeginCommandBuffer(command_buffer, &begin_info);
				check_vk_result(err);

				return command_buffer;
			}
		}
		return nullptr;
	}

	void Application::FlushCommandBuffer(VkCommandBuffer commandBuffer, const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

				VkSubmitInfo end_info = {};
				end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				end_info.commandBufferCount = 1;
				end_info.pCommandBuffers = &commandBuffer;
				auto err = vkEndCommandBuffer(commandBuffer);
				check_vk_result(err);

				// Create fence to ensure that the command buffer has finished executing
				VkFenceCreateInfo fenceCreateInfo = {};
				fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCreateInfo.flags = 0;
				VkFence fence;
				err = vkCreateFence(g_Device, &fenceCreateInfo, nullptr, &fence);
				check_vk_result(err);

				err = vkQueueSubmit(g_Queue, 1, &end_info, fence);
				check_vk_result(err);

				err = vkWaitForFences(g_Device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
				check_vk_result(err);

				vkDestroyFence(g_Device, fence, nullptr);
			}
		}
	}

	void Application::SubmitResourceFree(std::function<void()> &&func, const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (win->GetName() == winname)
			{
				s_ResourceFreeQueue[s_CurrentFrameIndex].emplace_back(func);
			}
		}
	}

	ImFont *Application::GetFont(const std::string &name)
	{
		// if (!s_Fonts.contains(name))
		//		return nullptr;

		return s_Fonts.at(name);
	}

	GLFWwindow *GetWindowHandle(const std::string &winname)
	{
		for (auto win : app->m_Windows)
		{
			if (winname == win->GetName())
			{
				return win->GetWindowHandle();
			}
		}
	}

	void Window::OnWindowResize(GLFWwindow *windowHandle, int width, int height)
	{
		if (width == 0 || height == 0)
			return;

		vkDeviceWaitIdle(g_Device);

		this->CleanupVulkanWindow();

		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(g_Instance, m_WindowHandle, g_Allocator, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to recreate window surface!");
		}

		SetupVulkanWindow(&m_WinData, surface, width, height, this);

		// Mettez à jour les ressources ImGui si nécessaire
		ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
		ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &m_WinData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
	}

ImDrawData* Application::RenderWindow(Window *window)
{
    // Set the ImGui context for this window
    ImGui::SetCurrentContext(m_ImGuiContext);

	if(app->m_Windows.size() > 1){
		std::cout << "DFD" << app->m_Windows.size()  << std::endl;
		ImGui::UIKIT_UpdateFrame();
	}

	//ImGui::NewFrame();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGuiViewportP* p_viewport = m_ImGuiContext->Viewports[window->WinID];
	std::cout << " -> -> -> " << m_ImGuiContext->Viewports.size() << std::endl;


    // Mark rendering data as invalid to prevent user who may have a handle on it to use it.
    for (int n = 0; n < m_ImGuiContext->Viewports.Size; n++)
    {
        ImGuiViewportP* viewport = m_ImGuiContext->Viewports[n];
        viewport->DrawData = NULL;
        viewport->DrawDataP.Clear();
    }


    int width, height;
    glfwGetWindowSize(window->GetWindowHandle(), &width, &height);

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    if (!m_Specification.CustomTitlebar && m_MenubarCallback)
        window_flags |= ImGuiWindowFlags_MenuBar;

    const bool isMaximized = (bool)glfwGetWindowAttrib(window->GetWindowHandle(), GLFW_MAXIMIZED);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    std::string label = "DockSpaceWindow." + window->GetName();
    ImGui::SetNextWindowDockID(0);
    ImGui::Begin(label.c_str(), nullptr, window_flags);

    ImGuiWindow *win = ImGui::GetCurrentWindow();

    ImGui::PopStyleColor(); // MenuBarBg
    ImGui::PopStyleVar(2);

    {
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
        if (!isMaximized)
            UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());
        ImGui::PopStyleColor(); // ImGuiCol_Border
    }

    if (m_Specification.CustomTitlebar)
    {
        float titleBarHeight;
        window->UI_DrawTitlebar(titleBarHeight);
        ImGui::SetCursorPosY(titleBarHeight);
    }

    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle &style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;

    AppPushTabStyle();
    ImGui::DockSpace(ImGui::GetID("MainDockspace"));
    AppPopTabStyle();

    style.WindowMinSize.x = minWinSizeX;

    if (!m_Specification.CustomTitlebar)
        window->UI_DrawMenubar();

    for (auto &layer : m_LayerStack)
    {
        if (!layer->initialized)
        {
            layer->ParentWindow = window->GetName();
            layer->initialized = true;
        }

        static bool windowJustUndocked = false;

        layer->m_WindowControlCallbalck = [window](ImGuiWindow *win)
        {
            if (win)
            {
                std::cout << "==========================================" << std::endl;
                std::cout << "[WINDOW] Name: " << win->Name << std::endl;
                std::cout << "[WINDOW] Dock Node : " << win->DockNode << std::endl;
                std::cout << "[WINDOW] Dock Tree Name : " << win->RootWindowDockTree->Name << std::endl;
                std::string str = win->RootWindowDockTree->Name;
                std::string str_finded = str.substr(str.find(".") + 1, str.size());
                std::cout << "[WINDOW] Deducted tree name : " << str_finded << std::endl;

                std::shared_ptr<Window> finded_win = nullptr;

                for (auto win : app->m_Windows)
                {
                    if (str_finded == win->GetName())
                    {
                        finded_win = win;
                    }
                }

                if (finded_win)
                {
                    std::cout << "The window is attached to a parent window ! (" << finded_win->GetName() << ")" << std::endl;
                }
                else
                {
                    std::cout << "The window is alone :(" << std::endl;
                    // Assign the subwindow in a new window !
                }

                std::cout << "[WINDOW] Dock ID : " << win->DockId << std::endl;
                std::cout << "[WINDOW] Dock Host ID : " << win->DockNodeAsHost << std::endl;
                std::cout << "[WINDOW] Identified Parent window : " << win->DockNodeAsHost << std::endl;

                // Check if the window is being moved (header bar clicked and dragging)
                if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    std::cout << "[WINDOW] The window is being moved!" << std::endl;
                }
                else
                {
                    std::cout << "[WINDOW] The window is not being moved." << std::endl;
                }

                std::cout << "==========================================" << std::endl;
            }
            else
            {
                std::cout << "==========================================" << std::endl;
                std::cout << "[WINDOW]: invalid" << std::endl;
                std::cout << "==========================================" << std::endl;
            }
        };

        if (layer->ParentWindow == window->GetName())
        {
            layer->OnUIRender();
        }
    }

    ImGui::End();

    // Don't execute this if it's the subwindow imgui which is no longer docked to the dockspace MainDockspace
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemHovered())
    {
        window->m_IsDragging = true;
        window->m_InitialMousePos = io.MousePos;
        window->m_InitialWindowPos = ImVec2(win->Pos.x, win->Pos.y);
    }

    if (window->m_IsDragging && !window->m_ResizePending)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            ImVec2 delta = ImVec2(io.MousePos.x - window->m_InitialMousePos.x, io.MousePos.y - window->m_InitialMousePos.y);
            if (delta.x != 0 || delta.y != 0)
            { // Only update position if there is a change
                ImVec2 newWindowPos = ImVec2(window->m_InitialWindowPos.x + delta.x, window->m_InitialWindowPos.y + delta.y);
                glfwSetWindowPos(window->GetWindowHandle(), (int)newWindowPos.x, (int)newWindowPos.y);
            }
        }
        else
        {
            window->m_IsDragging = false;
        }
    }

    // Detect window resize
    if (win->Size.x != window->m_PreviousWidth || win->Size.y != window->m_PreviousHeight)
    {
        glfwSetWindowSize(window->GetWindowHandle(), win->Size.x, win->Size.y);
    }

    // Render Draw data 
    ImGui::Render();

    // Copy the draw data for this window
    window->DrawData = *ImGui::GetDrawData();
	std::cout << "DRAW DATA PTR / " << &window->DrawData << std::endl;
	std::cout << "DRAW DATA CmdListsCount / " << &window->DrawData.CmdListsCount << std::endl;
	viewport->DrawData->Clear();

    return &window->DrawData;
}


	void Window::CleanupVulkanWindow()
	{
		ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &m_WinData, g_Allocator);
	}

}

// Handle any inputs with events array and window manager ?