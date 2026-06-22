//
//  app.cpp
//  Sources for main application features of Cherry
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "app.hpp"

#include <stdio.h>   // printf, fprintf
#include <stdlib.h>  // abort
#include <vulkan/vulkan.h>

#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>  // thread

#include "../../../lib/sdl2/include/SDL.h"
#include "../../../lib/sdl2/include/SDL_vulkan.h"
#include "../app_window/app_window.hpp"
#include "../base.hpp"
#include "../components/components.hpp"
#include "../hooks/hooks.hpp"
#include "../shaders/shaders.hpp"
#include "../window/window.hpp"

#ifdef CHERRY_ENABLE_NET

#include <curl/curl.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "../../../lib/httpcl/httpcl.h"

#ifdef _WIN32
#include <windows.h>
#define NOMINMAX  // std::max

#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif  // _WIN32
#endif  // CHERRY_ENABLE_NET

#if defined(__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h>
#endif

// Emedded font
#include "../imgui/embed/Hack-Regular.embed"
#include "../imgui/embed/Inconsolatas.embed"
#include "../imgui/embed/Roboto-Bold.embed"
#include "../imgui/embed/Roboto-Italic.embed"
#include "../imgui/embed/Roboto-Regular.embed"
#include "../imgui/theme/deftheme.hpp"

#ifdef CHERRY_DEBUG
#include "../debug/debug.hpp"
#endif

namespace fs = std::filesystem;

// #define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

static Cherry::Application *s_Instance = nullptr;
static Cherry::Application *app;

static bool c_ValidDropZoneFounded = false;
static bool c_DockIsDragging = false;
static bool c_MasterSwapChainRebuild = false;
static bool m_TriggerDevtools = false;
static std::shared_ptr<Cherry::WindowDragDropState> c_CurrentDragDropState;
static Cherry::Window s_EmptyWindow;
static Cherry::Component s_EmptyComponent;
static Cherry::AppWindow s_EmptyAppWindow;
static std::shared_ptr<Cherry::Window> c_CurrentRenderedWindow;
static std::shared_ptr<Cherry::AppWindow> c_CurrentRenderedAppWindow;
static Cherry::Component *c_LastComponent;
static std::unique_ptr<Cherry::Component> c_NextComponent;
static Cherry::Component *c_CurrentComponent;

static std::string g_ExecutablePath;

static VkAllocationCallbacks *g_Allocator = NULL;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = (uint32_t)-1;
static VkQueue g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static std::unordered_map<std::string, ImFont *> s_Fonts;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
static std::vector<std::shared_ptr<Cherry::AppWindow>> g_TempAppWindows;  // To be able to create app window before create
                                                                          // applications

static bool g_LogicalDeviceInitialized = false;
static int g_MinImageCount = 0;
static int c_WindowsCount = 0;
static std::string LastWindowPressed = "";
static int RedockCount = 0;
static bool DragRendered = false;

static std::shared_ptr<Cherry::RedockRequest> LatestRequest;
static std::vector<std::pair<Cherry::ProcessCallback, std::function<void()>>> g_ProcessCallbacks;

// Per-frame-in-flight

// Unlike g_MainWindowData.FrameIndex, this is not the the swapchain image index
// and is always guaranteed to increase (eg. 0, 1, 2, 0, 1, 2)

void check_vk_result(VkResult err) {
  if (err == 0)
    return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0)
    abort();
}
#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t object,
    size_t location,
    int32_t messageCode,
    const char *pLayerPrefix,
    const char *pMessage,
    void *pUserData) {
  (void)flags;
  (void)object;
  (void)location;
  (void)messageCode;
  (void)pUserData;
  (void)pLayerPrefix;  // Unused arguments
                       //	fprintf(stderr, "[vulkan] Debug report from
                       // ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
  return VK_FALSE;
}
#endif  // IMGUI_VULKAN_DEBUG_REPORT

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace Cherry {

#include "../embed/not_found_img.embed"
#include "../embed/window.embed"

  static void HandleSnapToEdge(Window *window) {
    if (!window->GetSpecifications().CustomTitlebar || !window->GetSpecifications().DisableWindowManagerTitleBar)
      return;

    SDL_Window *sdlWin = window->GetWindowHandle();
    SnapState &snap = window->GetSnapState();

    bool isMoving = window->GetIsMoving();

    if (!isMoving) {
      snap.isDragging = false;
      return;
    }

    if (!snap.isDragging) {
      SDL_GetWindowPosition(sdlWin, &snap.preSnapX, &snap.preSnapY);
      SDL_GetWindowSize(sdlWin, &snap.preSnapW, &snap.preSnapH);
      snap.isDragging = true;

      if (snap.isSnapped) {
        snap.isSnapped = false;
        SDL_SetWindowSize(sdlWin, snap.preSnapW, snap.preSnapH);
      }
    }

    int mouseX, mouseY;
    SDL_GetGlobalMouseState(&mouseX, &mouseY);

    int displayIndex = SDL_GetWindowDisplayIndex(sdlWin);
    if (displayIndex < 0)
      displayIndex = 0;

    SDL_Rect displayBounds;
    SDL_GetDisplayUsableBounds(displayIndex, &displayBounds);

    const int SNAP_THRESHOLD = 20;  // px

    int winW, winH;
    SDL_GetWindowSize(sdlWin, &winW, &winH);

    bool nearLeft = mouseX <= displayBounds.x + SNAP_THRESHOLD;
    bool nearRight = mouseX >= displayBounds.x + displayBounds.w - SNAP_THRESHOLD;
    bool nearTop = mouseY <= displayBounds.y + SNAP_THRESHOLD;
    bool nearBottom = mouseY >= displayBounds.y + displayBounds.h - SNAP_THRESHOLD;

    SDL_Rect target = {};
    bool shouldSnap = false;

    int halfW = displayBounds.w / 2;
    int halfH = displayBounds.h / 2;

    if (nearLeft && nearTop) {
      target = { displayBounds.x, displayBounds.y, halfW, halfH };
      shouldSnap = true;
    } else if (nearRight && nearTop) {
      target = { displayBounds.x + halfW, displayBounds.y, halfW, halfH };
      shouldSnap = true;
    } else if (nearLeft && nearBottom) {
      target = { displayBounds.x, displayBounds.y + halfH, halfW, halfH };
      shouldSnap = true;
    } else if (nearRight && nearBottom) {
      target = { displayBounds.x + halfW, displayBounds.y + halfH, halfW, halfH };
      shouldSnap = true;
    } else if (nearLeft) {
      target = { displayBounds.x, displayBounds.y, halfW, displayBounds.h };
      shouldSnap = true;
    } else if (nearRight) {
      target = { displayBounds.x + halfW, displayBounds.y, halfW, displayBounds.h };
      shouldSnap = true;
    } else if (nearTop) {
      target = { displayBounds.x, displayBounds.y, displayBounds.w, displayBounds.h };
      shouldSnap = true;
    }

    if (shouldSnap) {
      snap.snapTarget = target;
    } else {
      snap.snapTarget = {};
    }
  }

  static void ApplySnapOnRelease(Window *window) {
    if (!window->GetSpecifications().CustomTitlebar || !window->GetSpecifications().DisableWindowManagerTitleBar)
      return;

    SnapState &snap = window->GetSnapState();

    if (!snap.isDragging || snap.isSnapped)
      return;

    bool wasMoving = window->GetIsMoving();
    if (wasMoving)
      return;  // not released yet

    SDL_Window *sdlWin = window->GetWindowHandle();

    if (snap.snapTarget.w > 0) {
      SDL_SetWindowPosition(sdlWin, snap.snapTarget.x, snap.snapTarget.y);
      SDL_SetWindowSize(sdlWin, snap.snapTarget.w, snap.snapTarget.h);
      snap.isSnapped = true;
      snap.snapTarget = {};
      window->GetSwapChainRebuild() = true;
    }

    snap.isDragging = false;
  }

  Application::Application(const ApplicationSpecification &specification) : m_DefaultSpecification(specification) {
    s_Instance = this;
    m_RootPath = Application::CookPath("");

    if (specification.MainRenderCallback) {
      m_MainRenderCallback = specification.MainRenderCallback;
    }

    if (specification.RenderMode == WindowRenderingMethod::DockingWindows ||
        specification.RenderMode == WindowRenderingMethod::TabWidows ||
        specification.RenderMode == WindowRenderingMethod::SimpleWindow) {
      for (auto preinit_app_windows : Application::GetTempAppWindows()) {
        this->PutWindow(preinit_app_windows);
      }
      Application::GetTempAppWindows().clear();

      if (m_AppWindows.empty()) {
        EnableNoAppWindowSafety();
      }
    }

    ResetLastComponent();
    ResetCurrentComponent();
    ResetNextComponent();

    // Default theme (first position in m_ActiveThemes, can't be removed)
    AddTheme(DefaultCherryTheme());
    PushTheme("default");

    // Add themes from user.
    if (!specification.Themes.empty()) {
      for (auto theme : specification.Themes) {
        AddTheme(theme);
      }

      /*if (specification.SelectedTheme != "undefined") {
        PushTheme(specification.SelectedTheme);
      } else {
        PushTheme(specification.Themes[0].m_ThemeID);
      }*/
    }

    m_SelectedTheme = specification.SelectedTheme;

    Init();
  }

  Application::~Application() {
    Shutdown();
  }

  void Application::SetMasterSwapChainRebuild(const bool &new_state) {
    c_MasterSwapChainRebuild = new_state;
  }

  void Application::IncrementWindowNumber() {
    c_WindowsCount++;
  }

  void Application::SetupVulkan(const char **extensions, uint32_t extensions_count) {
    VkResult err;

    // Create Vulkan Instance
    if (!g_LogicalDeviceInitialized) {
      VkInstanceCreateInfo create_info = {};
      create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info.enabledExtensionCount = extensions_count;
      create_info.ppEnabledExtensionNames = extensions;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
      // Enabling validation layers
      const char *layers[] = { "VK_LAYER_KHRONOS_validation" };
      create_info.enabledLayerCount = 1;
      create_info.ppEnabledLayerNames = layers;

      // Enable debug report extension (we need additional storage, so we
      // duplicate the user array to add our new extension to it)
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
      auto vkCreateDebugReportCallbackEXT =
          (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
      IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

      // Setup the debug report callback
      VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
      debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
      debug_report_ci.flags =
          VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
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
    if (!g_LogicalDeviceInitialized) {
      uint32_t gpu_count;
      err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, NULL);
      check_vk_result(err);
      IM_ASSERT(gpu_count > 0);

      VkPhysicalDevice *gpus = (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * gpu_count);
      err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus);
      check_vk_result(err);

      // If a number >1 of GPUs got reported, find discrete GPU if present, or use
      // first one available. This covers most common cases
      // (multi-gpu/integrated+dedicated graphics). Handling more complicated
      // setups (multiple dedicated GPUs) is out of scope of this sample.
      int use_gpu = 0;
      for (int i = 0; i < (int)gpu_count; i++) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(gpus[i], &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
          use_gpu = i;
          break;
        }
      }

      g_PhysicalDevice = gpus[use_gpu];
      free(gpus);
    }

    // Select graphics queue family
    if (!g_LogicalDeviceInitialized) {
      uint32_t count;
      vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, NULL);
      VkQueueFamilyProperties *queues = (VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * count);
      vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
      for (uint32_t i = 0; i < count; i++)
        if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          g_QueueFamily = i;
          break;
        }
      free(queues);
      IM_ASSERT(g_QueueFamily != (uint32_t)-1);
    }

    // Create Logical Device (with 1 queue)
    if (!g_LogicalDeviceInitialized) {
      int device_extension_count = 1;
      const char *device_extensions[] = { "VK_KHR_swapchain" };
      const float queue_priority[] = { 1.0f };
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
      g_LogicalDeviceInitialized = true;
    }

    // Create Descriptor Pool
    {
      VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                                            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                                            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                                            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                                            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                                            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                                            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                                            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                                            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                                            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                                            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };
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

  // All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used
  // by the demo. Your real engine/app may not use them.
  void Application::SetupVulkanWindow(
      ImGui_ImplVulkanH_Window *wd,
      VkSurfaceKHR surface,
      int width,
      int height,
      Cherry::Window *win) {
    wd->Surface = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
    if (res != VK_TRUE) {
      fprintf(stderr, "Error no WSI support on physical device 0\n");
      exit(-1);
    }

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = {
      VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM
    };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
        g_PhysicalDevice,
        wd->Surface,
        requestSurfaceImageFormat,
        (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
        requestSurfaceColorSpace);

    // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR,
                                         VK_PRESENT_MODE_IMMEDIATE_KHR,
                                         VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
    wd->PresentMode =
        ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

    // Create SwapChain, RenderPass, Framebuffer, etc.
    IM_ASSERT(g_MinImageCount >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(
        g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
  }

  void Application::CleanupVulkanWindow(Cherry::Window *win) {
    if (!win)
      return;

    win->GetWinData()->Surface = win->GetSurface();

    ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, win->GetWinData(), g_Allocator);

    win->SetSurface(VK_NULL_HANDLE);
  }

  void Application::CleanupSpecificVulkanWindow(Cherry::Window *win) {
    CleanupVulkanWindow(win);
  }

  void Application::CleanupVulkan(Cherry::Window *win) {
    if (g_Device == VK_NULL_HANDLE)
      return;

    if (g_DescriptorPool != VK_NULL_HANDLE) {
      vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);
      g_DescriptorPool = VK_NULL_HANDLE;
    }

    if (g_PipelineCache != VK_NULL_HANDLE) {
      vkDestroyPipelineCache(g_Device, g_PipelineCache, g_Allocator);
      g_PipelineCache = VK_NULL_HANDLE;
    }

#ifdef IMGUI_VULKAN_DEBUG_REPORT
    auto vkDestroyDebugReportCallbackEXT =
        (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
    if (vkDestroyDebugReportCallbackEXT && g_DebugReport != VK_NULL_HANDLE) {
      vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
      g_DebugReport = VK_NULL_HANDLE;
    }
#endif

    vkDestroyDevice(g_Device, g_Allocator);
    g_Device = VK_NULL_HANDLE;

    if (g_Instance != VK_NULL_HANDLE) {
      vkDestroyInstance(g_Instance, g_Allocator);
      g_Instance = VK_NULL_HANDLE;
    }
  }

  Application &Application::Get() {
    return *s_Instance;
  }

  VkDevice &Application::GetVkDevice() {
    return g_Device;
  }

  VkPhysicalDevice &Application::GetVkPhysicalDevice() {
    return g_PhysicalDevice;
  }

  uint32_t &Application::GetQueueFamily() {
    return g_QueueFamily;
  }

  VkQueue &Application::GetVkQueue() {
    return g_Queue;
  }

  VkAllocationCallbacks &Application::GetVkAllocationCallbacks() {
    return *g_Allocator;
  }

  VkDebugReportCallbackEXT &Application::GetVkDebugReportCallbackEXT() {
    return g_DebugReport;
  }

  void Application::SetExecutablePath() {
    g_ExecutablePath = "test";
  }
  std::string &Application::GetExecutablePath() {
    return g_ExecutablePath;
  }

  VkPipelineCache &Application::GetVkPipelineCache() {
    return g_PipelineCache;
  }

  VkDescriptorPool &Application::GetVkDescriptorPool() {
    return g_DescriptorPool;
  }

  int &Application::GetMinImageCount() {
    return g_MinImageCount;
  }

  std::shared_ptr<Cherry::Window> &Application::GetCurrentRenderedWindow() {
    return c_CurrentRenderedWindow;
  }

  Cherry::Component &Application::GetSafeLastComponent() {
    return c_LastComponent ? *c_LastComponent : s_EmptyComponent;
  }

  void Application::SetLastComponent(Component *component) {
    c_LastComponent = component;
  }

  void Application::ResetLastComponent() {
    c_LastComponent = nullptr;
  }

  Cherry::Component &Application::GetSafeNextComponent() {
    return c_NextComponent ? *c_NextComponent : s_EmptyComponent;
  }

  void Application::RenderComponent(const std::shared_ptr<Component> &component) {
    if (component)
      component->RenderWrapper();
  }

  void Application::RenderComponent(Component &component) {
    component.RenderWrapper();
  }

  void Application::SetNextComponent(Component *component) {
    if (component) {
      c_NextComponent = std::make_unique<Component>(*component);
    } else {
      c_NextComponent.reset();
    }
  }

  void Application::ResetNextComponent() {
    c_NextComponent = std::make_unique<Cherry::Component>(s_EmptyComponent);
  }

  Cherry::Component &Application::GetSafeCurrentComponent() {
    return c_CurrentComponent ? *c_CurrentComponent : s_EmptyComponent;
  }

  void Application::SetCurrentComponent(Component *component) {
    c_CurrentComponent = component;
  }

  void Application::ResetCurrentComponent() {
    c_CurrentComponent = nullptr;
  }

  std::shared_ptr<Cherry::AppWindow> &Application::GetCurrentRenderedAppWindow() {
    return c_CurrentRenderedAppWindow;
  }

  void Application::SetCurrentRenderedAppWindow(std::shared_ptr<Cherry::AppWindow> appWindow) {
    c_CurrentRenderedAppWindow = appWindow;
  }

  Cherry::Window &Application::GetSafeCurrentRenderedWindow() {
    return c_CurrentRenderedWindow ? *c_CurrentRenderedWindow : s_EmptyWindow;
  }

  Cherry::AppWindow &Application::GetSafeCurrentRenderedAppWindow() {
    return c_CurrentRenderedAppWindow ? *c_CurrentRenderedAppWindow : s_EmptyAppWindow;
  }

  std::shared_ptr<Cherry::WindowDragDropState> &Application::GetCurrentDragDropState() {
    return c_CurrentDragDropState;
  }

  bool &Application::GetDockIsDragging() {
    return c_DockIsDragging;
  }

  std::string &Application::GetLastWindowPressed() {
    return LastWindowPressed;
  }

  std::vector<std::shared_ptr<Cherry::AppWindow>> &Application::GetTempAppWindows() {
    return g_TempAppWindows;
  }

  std::vector<std::pair<std::string, std::pair<std::string, float>>> &Application::GetCustomFonts() {
    return m_CustomFonts;
  }

  bool &Application::GetValidDropZoneFounded() {
    return c_ValidDropZoneFounded;
  }

  void Application::SetValidDropZoneFounded(const bool &founded) {
    c_ValidDropZoneFounded = founded;
  }

  void Application::SetCurrentDragDropState(const std::shared_ptr<Cherry::WindowDragDropState> &state) {
    c_CurrentDragDropState = state;
  }

  void Application::SetCurrentDragDropStateDragOwner(const std::string &new_name) {
    c_CurrentDragDropState->DragOwner = new_name;
  }

  void Application::SetCurrentDragDropStateDockIsDragging(const bool &is_dragging) {
    c_CurrentDragDropState->DockIsDragging = is_dragging;
  }

  void Application::SetCurrentDragDropStateAppWindow(const std::string &new_name) {
    c_CurrentDragDropState->LastDraggingAppWindow = new_name;
  }

  void Application::SetCurrentDragDropStateAppWindowHost(const std::string &new_name) {
    c_CurrentDragDropState->LastDraggingAppWindowHost = new_name;
  }

  void Application::SetCurrentDragDropStateWindow(const std::string &new_name) {
    c_CurrentDragDropState->LastDraggingWindow = new_name;
  }

  void Application::SetLastWindowPressed(const std::string &name) {
    LastWindowPressed = name;
  }

  void Application::SetCurrentDragDropStateDraggingPlace(const DockEmplacement &place) {
    c_CurrentDragDropState->LastDraggingPlace = place;
  }

  std::unordered_map<std::string, ImFont *> &Application::GetFontList() {
    return s_Fonts;
  }

  void Application::AddProcessCallback(ProcessCallback process, const std::function<void()> callback) {
    g_ProcessCallbacks.push_back({ process, callback });
  }

  const std::unordered_map<CherryKey, SDL_Scancode> &Application::GetKeyMap() {
    static std::unordered_map<CherryKey, SDL_Scancode> keyMap = { { CherryKey::A, SDL_SCANCODE_A },
                                                                  { CherryKey::B, SDL_SCANCODE_B },
                                                                  { CherryKey::C, SDL_SCANCODE_C },
                                                                  { CherryKey::D, SDL_SCANCODE_D },
                                                                  { CherryKey::E, SDL_SCANCODE_E },
                                                                  { CherryKey::F, SDL_SCANCODE_F },
                                                                  { CherryKey::G, SDL_SCANCODE_G },
                                                                  { CherryKey::H, SDL_SCANCODE_H },
                                                                  { CherryKey::I, SDL_SCANCODE_I },
                                                                  { CherryKey::J, SDL_SCANCODE_J },
                                                                  { CherryKey::K, SDL_SCANCODE_K },
                                                                  { CherryKey::L, SDL_SCANCODE_L },
                                                                  { CherryKey::M, SDL_SCANCODE_M },
                                                                  { CherryKey::N, SDL_SCANCODE_N },
                                                                  { CherryKey::O, SDL_SCANCODE_O },
                                                                  { CherryKey::P, SDL_SCANCODE_P },
                                                                  { CherryKey::Q, SDL_SCANCODE_Q },
                                                                  { CherryKey::R, SDL_SCANCODE_R },
                                                                  { CherryKey::S, SDL_SCANCODE_S },
                                                                  { CherryKey::T, SDL_SCANCODE_T },
                                                                  { CherryKey::U, SDL_SCANCODE_U },
                                                                  { CherryKey::V, SDL_SCANCODE_V },
                                                                  { CherryKey::W, SDL_SCANCODE_W },
                                                                  { CherryKey::X, SDL_SCANCODE_X },
                                                                  { CherryKey::Y, SDL_SCANCODE_Y },
                                                                  { CherryKey::Z, SDL_SCANCODE_Z },
                                                                  { CherryKey::NUM_0, SDL_SCANCODE_0 },
                                                                  { CherryKey::NUM_1, SDL_SCANCODE_1 },
                                                                  { CherryKey::NUM_2, SDL_SCANCODE_2 },
                                                                  { CherryKey::NUM_3, SDL_SCANCODE_3 },
                                                                  { CherryKey::NUM_4, SDL_SCANCODE_4 },
                                                                  { CherryKey::NUM_5, SDL_SCANCODE_5 },
                                                                  { CherryKey::NUM_6, SDL_SCANCODE_6 },
                                                                  { CherryKey::NUM_7, SDL_SCANCODE_7 },
                                                                  { CherryKey::NUM_8, SDL_SCANCODE_8 },
                                                                  { CherryKey::NUM_9, SDL_SCANCODE_9 },
                                                                  { CherryKey::ESCAPE, SDL_SCANCODE_ESCAPE },
                                                                  { CherryKey::SPACE, SDL_SCANCODE_SPACE },
                                                                  { CherryKey::KEY_DELETE, SDL_SCANCODE_DELETE },
                                                                  { CherryKey::ENTER, SDL_SCANCODE_RETURN },
                                                                  { CherryKey::BACKSPACE, SDL_SCANCODE_BACKSPACE },
                                                                  { CherryKey::TAB, SDL_SCANCODE_TAB },
                                                                  { CherryKey::SHIFT, SDL_SCANCODE_LSHIFT },
                                                                  { CherryKey::CTRL, SDL_SCANCODE_LCTRL },
                                                                  { CherryKey::ALT, SDL_SCANCODE_LALT },
                                                                  { CherryKey::LEFT, SDL_SCANCODE_LEFT },
                                                                  { CherryKey::RIGHT, SDL_SCANCODE_RIGHT },
                                                                  { CherryKey::UP, SDL_SCANCODE_UP },
                                                                  { CherryKey::DOWN, SDL_SCANCODE_DOWN },
                                                                  { CherryKey::F1, SDL_SCANCODE_F1 },
                                                                  { CherryKey::F2, SDL_SCANCODE_F2 },
                                                                  { CherryKey::F3, SDL_SCANCODE_F3 },
                                                                  { CherryKey::F4, SDL_SCANCODE_F4 },
                                                                  { CherryKey::F5, SDL_SCANCODE_F5 },
                                                                  { CherryKey::F6, SDL_SCANCODE_F6 },
                                                                  { CherryKey::F7, SDL_SCANCODE_F7 },
                                                                  { CherryKey::F8, SDL_SCANCODE_F8 },
                                                                  { CherryKey::F9, SDL_SCANCODE_F9 },
                                                                  { CherryKey::F10, SDL_SCANCODE_F10 },
                                                                  { CherryKey::F11, SDL_SCANCODE_F11 },
                                                                  { CherryKey::F12, SDL_SCANCODE_F12 } };
    return keyMap;
  }

  std::optional<CherryKey> Application::StringToCherryKey(std::string str) {
    if (str.empty())
      return std::nullopt;

    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });

    static const std::unordered_map<std::string, CherryKey> stringToEnumMap = { { "A", CherryKey::A },
                                                                                { "B", CherryKey::B },
                                                                                { "C", CherryKey::C },
                                                                                { "D", CherryKey::D },
                                                                                { "E", CherryKey::E },
                                                                                { "F", CherryKey::F },
                                                                                { "G", CherryKey::G },
                                                                                { "H", CherryKey::H },
                                                                                { "I", CherryKey::I },
                                                                                { "J", CherryKey::J },
                                                                                { "K", CherryKey::K },
                                                                                { "L", CherryKey::L },
                                                                                { "M", CherryKey::M },
                                                                                { "N", CherryKey::N },
                                                                                { "O", CherryKey::O },
                                                                                { "P", CherryKey::P },
                                                                                { "Q", CherryKey::Q },
                                                                                { "R", CherryKey::R },
                                                                                { "S", CherryKey::S },
                                                                                { "T", CherryKey::T },
                                                                                { "U", CherryKey::U },
                                                                                { "V", CherryKey::V },
                                                                                { "W", CherryKey::W },
                                                                                { "X", CherryKey::X },
                                                                                { "Y", CherryKey::Y },
                                                                                { "Z", CherryKey::Z },

                                                                                { "0", CherryKey::NUM_0 },
                                                                                { "NUM_0", CherryKey::NUM_0 },
                                                                                { "1", CherryKey::NUM_1 },
                                                                                { "NUM_1", CherryKey::NUM_1 },
                                                                                { "2", CherryKey::NUM_2 },
                                                                                { "NUM_2", CherryKey::NUM_2 },
                                                                                { "3", CherryKey::NUM_3 },
                                                                                { "NUM_3", CherryKey::NUM_3 },
                                                                                { "4", CherryKey::NUM_4 },
                                                                                { "NUM_4", CherryKey::NUM_4 },
                                                                                { "5", CherryKey::NUM_5 },
                                                                                { "NUM_5", CherryKey::NUM_5 },
                                                                                { "6", CherryKey::NUM_6 },
                                                                                { "NUM_6", CherryKey::NUM_6 },
                                                                                { "7", CherryKey::NUM_7 },
                                                                                { "NUM_7", CherryKey::NUM_7 },
                                                                                { "8", CherryKey::NUM_8 },
                                                                                { "NUM_8", CherryKey::NUM_8 },
                                                                                { "9", CherryKey::NUM_9 },
                                                                                { "NUM_9", CherryKey::NUM_9 },

                                                                                { "ESC", CherryKey::ESCAPE },
                                                                                { "ESCAPE", CherryKey::ESCAPE },
                                                                                { "SPACE", CherryKey::SPACE },
                                                                                { " ", CherryKey::SPACE },
                                                                                { "ENTER", CherryKey::ENTER },
                                                                                { "RETURN", CherryKey::ENTER },
                                                                                { "BACKSPACE", CherryKey::BACKSPACE },
                                                                                { "DELETE", CherryKey::KEY_DELETE },
                                                                                { "DEL", CherryKey::KEY_DELETE },
                                                                                { "TAB", CherryKey::TAB },
                                                                                { "SHIFT", CherryKey::SHIFT },
                                                                                { "LSHIFT", CherryKey::SHIFT },
                                                                                { "CTRL", CherryKey::CTRL },
                                                                                { "CONTROL", CherryKey::CTRL },
                                                                                { "ALT", CherryKey::ALT },

                                                                                { "LEFT", CherryKey::LEFT },
                                                                                { "RIGHT", CherryKey::RIGHT },
                                                                                { "UP", CherryKey::UP },
                                                                                { "DOWN", CherryKey::DOWN },

                                                                                { "F1", CherryKey::F1 },
                                                                                { "F2", CherryKey::F2 },
                                                                                { "F3", CherryKey::F3 },
                                                                                { "F4", CherryKey::F4 },
                                                                                { "F5", CherryKey::F5 },
                                                                                { "F6", CherryKey::F6 },
                                                                                { "F7", CherryKey::F7 },
                                                                                { "F8", CherryKey::F8 },
                                                                                { "F9", CherryKey::F9 },
                                                                                { "F10", CherryKey::F10 },
                                                                                { "F11", CherryKey::F11 },
                                                                                { "F12", CherryKey::F12 } };

    auto it = stringToEnumMap.find(str);
    if (it != stringToEnumMap.end()) {
      return it->second;
    }

    return std::nullopt;
  }

  void Application::ExecuteProcessCallbacks(ProcessCallback process) {
    for (auto callback : g_ProcessCallbacks) {
      if (callback.first == process) {
        if (callback.second) {
          callback.second();
        }
      }
    }
  }

  void Application::PushRedockEvent(const std::shared_ptr<Cherry::WindowDragDropState> &state) {
    for (auto app_win : s_Instance->m_AppWindows) {
      if (app_win->m_IdName == state->LastDraggingAppWindowHost) {
        std::shared_ptr<Cherry::RedockRequest> req = app_win->CreateRedockEvent(
            state->LastDraggingWindow,
            state->LastDraggingPlace,
            state->FromSave,
            state->LastDraggingAppWindow,
            c_CurrentDragDropState->CreateNewWindow);
        LatestRequest = req;
        s_Instance->m_RedockRequests.push_back(req);
        RedockCount++;
      }
    }
  }

  void Application::Init() {
    // Set main context application ptr
    app = &this->Get();
#ifdef CHERRY_DEBUG
    Cherry::Application::DebugToolState().store(false);
#endif
#ifdef CHERRY_ENABLE_AUDIO
    // Init audio service if needed
    if (app->m_DefaultSpecification.UseAudioService) {
      StartAudioService();
    }
#endif

#ifdef CHERRY_ENABLE_NET
    InitHttpFetcher();
#endif

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
      printf("Error: %s\n", SDL_GetError());
      return;
    }

#ifdef CHERRY_ENABLE_LINUXDRM
    if (SDL_getenv("SDL_VIDEODRIVER") == nullptr) {
      SDL_setenv("SDL_VIDEODRIVER", "KMSDRM", 1);
    }
#endif

    ExecuteProcessCallbacks(ProcessCallback::ON_INITIALIZATION_FINISHED);
  }

  std::string Application::GetHttpCacheFolderName() {
    return m_HttpCacheFolderName;
  }

  void Application::SetHttpCacheFolderName(const std::string &name) {
    m_HttpCacheFolderName = name;
  }

  void Application::BoostrappWindow() {
    std::string name;

    if (s_Instance->m_DefaultSpecification.DefaultWindowName.empty()) {
      name = CertifyWindowName(s_Instance->m_DefaultSpecification.Name);
    } else {
      name = CertifyWindowName(s_Instance->m_DefaultSpecification.DefaultWindowName);
    }

    this->m_Windows.push_back(
        std::make_shared<Window>(
            name, app->m_DefaultSpecification.Width, app->m_DefaultSpecification.Height, app->m_DefaultSpecification));
  }

  std::atomic<bool> &Application::RunningState() {
    static std::atomic<bool> running{ true };
    return running;
  }

  void Application::RequestShutdown() {
    RunningState().store(false, std::memory_order_release);
  }

#ifdef CHERRY_DEBUG
  std::atomic<bool> &Application::DebugToolState() {
    static std::atomic<bool> devtools{ true };
    return devtools;
  }
  void Application::EnableDebugTools() {
    DebugToolState().store(true);
    Debugger::StartDevTools();
  }

  void Application::DisableDebugTools() {
    DebugToolState().store(false);
    Debugger::StopDevTools();
  }

  void Application::ToggleDebugTools() {
    bool current = DebugToolState().load();
    bool nextState = !current;

    DebugToolState().store(nextState);

    if (nextState) {
      Debugger::StartDevTools();
    } else {
      Debugger::StopDevTools();
    }
  }
#endif  // CHERRY_DEBUG

  void Application::Shutdown() {
    RequestShutdown();

#ifdef CHERRY_ENABLE_AUDIO
    if (app->m_DefaultSpecification.UseAudioService) {
      StopAudioService();
    }
#endif

#ifdef CHERRY_ENABLE_NET
    ShutdownHttpFetcher();
#endif

    if (g_Device != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(g_Device);
    }

    for (size_t i = 0; i < m_Windows.size(); ++i) {
      auto &window = m_Windows[i];
      if (!window)
        continue;

      window->SetIsClosing(true);
      if (window->GetImGuiContext()) {
        ImGui::SetCurrentContext(window->GetImGuiContext());
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(window->GetImGuiContext());
        window->SetImGuiContext(nullptr);
      }

      CleanupVulkanWindow(window.get());

      if (window->GetWindowHandle()) {
        SDL_DestroyWindow(window->GetWindowHandle());
      }
    }

    if (!m_Windows.empty()) {
      CleanupVulkan(nullptr);
    }

    for (auto &window : m_Windows) {
      if (window) {
        window->GetImageMap().clear();
        window->GetHexImageMap().clear();
      }
    }

    m_Windows.clear();

    SDL_Quit();
    s_Instance = nullptr;
  }

  void Application::FrameRender(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win, ImDrawData *draw_data) {
    VkResult err;
    VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;

    err = vkAcquireNextImageKHR(
        g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);

    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
      win->GetSwapChainRebuild() = true;

      return;
    }

    check_vk_result(err);

    ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];

    win->GetCurrentFrameIndex() = (win->GetCurrentFrameIndex() + 1) % win->GetWinData()->ImageCount;

    {
      err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);
      check_vk_result(err);

      err = vkResetFences(g_Device, 1, &fd->Fence);
      check_vk_result(err);
    }
    {
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

  void Application::FramePresent(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win) {
    if (win->GetSwapChainRebuild())
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
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
      win->GetSwapChainRebuild() = true;
      return;
    }
    check_vk_result(err);
    wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount;  // Now we can use the next set of semaphores
  }

  std::string Application::SpawnWindow(ApplicationSpecification spec) {
    std::string name;

    if (spec.DefaultWindowName.empty()) {
      name = CertifyWindowName(spec.Name);
    } else {
      name = CertifyWindowName(spec.DefaultWindowName);
    }

    ImGuiContext *res_ctx = ImGui::GetCurrentContext();

    std::shared_ptr<Window> new_win = std::make_shared<Window>(name, spec.Width, spec.Height, spec);

    this->m_Windows.push_back(new_win);

    ImGui::SetCurrentContext(res_ctx);

    return name;
  }

  void Application::SpawnWindow(const std::string &winname, ApplicationSpecification spec) {
    std::string name = CertifyWindowName(winname);

    ImGuiContext *res_ctx = ImGui::GetCurrentContext();

    std::shared_ptr<Window> new_win = std::make_shared<Window>(name, spec.Width, spec.Height, spec);

    this->m_Windows.push_back(new_win);

    ImGui::SetCurrentContext(res_ctx);
  }

  std::string Application::SpawnWindow() {
    std::string name;

    if (app->m_DefaultSpecification.DefaultWindowName.empty()) {
      name = CertifyWindowName(app->m_DefaultSpecification.Name);
    } else {
      name = CertifyWindowName(app->m_DefaultSpecification.DefaultWindowName);
    }

    ImGuiContext *res_ctx = ImGui::GetCurrentContext();

    std::shared_ptr<Window> new_win = std::make_shared<Window>(
        name, app->m_DefaultSpecification.Width, app->m_DefaultSpecification.Height, app->m_DefaultSpecification);

    this->m_Windows.push_back(new_win);

    ImGui::SetCurrentContext(res_ctx);
    return name;
  }

  void Application::SpawnWindow(const std::string &winname) {
    std::string name = CertifyWindowName(winname);

    ImGuiContext *res_ctx = ImGui::GetCurrentContext();

    std::shared_ptr<Window> new_win = std::make_shared<Window>(
        name, app->m_DefaultSpecification.Width, app->m_DefaultSpecification.Height, app->m_DefaultSpecification);

    this->m_Windows.push_back(new_win);

    ImGui::SetCurrentContext(res_ctx);
  }

  void Application::UnspawnWindow(const std::string &name) {
    for (auto window : m_Windows) {
      if (name == window->GetName()) {
        // TODO Unspawn window
      }
    }
  }

  void Application::InitializeWindowStates() {
    if (!m_IsDataInitialized) {
      if (s_Instance->m_PreviousSaveData.find("data") != s_Instance->m_PreviousSaveData.end()) {
        auto windowsJson = s_Instance->m_PreviousSaveData["data"].value("windows", nlohmann::json::array());

        for (const auto &appWindowJson : windowsJson) {
          std::string appWindowName = appWindowJson.value("name", "");
          std::string dockPlace = appWindowJson.value("dockplace", "");
          std::string type = appWindowJson.value("type", "");
          std::string path = appWindowJson.value("path", "");
          std::string win = appWindowJson.value("win", "");
          std::string id = appWindowJson.value("id", "");
          std::string parent = appWindowJson.value("dockparent", "");
          std::string docknodeparent = appWindowJson.value("docknodeparent", "");

          std::shared_ptr<AppWindow> appWindow = std::make_shared<AppWindow>();

          appWindow->m_IdName = appWindowName;
          appWindow->SetFetchedSaveData("dockplace", dockPlace);
          appWindow->SetFetchedSaveData("type", type);
          appWindow->SetFetchedSaveData("path", path);
          appWindow->SetFetchedSaveData("win", win);
          appWindow->SetFetchedSaveData("id", id);
          appWindow->SetFetchedSaveData("dockparent", parent);
          appWindow->SetFetchedSaveData("docknodeparent", docknodeparent);
          m_SavedAppWindows.push_back(appWindow);
        }
      }

      m_IsDataInitialized = true;
    } else {
      // throw std::time_error("No valid data found in m_PreviousSaveData.");
    }
  }

  void Application::SaveData() {
    s_Instance->m_IsDataSaved = true;
    nlohmann::json jsonData;

    std::ifstream inputFile(this->m_WindowSaveDataPath);
    if (inputFile) {
      inputFile >> jsonData;
      inputFile.close();
    }

    if (jsonData.find("data") == jsonData.end()) {
      jsonData["data"] = nlohmann::json::object();
    }

    nlohmann::json windowsJson = nlohmann::json::array();

    nlohmann::json appWindowsJson = nlohmann::json::array();

    for (auto &app_window : s_Instance->m_AppWindows) {
      std::string dockspace_state = "default";
      std::string docknodeparent = "default";

      if (app_window->GetSimpleStorage("dockplace")) {
        dockspace_state = app_window->GetSimpleStorage("dockplace")->m_Data;
      }

      if (app_window->GetSimpleStorage("docknodeparent")) {
        docknodeparent = app_window->GetSimpleStorage("docknodeparent")->m_Data;
      }

      nlohmann::json appWindowJson;
      appWindowJson["name"] = app_window->m_IdName;
      appWindowJson["dockplace"] = dockspace_state;
      appWindowJson["docknodeparent"] = docknodeparent;
      appWindowJson["win"] = app_window->m_WinParent;
      if (app_window->m_HaveParentAppWindow)
        appWindowJson["dockparent"] = app_window->m_ParentAppWindow->m_IdName;
      appWindowJson["type"] = "instanciable,static";
      appWindowJson["id"] = "test_window";

      nlohmann::json appWindowStorageJson;

      nlohmann::json simpleStorage = nlohmann::json::array();
      nlohmann::json windowStorage = nlohmann::json::array();

      for (auto &window_storage_item : app_window->DumpWindowStorage()) {
        if (window_storage_item.second->m_Persistant) {
          nlohmann::json item;
          item["key"] = window_storage_item.first;
          item["value"] = window_storage_item.second->m_JsonData;
          windowStorage.push_back(item);
        }
      }

      for (auto &simple_storage_item : app_window->DumpSimpleStorage()) {
        if (simple_storage_item.second->m_Persistant) {
          nlohmann::json item;
          item["key"] = simple_storage_item.first;
          item["value"] = simple_storage_item.second->m_Data;
          simpleStorage.push_back(item);
        }
      }
      appWindowStorageJson["simple_storage"] = simpleStorage;
      appWindowStorageJson["window_storage"] = windowStorage;

      appWindowJson["data"] = appWindowStorageJson;

      appWindowsJson.push_back(appWindowJson);
    }

    jsonData["data"]["windows"] = appWindowsJson;

    std::ofstream outputFile(this->m_WindowSaveDataPath);
    if (!outputFile) {
      // throw std::time_error("Unable to open the file for writing: " +
      // this->m_WindowSaveDataPath);
    }

    outputFile << jsonData.dump(4);
    outputFile.close();
  }

  std::vector<std::shared_ptr<AppWindow>> Application::GetLastSaveInstanciableAppWindows() {
    std::vector<std::shared_ptr<AppWindow>> results;
    for (auto &savedappwins : Application::Get().m_SavedAppWindows) {
      if (savedappwins->m_AppWindowType == AppWindowTypes::InstanciableWindow) {
        results.push_back(savedappwins);
      }
    }
    return results;
  }

  void Application::ApplyDockingFromSave() {
    if (s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::DockingWindows ||
        s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::TabWidows ||
        s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::SimpleWindow) {
      for (auto &appwin : s_Instance->m_AppWindows) {
        bool dockplace_initialized = false;
        bool parent_initialized = false;
        bool win_initialized = false;
        bool sizex_initialized = false;

        if (!appwin->m_WindowRebuilded) {
          std::shared_ptr<WindowDragDropState> dragdropstate = std::make_shared<WindowDragDropState>();
          dragdropstate->LastDraggingAppWindowHost = appwin->m_IdName;
          dragdropstate->FromSave = true;
          LastWindowPressed = dragdropstate->LastDraggingAppWindowHost;
          for (auto &savedappwin : s_Instance->m_SavedAppWindows) {
            if (appwin->m_IdName == savedappwin->m_IdName) {
              if (savedappwin->GetFetchedSaveData("docknodeparent") != "undefined") {
                dragdropstate->LastDraggingAppWindow = savedappwin->GetFetchedSaveData("docknodeparent");
              }

              if (savedappwin->GetFetchedSaveData("dockplace") != "undefined") {
                dragdropstate->DragOwner = savedappwin->m_IdName;

                if (savedappwin->GetFetchedSaveData("dockplace") == "right") {
                  dragdropstate->LastDraggingPlace = DockEmplacement::DockRight;
                } else if (savedappwin->GetFetchedSaveData("dockplace") == "left") {
                  dragdropstate->LastDraggingPlace = DockEmplacement::DockLeft;
                }

                else if (savedappwin->GetFetchedSaveData("dockplace") == "up") {
                  dragdropstate->LastDraggingPlace = DockEmplacement::DockUp;
                } else if (savedappwin->GetFetchedSaveData("dockplace") == "down") {
                  dragdropstate->LastDraggingPlace = DockEmplacement::DockDown;
                }

                else if (savedappwin->GetFetchedSaveData("dockplace") == "full") {
                  dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
                } else {
                  dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
                }
                dockplace_initialized = true;
              }

              if (savedappwin->GetFetchedSaveData("sizex") != "undefined") {
                bool sizex_initialized = true;
              }

              if (savedappwin->GetFetchedSaveData("parent") != "undefined") {
                dragdropstate->LastDraggingAppWindow = savedappwin->GetFetchedSaveData("dockparent");
                dragdropstate->DragOwner = savedappwin->GetFetchedSaveData("dockparent");
                bool parent_initialized = true;
              }

              if (savedappwin->GetFetchedSaveData("win") != "undefined") {
                dragdropstate->LastDraggingWindow = savedappwin->GetFetchedSaveData("win");
                appwin->m_WinParent = savedappwin->GetFetchedSaveData("win");

                bool is_win_existing = false;
                for (auto &win : s_Instance->m_Windows) {
                  if (win->GetName() == appwin->GetFetchedSaveData("win")) {
                    is_win_existing = true;
                  }
                }

                if (!is_win_existing) {
                  s_Instance->SpawnWindow(appwin->GetFetchedSaveData("win"));
                }

                bool win_initialized = true;
              }
            }
          }

          if (!dockplace_initialized) {
            dragdropstate->DragOwner = appwin->m_IdName;

            if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "right") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockRight;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "left") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockLeft;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "up") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockUp;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "down") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockDown;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "full") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
            } else {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
            }
          }

          if (!win_initialized && s_Instance->m_Windows[0]) {
            dragdropstate->LastDraggingWindow = s_Instance->m_Windows[0]->GetName();
            dragdropstate->DragOwner = s_Instance->m_Windows[0]->GetName();
          }

          bool win_finded = false;
          for (auto &win : s_Instance->m_Windows) {
            if (dragdropstate->LastDraggingWindow == win->GetName()) {
              win_finded = true;
            }
          }
          if (!win_finded) {
            appwin->AttachOnNewWindow(s_Instance->m_DefaultSpecification);
          }

          c_CurrentDragDropState = dragdropstate;
          Application::PushRedockEvent(c_CurrentDragDropState);
          dragdropstate->DragOwner = "none";
          c_CurrentDragDropState = nullptr;

          appwin->m_WindowRebuilded = true;
          appwin->m_WindowJustRebuilded = true;
        }
      }
    }
  }

  void Application::ApplyDockingFromDefault() {
    if (s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::DockingWindows ||
        s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::TabWidows ||
        s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::SimpleWindow) {
      for (auto &appwin : s_Instance->m_AppWindows) {
        if (!appwin->m_WindowRebuilded) {
          bool dockplace_initialized = false;
          bool parent_initialized = false;
          bool win_initialized = false;
          bool sizex_initialized = false;

          std::shared_ptr<WindowDragDropState> dragdropstate = std::make_shared<WindowDragDropState>();
          dragdropstate->LastDraggingAppWindowHost = appwin->m_IdName;
          LastWindowPressed = dragdropstate->LastDraggingAppWindowHost;
          dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;

          if (!dockplace_initialized) {
            dragdropstate->DragOwner = appwin->m_IdName;

            if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "right") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockRight;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "left") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockLeft;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "up") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockUp;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "down") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockDown;
            } else if (appwin->GetDefaultBehavior(DefaultAppWindowBehaviors::DefaultDocking) == "full") {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
            } else {
              dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
            }
          }

          if (!win_initialized && s_Instance->m_Windows[0]) {
            dragdropstate->LastDraggingWindow = s_Instance->m_Windows[0]->GetName();
            dragdropstate->DragOwner = s_Instance->m_Windows[0]->GetName();
          }

          c_CurrentDragDropState = dragdropstate;
          Application::PushRedockEvent(c_CurrentDragDropState);
          dragdropstate->DragOwner = "none";
          c_CurrentDragDropState = nullptr;

          appwin->m_WindowRebuilded = true;
          appwin->m_WindowJustRebuilded = true;
        }
      }
    }
  }

  void Application::CurrentDockRequestOnNewWindow() {
    std::string new_win_title = s_Instance->SpawnWindow();

    c_CurrentDragDropState->LastDraggingPlace = DockEmplacement::DockFull;
    c_CurrentDragDropState->LastDraggingAppWindow = "none";
    c_CurrentDragDropState->LastDraggingWindow = new_win_title;

    Application::PushRedockEvent(c_CurrentDragDropState);

    c_CurrentDragDropState->CreateNewWindow = false;
  }

  void Application::SetCurrentRedockEvent(const std::shared_ptr<Cherry::WindowDragDropState> &state) {
    c_CurrentDragDropState = state;
  }

  void Application::PresentAllWindows() {
    for (auto &window : m_Windows) {
      ImGui::SetCurrentContext(window->GetImGuiContext());

      bool isMouseOver = (m_MouseHoveredWindow == window->GetWindowHandle());
      if (!isMouseOver && m_MouseHoveredWindow != nullptr) {
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
      }

      // Push window selected theme if defined
      if (window->GetSelectedTheme() != "undefined") {
        PushTheme(window->GetSelectedTheme());
      }

      c_CurrentRenderedWindow = window;

      if (c_MasterSwapChainRebuild) {
        window->GetSwapChainRebuild() = true;
      }

      if (window->GetSwapChainRebuild()) {
        int width, height;
        SDL_GetWindowSize(window->GetWindowHandle(), &width, &height);
        if (width > 0 && height > 0) {
          ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
          ImGui_ImplVulkanH_CreateOrResizeWindow(
              g_Instance,
              g_PhysicalDevice,
              g_Device,
              window->GetWinData(),
              g_QueueFamily,
              g_Allocator,
              width,
              height,
              g_MinImageCount);
          window->GetWinData()->FrameIndex = 0;
          window->GetSwapChainRebuild() = false;
        }
      }

      ImGui_ImplVulkan_NewFrame();
      ImGui_ImplSDL2_NewFrame();

      if (!isMouseOver && m_MouseHoveredWindow != nullptr) {
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
      }

      ImGui::SetCurrentContext(window->GetImGuiContext());

      ImGuiIO &inputIO = ImGui::GetIO();
      const bool prevTrickling = inputIO.ConfigInputTrickleEventQueue;
      if (prevTrickling && inputIO.WantTextInput) {
        inputIO.ConfigInputTrickleEventQueue = false;
      }

      ImGui::NewFrame();

      inputIO.ConfigInputTrickleEventQueue = prevTrickling;

      window->LoadTheme();

      app->RenderWindow(window.get());

      if (c_DockIsDragging && c_CurrentDragDropState) {
        SDL_GetGlobalMouseState(&c_CurrentDragDropState->mouseX, &c_CurrentDragDropState->mouseY);

        float oldScale = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= window->GetSpecifications().FontGlobalScale;
        ImGui::PushFont(ImGui::GetFont());

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.12f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.30f, 0.30f, 0.30f, 1.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

        ImGui::SetNextWindowPos(
            ImVec2((float)c_CurrentDragDropState->mouseX + 15, (float)c_CurrentDragDropState->mouseY + 15));

        ImGuiWindowFlags previewFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                        ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoSavedSettings;

        if (ImGui::Begin("##DragPreviewOverlay", NULL, previewFlags)) {
          float iconSize = 30.0f;
          ImVec2 p = ImGui::GetCursorScreenPos();

          const std::string &windowId = c_CurrentDragDropState->LastDraggingAppWindowHost;
          const std::string &logoPath = GetLogoPathForAppWindow(windowId);
          const std::string &descStr = GetDescriptionForAppWindow(windowId);

          const char *titleText = windowId.c_str();
          const char *descText = descStr.c_str();
          bool hasDesc = (descText && descText[0] != '\0');

          if (!logoPath.empty()) {
            ImGui::SetCursorScreenPos(p);
            CherryGUI::Image(Cherry::GetTexture(logoPath), ImVec2(iconSize, iconSize));
          } else {
            ImDrawList *drawList = ImGui::GetWindowDrawList();
            ImU32 colGray = IM_COL32(200, 200, 200, 255);
            ImU32 colAccent = IM_COL32(230, 40, 40, 255);

            drawList->AddRectFilled(p, ImVec2(p.x + iconSize, p.y + iconSize), IM_COL32(50, 50, 50, 255), 2.0f);

            float winX = p.x + 5.0f;
            float winY = p.y + 7.0f;
            float winW = iconSize - 10.0f;
            float winH = iconSize - 14.0f;

            drawList->AddRectFilled(ImVec2(winX, winY), ImVec2(winX + winW, winY + 4.0f), colAccent, 1.0f);
            drawList->AddRect(ImVec2(winX, winY), ImVec2(winX + winW, winY + winH), colGray, 1.0f);
          }

          float textStartX = p.x + iconSize + 10.0f;
          ImVec2 titleSize = ImGui::CalcTextSize(titleText);

          if (hasDesc) {
            ImGui::SetCursorScreenPos(ImVec2(textStartX, p.y - 1.0f));
            ImGui::TextUnformatted(titleText);

            ImGui::SetCursorScreenPos(ImVec2(textStartX, p.y + titleSize.y + 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::TextUnformatted(descText);
            ImGui::PopStyleColor();
          } else {
            float centeredY = p.y + (iconSize - titleSize.y) * 0.5f;
            ImGui::SetCursorScreenPos(ImVec2(textStartX, centeredY));
            ImGui::TextUnformatted(titleText);
          }

          ImGui::SetCursorScreenPos(p);
          ImGui::Dummy(ImVec2(iconSize + 10.0f + titleSize.x, iconSize));
        }

        ImGui::End();

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);
        ImGui::GetFont()->Scale = oldScale;
        ImGui::PopFont();
      }

      if (window->GetIsMoving() && window->GetSpecifications().CustomTitlebar &&
          window->GetSpecifications().DisableWindowManagerTitleBar) {
        SnapState &snap = window->GetSnapState();
        HandleSnapToEdge(window.get());

        if (snap.snapTarget.w > 0) {
          int winX, winY, winW, winH;
          SDL_GetWindowPosition(window->GetWindowHandle(), &winX, &winY);
          SDL_GetWindowSize(window->GetWindowHandle(), &winW, &winH);
          ImGuiViewport *vp = ImGui::GetMainViewport();
          float scaleX = vp->Size.x / (float)winW;
          float scaleY = vp->Size.y / (float)winH;

          float localX = (float)(snap.snapTarget.x - winX) * scaleX;
          float localY = (float)(snap.snapTarget.y - winY) * scaleY;
          float localW = (float)snap.snapTarget.w * scaleX;
          float localH = (float)snap.snapTarget.h * scaleY;

          ImGuiWindowFlags snapFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav |
                                       ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;

          ImGui::SetNextWindowPos(ImVec2(localX, localY));
          ImGui::SetNextWindowSize(ImVec2(localW, localH));
          ImGui::SetNextWindowBgAlpha(0.20f);

          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.4f, 0.7f, 1.0f, 1.0f));
          ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5f, 0.8f, 1.0f, 1.0f));
          ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
          ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);

          ImGui::Begin("##SnapPreview", nullptr, snapFlags);
          ImGui::End();

          ImGui::PopStyleVar(2);
          ImGui::PopStyleColor(2);
        }
      } else if (
          !window->GetIsMoving() && window->GetSpecifications().CustomTitlebar &&
          window->GetSpecifications().DisableWindowManagerTitleBar) {
        ApplySnapOnRelease(window.get());
      }

      window->UnloadTheme();

      ImGui_ImplVulkanH_Window *wd = window->GetWinData();
      ImGuiIO &io = ImGui::GetIO();
      ImGui::Render();
      ImDrawData *main_draw_data = ImGui::GetDrawData();
      const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);

      if (!main_is_minimized) {
        FrameRender(wd, window.get(), main_draw_data);
      }

      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
      }

      if (!main_is_minimized) {
        FramePresent(wd, window.get());
      }

      // Push window selected theme if defined
      if (window->GetSelectedTheme() != "undefined") {
        PopTheme();
      }
    }
  }

#ifndef _WIN32
  std::string Application::CertifyWindowName(const std::string &name) {
    int max_suffix = 0;
    bool name_exists = false;

    std::regex suffix_regex(R"(^(.*?)(?: <(\d+)>)?$)");
    std::smatch match;

    for (const auto &win : s_Instance->m_Windows) {
      std::string window_name = win->GetName();

      if (std::regex_match(window_name, match, suffix_regex)) {
        std::string base_name = match[1].str();
        std::string suffix_str = match[2].str();

        if (base_name == name) {
          name_exists = true;

          if (!suffix_str.empty()) {
            int suffix_value = std::stoi(suffix_str);
            max_suffix = std::max(max_suffix, suffix_value);
          }
        }
      }
    }

    if (name_exists) {
      std::stringstream new_name;
      new_name << name << " <" << (max_suffix + 1) << ">";
      return new_name.str();
    }

    return name;
  }
#endif

#ifdef _WIN32
  std::string Application::CertifyWindowName(const std::string &name) {
    int max_suffix = 0;
    bool name_exists = false;

    std::regex suffix_regex(R"(^(.*?)(?: <(\d+)>)?$)");
    std::smatch match;

    for (const auto &win : s_Instance->m_Windows) {
      std::string window_name = win->GetName();

      if (std::regex_match(window_name, match, suffix_regex)) {
        std::string base_name = match[1].str();
        std::string suffix_str = match[2].str();

        if (base_name == name) {
          name_exists = true;

          if (!suffix_str.empty()) {
            int suffix_value = std::stoi(suffix_str);
            max_suffix = (std::max)(max_suffix, suffix_value);
          }
        }
      }
    }

    if (name_exists) {
      std::stringstream new_name;
      new_name << name << " <" << (max_suffix + 1) << ">";
      return new_name.str();
    }

    return name;
  }
#endif

  void Application::CleanupWindowIfEmpty(const std::shared_ptr<Window> &win) {
    if (!win || m_Windows.size() <= 1) {
      return;
    }

    if (win->GetSpecifications().RenderMode == WindowRenderingMethod::DockingWindows ||
        win->GetSpecifications().RenderMode == WindowRenderingMethod::TabWidows ||
        win->GetSpecifications().RenderMode == WindowRenderingMethod::SimpleWindow) {
      int app_wins_inside = 0;

      for (const auto &app_win : m_AppWindows) {
        if (app_win->CheckWinParent(win->GetName())) {
          app_wins_inside++;
        }
      }

      if (app_wins_inside == 0) {
        if (g_Device != VK_NULL_HANDLE) {
          vkDeviceWaitIdle(g_Device);
        }

        win->SetIsClosing(true);

        CleanupVulkanWindow(win.get());

        if (win->GetWindowHandle()) {
          SDL_DestroyWindow(win->GetWindowHandle());
        }

        m_Windows.erase(std::remove(m_Windows.begin(), m_Windows.end(), win), m_Windows.end());

        if (!m_Windows.empty()) {
          c_CurrentRenderedWindow = m_Windows.front();
          if (c_CurrentRenderedWindow->GetImGuiContext()) {
            ImGui::SetCurrentContext(c_CurrentRenderedWindow->GetImGuiContext());
          }
        }
      }
    }
  }

  void Application::RebuildDockingLinks() {
    //
  }

  void Application::MultiThreadWindowRuntime() {
    const std::chrono::milliseconds frameDuration(1000 / s_Instance->m_DefaultSpecification.MaxFps);

    int frameCount = 0;
    auto startTime = std::chrono::steady_clock::now();
    auto frameStart = std::chrono::steady_clock::now();

    m_Running = true;
    this->BoostrappWindow();
    while (m_Running) {
      frameStart = std::chrono::steady_clock::now();
      frameCount++;

      auto now = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
      if (elapsed >= 1) {
        frameCount = 0;
        startTime = std::chrono::steady_clock::now();
      }

      auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - frameStart);
      if (frameTime < frameDuration) {
        std::this_thread::sleep_for(frameDuration - frameTime);
      }

      Identifier::ResetUniqueIndex();  // Reset anonymous components indexes
      c_ValidDropZoneFounded = false;

      if (m_NoAppWindowSafetyEnabled) {
        if (m_AppWindows.size() > 1) {
          DisableNoAppWindowSafety();
        }
      }

      if (s_Instance->m_DefaultSpecification.WindowSaves) {
        if (!m_IsDataInitialized) {
          s_Instance->InitializeWindowStates();
        }

        if (!s_Instance->m_IsDataSaved) {
          s_Instance->SaveData();
        }

        s_Instance->ApplyDockingFromSave();
      } else {
        s_Instance->ApplyDockingFromDefault();
      }

      if (c_CurrentDragDropState) {
        if (c_CurrentDragDropState->CreateNewWindow) {
          s_Instance->CurrentDockRequestOnNewWindow();
        }
      }

      for (auto &window : m_Windows) {
        if (window->GetSpecifications().FavIconPath != window->GetSpecifications().LastFavIconPath) {
          window->SetFavIcon(window->GetSpecifications().FavIconPath);
          window->GetSpecifications().LastFavIconPath = window->GetSpecifications().FavIconPath;
        }

        c_CurrentRenderedWindow = window;
        if (window->GetDragDropState()->DockIsDragging) {
          c_DockIsDragging = true;
        }
      }

#ifdef CHERRY_DEBUG
      m_TriggerDevtools = false;
#endif  // CHERRY_DEBUG

      SDL_Event event;

      while (SDL_PollEvent(&event)) {
        SDL_Window *focusedWindow = SDL_GetMouseFocus();
        Uint32 focusedWindowID = focusedWindow ? SDL_GetWindowID(focusedWindow) : 0;

        bool eventHandled = false;

        for (auto &window : m_Windows) {
          c_CurrentRenderedWindow = window;
          Uint32 windowID = SDL_GetWindowID(window->GetWindowHandle());

          if (focusedWindowID != 0 && windowID != focusedWindowID) {
            continue;
          }

          ImGui::SetCurrentContext(window->GetImGuiContext());
          ImGui_ImplSDL2_ProcessEvent(&event);

          if (event.type == SDL_QUIT) {
            m_Running = false;
            eventHandled = true;
            break;
          }

          if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
              event.window.windowID == windowID) {
            if (window->GetSpecifications().UsingCloseCallback) {
              if (window->GetSpecifications().CloseCallback) {
                window->GetSpecifications().CloseCallback();
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

        if (eventHandled) {
          break;
        }
      }

      bool AppWindowRedocked = false;
      if (s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::DockingWindows ||
          Application::GetCurrentRenderedWindow()->GetSpecifications().RenderMode == WindowRenderingMethod::TabWidows ||
          Application::GetCurrentRenderedWindow()->GetSpecifications().RenderMode == WindowRenderingMethod::SimpleWindow) {
        for (auto &req : m_RedockRequests) {
          if (req->m_IsObsolete) {
            continue;
          }

          for (auto &app_win : m_AppWindows) {
            if (req->m_ParentAppWindowHost == app_win->m_IdName) {
              bool parentFound = false;
              for (auto &win : m_Windows) {
                if (win->GetName() == req->m_ParentWindow) {
                  app_win->SetParentWindow(win->GetName());

                  parentFound = true;
                }
              }

              if (!parentFound) {
                app_win->SetParentWindow("unknow");
              }

              AppWindowRedocked = true;
            }
          }
        }
      }

      if (s_Instance->m_DefaultSpecification.RenderMode == WindowRenderingMethod::DockingWindows ||
          Application::GetCurrentRenderedWindow()->GetSpecifications().RenderMode == WindowRenderingMethod::TabWidows ||
          Application::GetCurrentRenderedWindow()->GetSpecifications().RenderMode == WindowRenderingMethod::SimpleWindow) {
        for (auto &appwin : s_Instance->m_AppWindows) {
          if (!AppWindowRedocked) {
            if (!appwin->m_AttachRequest.m_IsFinished) {
              std::string win = Application::Get().SpawnWindow(appwin->m_AttachRequest.m_Specification);
              appwin->SetParentWindow(win);
              appwin->m_AttachRequest.m_IsFinished = true;
            }
          }
        }
      }

      DragRendered = false;

      RenderHooks();
      PresentAllWindows();

      float time = GetTime();
      m_FrameTime = time - m_LastFrameTime;
      m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
      m_LastFrameTime = time;

      c_MasterSwapChainRebuild = false;
      c_DockIsDragging = false;

      // Erase empty main windows
      // CleanupEmptyWindows();

      for (auto &appwin : s_Instance->m_AppWindows) {
        appwin->m_WindowJustRebuilded = false;
      }

      AppWindowRedocked = false;
    }
  }

  void Application::SingleThreadRuntime() {
    const std::chrono::milliseconds frameDuration(1000 / s_Instance->m_DefaultSpecification.MaxFps);

    int frameCount = 0;
    auto startTime = std::chrono::steady_clock::now();
    auto frameStart = std::chrono::steady_clock::now();

    m_Running = true;
    this->BoostrappWindow();
    while (m_Running) {
      frameStart = std::chrono::steady_clock::now();
      frameCount++;

      auto now = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
      if (elapsed >= 1) {
        frameCount = 0;
        startTime = std::chrono::steady_clock::now();
      }

      SDL_Event event;

      while (SDL_PollEvent(&event)) {
        bool eventHandled = false;

        SDL_Window *inputTargetSDLWindow = nullptr;

        switch (event.type) {
          case SDL_MOUSEBUTTONDOWN:
          case SDL_MOUSEBUTTONUP: inputTargetSDLWindow = SDL_GetWindowFromID(event.button.windowID); break;
          case SDL_MOUSEMOTION: inputTargetSDLWindow = SDL_GetWindowFromID(event.motion.windowID); break;
          case SDL_MOUSEWHEEL: inputTargetSDLWindow = SDL_GetWindowFromID(event.wheel.windowID); break;
          case SDL_KEYDOWN:
          case SDL_KEYUP: inputTargetSDLWindow = SDL_GetWindowFromID(event.key.windowID); break;
          case SDL_TEXTINPUT: inputTargetSDLWindow = SDL_GetWindowFromID(event.text.windowID); break;
          case SDL_TEXTEDITING: inputTargetSDLWindow = SDL_GetWindowFromID(event.edit.windowID); break;
          default: inputTargetSDLWindow = nullptr; break;
        }

        ImGuiContext *inputTargetContext = nullptr;

        if (inputTargetSDLWindow) {
          for (auto &window : m_Windows) {
            if (window->GetWindowHandle() == inputTargetSDLWindow) {
              inputTargetContext = window->GetImGuiContext();
              break;
            }
          }

          if (!inputTargetContext) {
            for (auto &window : m_Windows) {
              ImGui::SetCurrentContext(window->GetImGuiContext());
              ImGuiPlatformIO &pio = ImGui::GetPlatformIO();
              for (ImGuiViewport *vp : pio.Viewports) {
                if (vp->PlatformHandle == (void *)inputTargetSDLWindow) {
                  inputTargetContext = window->GetImGuiContext();
                  break;
                }
              }
              if (inputTargetContext)
                break;
            }
          }
        }

        for (auto &window : m_Windows) {
          c_CurrentRenderedWindow = window;
          Uint32 windowID = SDL_GetWindowID(window->GetWindowHandle());

          ImGui::SetCurrentContext(window->GetImGuiContext());

          bool isOwner = (inputTargetContext == nullptr) || (inputTargetContext == window->GetImGuiContext());

          if (isOwner) {
            ImGui_ImplSDL2_ProcessEvent(&event);
          } else {
            if (event.type == SDL_WINDOWEVENT || event.type == SDL_QUIT) {
              ImGui_ImplSDL2_ProcessEvent(&event);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
              ImGui::ClearActiveID();
            }
          }

          if (event.type == SDL_QUIT) {
            m_Running = false;
            eventHandled = true;
            break;
          }

          if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_ENTER) {
              m_MouseHoveredWindow = SDL_GetWindowFromID(event.window.windowID);

            } else if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
              SDL_Window *leavingWindow = SDL_GetWindowFromID(event.window.windowID);
              if (m_MouseHoveredWindow == leavingWindow)
                m_MouseHoveredWindow = nullptr;
            }
          }

          if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
              event.window.windowID == windowID) {
            if (window->GetSpecifications().UsingCloseCallback) {
              if (window->GetSpecifications().CloseCallback)
                window->GetSpecifications().CloseCallback();
              m_ClosePending = false;
            } else {
              if (Application::Get().m_CloseCallback)
                Application::Get().m_CloseCallback();
              else
                Application::Get().Close();
            }
          }
        }

        if (eventHandled)
          break;
      }

      Identifier::ResetUniqueIndex();  // Reset anonymous components indexes

      // If persistant (in all app) static mode (and not immediate mode)
      // CherryApp.ResetCurrentComponent();
      // CherryApp.ResetNextComponent();
      // CherryApp.ResetLastComponent();
      CherryApp.PurgeNoRenderedComponents();
      CherryApp.RefreshComponentsRenderFlags();

      // Push Selected theme if defined
      if (m_SelectedTheme != "undefined") {
        PushTheme(m_SelectedTheme);
      }

      c_ValidDropZoneFounded = false;

      if (m_NoAppWindowSafetyEnabled) {
        if (m_AppWindows.size() > 1) {
          DisableNoAppWindowSafety();
        }
      }

      if (s_Instance->m_DefaultSpecification.WindowSaves) {
        if (!m_IsDataInitialized) {
          s_Instance->InitializeWindowStates();
        }

        if (!s_Instance->m_IsDataSaved) {
          s_Instance->SaveData();
        }

        s_Instance->ApplyDockingFromSave();
      } else {
        s_Instance->ApplyDockingFromDefault();
      }

      if (c_CurrentDragDropState) {
        if (c_CurrentDragDropState->CreateNewWindow) {
          s_Instance->CurrentDockRequestOnNewWindow();
        }
      }

      for (auto &window : m_Windows) {
        if (window->GetSpecifications().FavIconPath != window->GetSpecifications().LastFavIconPath) {
          window->SetFavIcon(window->GetSpecifications().FavIconPath);
          window->GetSpecifications().LastFavIconPath = window->GetSpecifications().FavIconPath;
        }

        c_CurrentRenderedWindow = window;
        if (window->GetDragDropState()->DockIsDragging) {
          c_DockIsDragging = true;
        }
      }

      bool AppWindowRedocked = false;
      for (auto &req : m_RedockRequests) {
        if (req->m_IsObsolete) {
          continue;
        }

        for (auto &app_win : m_AppWindows) {
          if (req->m_ParentAppWindowHost == app_win->m_IdName) {
            bool parentFound = false;
            for (auto &win : m_Windows) {
              if (win->GetName() == req->m_ParentWindow) {
                app_win->SetParentWindow(win->GetName());

                parentFound = true;
              }
            }

            if (!parentFound) {
              app_win->SetParentWindow("unknow");
            }

            // AppWindowRedocked = true;
          }
        }
      }

      for (auto &appwin : s_Instance->m_AppWindows) {
        if (!AppWindowRedocked) {
          if (!appwin->m_AttachRequest.m_IsFinished) {
            std::string win;
            if (appwin->m_AttachRequest.m_ExistingWindow) {
              win = appwin->m_AttachRequest.m_Specification.Name;
            } else {
              win = Application::Get().SpawnWindow(appwin->m_AttachRequest.m_Specification);
            }
            appwin->SetParentWindow(win);
            appwin->m_AttachRequest.m_IsFinished = true;
          }
        }
      }

      DragRendered = false;

      RenderHooks();
      PresentAllWindows();

      float time = GetTime();
      m_FrameTime = time - m_LastFrameTime;
      m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
      m_LastFrameTime = time;

      c_MasterSwapChainRebuild = false;
      c_DockIsDragging = false;

      // Erase empty main windows
      auto windowsSnapshot = m_Windows;
      for (auto win : windowsSnapshot) {
        CleanupWindowIfEmpty(win);
      }

      // Pop Selected theme if defined
      if (m_SelectedTheme != "undefined") {
        PopTheme();
      }

      for (auto &appwin : s_Instance->m_AppWindows) {
        appwin->m_WindowJustRebuilded = false;
      }

      AppWindowRedocked = false;

      auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - frameStart);
      if (frameTime < frameDuration) {
        std::this_thread::sleep_for(frameDuration - frameTime);
      }
    }
  }

  void Application::Close() {
    m_Running = false;
  }

  bool Application::IsMaximized(const std::shared_ptr<Window> &win) const {
    Uint32 flags = SDL_GetWindowFlags(win->GetWindowHandle());
    return (flags & SDL_WINDOW_MAXIMIZED) != 0;
  }

  float Application::GetTime() {
    return (float)SDL_GetTicks() / 1000.0f;
  }

  VkInstance Application::GetInstance() {
    return g_Instance;
  }

  VkPhysicalDevice Application::GetPhysicalDevice() {
    return g_PhysicalDevice;
  }

  VkDevice Application::GetDevice() {
    return g_Device;
  }

  std::vector<uint8_t> Application::LoadPngHexa(const std::string &path) {
    std::ifstream file(path, std::ios::binary);

    if (!file) {
      return {};
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> hexContent(fileSize);

    file.read(reinterpret_cast<char *>(hexContent.data()), fileSize);

    return hexContent;
  }

  VkCommandBuffer Application::GetCommandBuffer(bool begin, const std::shared_ptr<Window> &win) {
    ImGui_ImplVulkanH_Window *wd = win->GetWinData();

    VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

    VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
    cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufAllocateInfo.commandPool = command_pool;
    cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufAllocateInfo.commandBufferCount = 1;

    VkCommandBuffer &command_buffer = win->GetAllocatedCommandBuffers()[wd->FrameIndex].emplace_back();
    auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(command_buffer, &begin_info);
    check_vk_result(err);

    return command_buffer;
  }

  void Application::FlushCommandBuffer(VkCommandBuffer commandBuffer) {
    const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &commandBuffer;
    auto err = vkEndCommandBuffer(commandBuffer);
    check_vk_result(err);

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

  void Application::SubmitResourceFree(std::function<void()> &&func, const std::string &winname) {
    for (auto win : app->m_Windows) {
      if (win->GetName() == winname) {
        win->GetResourceFreeQueue()[win->GetCurrentFrameIndex()].emplace_back(func);
      }
    }
  }

  ImFont *Application::GetFont(const std::string &name) {
    auto it = s_Fonts.find(name);
    if (it != s_Fonts.end()) {
      return it->second;
    }

    return nullptr;
  }

  SDL_Window *Application::GetWindowHandle(const std::string &winname) {
    for (auto win : app->m_Windows) {
      if (winname == win->GetName()) {
        return win->GetWindowHandle();
      }
    }
    return nullptr;
  }

  void Application::HandleResizing(Window *window) {
  }

  /*std::string Application::GetThemeProperty(const std::string &key) {
    auto app_window_theme_prop = CherryAppWindow.GetThemeProperty(key);
    if (app_window_theme_prop == "undefined") {

      auto window_theme_prop = CherryWindow.GetThemeProperty(key);
      if (window_theme_prop == "undefined") {

        if (!GetSelectedTheme()) {
          return;
        } else {
          return GetSelectedTheme()->GetProperty(key);
        }
      }
    }
  }*/

  Window *Application::GetWindowByHandle(SDL_Window *window_handle) {
    for (auto win : this->m_Windows) {
      if (win->GetWindowHandle() == window_handle) {
        return win.get();
      }
    }
    return nullptr;
  }

  unsigned int SimpleHash(const std::string &str) {
    unsigned int hash = 0;
    for (char c : str) {
      hash = (hash * 31) + static_cast<unsigned int>(c);
    }
    return hash;
  }

  ImU32 HashToColor(unsigned int hash) {
    ImU8 r = (hash & 0xFF0000) >> 16;
    ImU8 g = (hash & 0x00FF00) >> 8;
    ImU8 b = (hash & 0x0000FF);
    ImU8 a = 255;

    return IM_COL32(r, g, b, a);
  }

  void Application::SetWindowSaveDataFile(const std::string &input_path, const bool &relative) {
    std::string path = input_path;
    if (relative) {
      path = std::filesystem::current_path().string() + "/" + path;
    }

    std::filesystem::path filePath(path);

    if (!std::filesystem::exists(filePath.parent_path())) {
      std::filesystem::create_directories(filePath.parent_path());
    }

    if (std::filesystem::exists(filePath)) {
      if (std::filesystem::is_regular_file(filePath)) {
        std::ifstream inputFile(path);
        if (!inputFile) {
          std::cout << "Unable to open file for reading: " << path << std::endl;
        }

        inputFile >> m_PreviousSaveData;
        inputFile.close();

        if (m_PreviousSaveData.find("data") == m_PreviousSaveData.end()) {
          m_PreviousSaveData["data"] = nlohmann::json::object();
        }
      } else {
        std::cout << "The specified path is not a valid file: " << path << std::endl;
      }
    }

    std::ofstream outputFile(path);
    if (!outputFile) {
      std::cout << "Unable to create or open file: " << path << std::endl;
    }

    nlohmann::json jsonData = { { "save", true } };

    if (!m_PreviousSaveData.empty()) {
      jsonData["data"] = m_PreviousSaveData["data"];
    }

    outputFile << jsonData.dump(4);
    outputFile.close();

    this->m_WindowSaveDataPath = path;
    this->m_SaveWindowData = true;
  }

  void Application::PushComponentPool(ComponentsPool *component_pool) {
    if (component_pool == nullptr) {
      return;
    }
    m_ComponentPoolStack.push_back(component_pool);
  }

  void Application::PopComponentPool() {
    if (!m_ComponentPoolStack.empty()) {
      m_ComponentPoolStack.pop_back();
    }
  }

  std::vector<ComponentsPool *> &Application::GetComponentPoolStack() {
    return m_ComponentPoolStack;
  }

  ComponentsPool *Application::GetComponentPool() {
    if (!m_ComponentPoolStack.empty()) {
      return m_ComponentPoolStack.back();
    } else {
      return &m_ApplicationComponentPool;
    }
  }

  void Application::ClearComponentPool() {
    m_ApplicationComponentPool.AnonymousComponents.clear();
    m_ApplicationComponentPool.IdentifiedComponents.clear();
  }

  void Application::ClearComponentPoolStack() {
    m_ComponentPoolStack.clear();
  }

  void Application::SetDataOnGroup(const std::string &group_name, const std::string &key, const std::string &value) {
    for (auto component : CherryApp.GetComponentPool()->IdentifiedComponents) {
      if (component->GetIdentifier().component_group() == group_name) {
        component->SetData(key, value);
      }
    }
  }
  void Application::SetPropertyOnGroup(const std::string &group_name, const std::string &key, const std::string &value) {
    for (auto component : CherryApp.GetComponentPool()->IdentifiedComponents) {
      if (component->GetIdentifier().component_group() == group_name) {
        component->SetProperty(key, value);
      }
    }
  }

  bool Application::IsKeyPressed(CherryKey key) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    const auto &keyMap = GetKeyMap();
    auto it = keyMap.find(key);
    return it != keyMap.end() && state[it->second];
  }

  bool Application::IsMouseClicked(int btn, bool repeat) {
    return CherryGUI::IsMouseClicked(btn, repeat);
  }

  bool Application::IsMousePressed(int btn) {
    return CherryGUI::IsMouseDown(btn);
  }

  bool Application::IsMouseDoubleClicked(int btn) {
    return CherryGUI::IsMouseDoubleClicked(btn);
  }

  void Application::PushCurrentComponent(const std::shared_ptr<Component> &component) {
    m_PushedCurrentComponent.push_back(component);
  }

  void Application::PopCurrentComponent(int pop_number) {
    if (m_PushedCurrentComponent.empty())
      return;

    if (pop_number <= 0 || pop_number >= static_cast<int>(m_PushedCurrentComponent.size())) {
      m_PushedCurrentComponent.clear();
    } else {
      m_PushedCurrentComponent.erase(m_PushedCurrentComponent.end() - pop_number, m_PushedCurrentComponent.end());
    }
  }

  std::shared_ptr<Component> Application::GetCurrentComponent() const {
    if (m_PushedCurrentComponent.empty()) {
      return nullptr;
    }
    return m_PushedCurrentComponent.back();
  }

  void Application::SetTheme(const std::string &theme_name) {
    m_SelectedTheme = theme_name;
  }

  void Application::RemoveTheme() {
    m_SelectedTheme = "undefined";
  }

  void Application::PushComponentGroup(const std::string &groupname) {
    m_PushedComponentGroups.push_back(groupname);
  }

  void Application::PopComponentGroup(int pop_number) {
    if (m_PushedComponentGroups.empty())
      return;

    if (pop_number <= 0 || pop_number >= static_cast<int>(m_PushedComponentGroups.size())) {
      m_PushedComponentGroups.clear();
    } else {
      m_PushedComponentGroups.erase(m_PushedComponentGroups.end() - pop_number, m_PushedComponentGroups.end());
    }
  }

  std::string Application::GetComponentGroup() const {
    if (m_PushedComponentGroups.empty()) {
      return "undefined";
    }
    return m_PushedComponentGroups.back();
  }

  void Application::AddTheme(const Theme &theme) {
    m_Themes[theme.GetName()] = theme;
  }

  void Application::RemoveTheme(const std::string &theme_name) {
    m_Themes.erase(theme_name);
  }

  void Application::ClearThemes() {
    m_Themes.clear();
  }

  void Application::PushTheme(const std::string &theme_name) {
    auto it = m_Themes.find(theme_name);
    if (it != m_Themes.end()) {
      m_ActiveThemes.push_back(it->second);
    }
  }

  void Application::PopTheme(int number) {
    while (number-- > 0 && m_ActiveThemes.size() > 1) {
      m_ActiveThemes.pop_back();
    }
  }

  void Application::PurgeNoRenderedComponents(ComponentsPool *pool) {
    if (!pool) {
      pool = &m_ApplicationComponentPool;
    }

    auto &components = pool->IdentifiedComponents;
    auto &anonymous_components = pool->AnonymousComponents;

    components.erase(
        std::remove_if(
            components.begin(),
            components.end(),
            [](const std::shared_ptr<Component> &component) { return !component->GetIsComponentRendered(); }),
        components.end());

    anonymous_components.erase(
        std::remove_if(
            anonymous_components.begin(),
            anonymous_components.end(),
            [](const std::shared_ptr<Component> &component) { return !component->GetIsComponentRendered(); }),
        anonymous_components.end());
  }

  void Application::RefreshComponentsRenderFlags(ComponentsPool *pool) {
    if (!pool) {
      pool = &m_ApplicationComponentPool;
    }

    for (auto &component : pool->IdentifiedComponents) {
      component->SetIsComponentRendered(false);
    }

    for (auto &component : pool->AnonymousComponents) {
      component->SetIsComponentRendered(false);
    }
  }

  void Application::RefreshComponent(const Identifier &id, ComponentsPool *pool) {
    if (!pool) {
      pool = &m_ApplicationComponentPool;
    }

    auto &components = pool->IdentifiedComponents;

    for (const auto &component : components) {
      if (component->GetIdentifier() == id) {
        component->Refresh();
      }
    }
  }

  void Application::DestroyComponent(const Identifier &id, ComponentsPool *pool) {
    if (!pool) {
      pool = &m_ApplicationComponentPool;
    }

    auto &components = pool->IdentifiedComponents;

    size_t beforeSize = components.size();

    auto it = std::remove_if(components.begin(), components.end(), [&id](const std::shared_ptr<Component> &component) {
      return component->GetIdentifier() == id;
    });

    if (it != components.end()) {
      components.erase(it, components.end());
    }

    size_t afterSize = components.size();

    if (CherryLastComponent.GetIdentifier() == id) {
      CherryApp.ResetLastComponent();
    }
    if (CherryNextComponent.GetIdentifier() == id) {
      CherryApp.ResetNextComponent();
    }
  }

  std::string Application::GetThemeProperty(const std::string &theme_name, const std::string &key) {
    auto it = m_Themes.find(theme_name);
    if (it != m_Themes.end()) {
      return it->second.GetProperty(key);
    }
    return "undefined";
  }

  std::string Application::GetActiveThemeProperty(const std::string &key) {
    for (auto it = m_ActiveThemes.rbegin(); it != m_ActiveThemes.rend(); ++it) {
      std::string val = it->GetProperty(key);
      if (val != "undefined") {
        return val;
      }
    }
    return "undefned";
  }

  std::string Application::CookPath(const std::string &input_path) {
    static const std::string root_path = []() {
      std::string path;

#ifdef _WIN32
      char result[MAX_PATH];
      if (GetModuleFileNameA(nullptr, result, MAX_PATH)) {
        path = std::filesystem::path(result).parent_path().string();
      } else {
        std::cerr << "Failed to get root path (Windows)\n";
      }

#elif defined(__APPLE__)
      char result[PATH_MAX];
      uint32_t size = sizeof(result);
      if (_NSGetExecutablePath(result, &size) == 0) {
        path = std::filesystem::path(result).parent_path().string();
      } else {
        std::cerr << "Failed to get root path (macOS)\n";
      }

#else  // Linux
      char result[PATH_MAX];
      ssize_t count = readlink("/proc/self/exe", result, sizeof(result) - 1);
      if (count != -1) {
        result[count] = '\0';
        path = std::filesystem::path(result).parent_path().string();
      } else {
        std::cerr << "Failed to get root path (Linux)\n";
      }
#endif

      return path;
    }();

    return (input_path.empty() || input_path.front() == '/') ? std::string(input_path)
                                                             : root_path + "/" + std::string(input_path);
  }

  std::string Application::GetComponentData(const Identifier &id, const std::string &topic) {
    if (id.component_array_ptr() != nullptr) {
      for (const auto &component : id.component_array_ptr()->IdentifiedComponents) {
        if (component->GetIdentifier() == id) {
          return component->GetData(topic);
        }
      }
    } else {
      ComponentsPool *pool = GetComponentPool();
      for (const auto &component : pool->IdentifiedComponents) {
        if (component->GetIdentifier() == id) {
          return component->GetData(topic);
        }
      }
    }

    return "undefined";
  }

  std::string Application::PutWindow(const std::shared_ptr<AppWindow> &win) {
    if (Cherry::IsReady()) {
      m_AppWindows.push_back(win);
    } else {
      Application::GetTempAppWindows().push_back(win);
    }

    return "id";
  }

  void Application::AddFont(const std::string &name, const std::string &ttf_file_path, const float &size) {
    m_CustomFonts.push_back({ name, { ttf_file_path, size } });
  };

  void Application::SetFavIconPath(const std::string &icon_path) {
    m_FavIconPath = icon_path;
  }

  void Application::AddLocale(const std::string &locale_name, const std::string &data_path) {
    std::ifstream file(data_path);
    if (file.is_open()) {
      nlohmann::json json_data;
      file >> json_data;
      file.close();

      if (m_Locales.find(locale_name) != m_Locales.end()) {
        auto &existing_locale = m_Locales[locale_name]["locales"];
        for (const auto &new_item : json_data["locales"]) {
          existing_locale.push_back(new_item);
        }
      } else {
        m_Locales[locale_name] = json_data;
      }
    }
  }

  void Application::SetLocale(const std::string &locale_name) {
    if (m_Locales.find(locale_name) != m_Locales.end()) {
      m_SelectedLocale = locale_name;
    }
  }

  void Application::SetDefaultLocale(const std::string &locale_name) {
    if (m_Locales.find(locale_name) != m_Locales.end()) {
      m_DefaultLocale = locale_name;
    }
  }

  void Application::PushPermanentProperty(const std::string &property, const std::string &value) {
    m_PermanentProperties.push_back({ property, value });
  }

  void Application::PopPermanentProperty(int number_of_pops) {
    if (m_PermanentProperties.empty())
      return;

    int pops = (number_of_pops == 0) ? 1 : number_of_pops;

    if (pops > static_cast<int>(m_PermanentProperties.size())) {
      pops = static_cast<int>(m_PermanentProperties.size());
    }

    for (int i = 0; i < pops; ++i) {
      m_PermanentProperties.pop_back();
    }
  }

  void Application::ClearOneTimeProperties() {
    m_OnTimeProperties.clear();
  }

  void Application::RemoveOneTimeProperty(const std::string &key) {
    m_OnTimeProperties.erase(key);
  }

  const std::unordered_map<std::string, std::string> &Application::GetOneTimeProperties() {
    return m_OnTimeProperties;
  }

  void Application::AddOneTimeProperty(const std::string &property, const std::string &value) {
    m_OnTimeProperties[property] = value;
  }

  void Application::PushParentComponent(const std::shared_ptr<Component> &component) {
    m_ParentComponentsStack.push_back(component);
  }

  void Application::PopParentComponent() {
    if (!m_ParentComponentsStack.empty()) {
      m_ParentComponentsStack.pop_back();
    }
  }

  std::shared_ptr<Component> Application::GetParent(int parent_number) {
    if (parent_number < 0 || parent_number >= static_cast<int>(m_ParentComponentsStack.size())) {
      return nullptr;
    }
    return m_ParentComponentsStack[m_ParentComponentsStack.size() - 1 - parent_number];
  }

  std::string Application::GetSelectedLocale() {
    return m_SelectedLocale;
  }

  std::string Application::GetLocale(const std::string &locale_type) {
    if (m_SelectedLocale.empty() || m_Locales.find(m_SelectedLocale) == m_Locales.end()) {
      return "locale_undefined";
    }

    const nlohmann::json &current_locale = m_Locales[m_SelectedLocale];

    for (const auto &item : current_locale["locales"]) {
      if (item.contains(locale_type)) {
        return item[locale_type].get<std::string>();
      }
    }

    // Get the default traductor if wanted locales not finded.
    if (m_DefaultLocale.empty() || m_Locales.find(m_DefaultLocale) == m_Locales.end()) {
      return "locale_undefined";
    }

    const nlohmann::json &current_def_locale = m_Locales[m_DefaultLocale];

    for (const auto &item : current_def_locale["locales"]) {
      if (item.contains(locale_type)) {
        return item[locale_type].get<std::string>();
      }
    }

    return "locale_undefined";
  }

  void Application::OverrideLocale(const std::string &locale_name, const std::string &data_path) {
    std::ifstream file(data_path);
    if (!file.is_open())
      return;

    nlohmann::json json_data;
    file >> json_data;
    file.close();

    if (m_Locales.find(locale_name) == m_Locales.end()) {
      m_Locales[locale_name] = json_data;
      return;
    }

    auto &existing_locales = m_Locales[locale_name]["locales"];

    for (const auto &new_item : json_data["locales"]) {
      auto new_it = new_item.begin();
      const std::string &new_key = new_it.key();
      const auto &new_value = new_it.value();

      bool replaced = false;

      for (auto &existing_item : existing_locales) {
        if (existing_item.contains(new_key)) {
          existing_item[new_key] = new_value;
          replaced = true;
          break;
        }
      }

      if (!replaced) {
        existing_locales.push_back(new_item);
      }
    }
  }

  Identifier Application::GetAnonymousID(const std::string &label) {
    std::size_t label_hash = std::hash<std::string>{}(label);
    return Identifier(std::to_string(label_hash));
  }

  Component &Application::GetAnonymousComponent(const Identifier &identifier) {
    if (identifier.component_array_ptr() != nullptr) {
      for (const auto &existing_component : identifier.component_array_ptr()->IdentifiedComponents) {
        if (existing_component->GetIdentifier() == identifier) {
          return existing_component ? *existing_component : s_EmptyComponent;
        }
      }
    } else {
      ComponentsPool *pool = Application::Get().GetComponentPool();
      for (const auto &existing_component : pool->AnonymousComponents) {
        if (existing_component->GetIdentifier() == identifier) {
          return existing_component ? *existing_component : s_EmptyComponent;
        }
      }
    }

    return s_EmptyComponent;
  }

  Component &Application::GetComponent(const Identifier &identifier) {
    auto &components = identifier.component_array_ptr() != nullptr
                           ? identifier.component_array_ptr()->IdentifiedComponents
                           : Application::Get().GetComponentPool()->IdentifiedComponents;

    for (const auto &existing_component : components) {
      if (existing_component && existing_component->GetIdentifier() == identifier) {
        return *existing_component;
      }
    }

    return s_EmptyComponent;
  }

  std::shared_ptr<Component> Application::GetComponentPtr(const Identifier &identifier) {
    auto &components = identifier.component_array_ptr() != nullptr
                           ? identifier.component_array_ptr()->IdentifiedComponents
                           : Application::Get().GetComponentPool()->IdentifiedComponents;

    for (const auto &existing_component : components) {
      if (existing_component && existing_component->GetIdentifier() == identifier) {
        return existing_component;
      }
    }

    return nullptr;
  }

  void Application::SetDescriptionForAppWindow(const std::string &windowId, const std::string &description) {
    m_AppWindowRegistry[windowId].Description = description;
  }

  void Application::SetCloseCallback(const std::function<void()> &closeCallback) {
    m_CloseCallback = closeCallback;
  }
  void Application::SetMenubarCallback(const std::function<void()> &menubarCallback) {
    m_MenubarCallback = menubarCallback;
  }
  void Application::SetFramebarCallback(const std::function<void()> &framebarCallback) {
    m_FramebarCallback = framebarCallback;
  }
  void Application::SetCloseCallback(const std::function<bool()> &closeCallback) {
    m_CloseCallback = closeCallback;
  }
  void Application::SetMainRenderCallback(const std::function<void()> &mainRenderCallback) {
    m_MainRenderCallback = mainRenderCallback;
  }

  std::string Application::GetRootPath() {
    return m_RootPath;
  }

  bool Application::IsTitleBarHovered() const {
    return m_TitleBarHovered;
  }

  std::string Application::GetName() {
    return m_DefaultSpecification.Name;
  }

  Identifier Application::GetAnonymousID() {
    return Identifier(Identifier::GetUniqueIndex());
  }

  void Application::PushComponentRenderMode(Cherry::RenderMode render_mode) {
    m_RenderModeStack.push_back(render_mode);
  }

  void Application::PopRenderMode(int number) {
    for (int i = 0; i < number && !m_RenderModeStack.empty(); ++i) {
      m_RenderModeStack.pop_back();
    }
  }

  Cherry::RenderMode Application::GetRenderMode() const {
    auto stackCopy = m_RenderModeStack;
    if (!stackCopy.empty()) {
      return stackCopy.back();
    }

    Cherry::RenderMode fallback = CherryNextComponent.GetRenderMode();

    if (IsValidRenderMode(fallback)) {
      return fallback;
    }

    return Cherry::RenderMode::None;
  }

  bool Application::IsValidRenderMode(Cherry::RenderMode mode) const {
    return mode != Cherry::RenderMode::None;
  }

  void Application::SetLogoPathForAppWindow(const std::string &windowId, const std::string &path) {
    m_AppWindowRegistry[windowId].LogoPath = path;
  }

  const std::string &Application::GetDescriptionForAppWindow(const std::string &windowId) {
    auto it = m_AppWindowRegistry.find(windowId);
    if (it != m_AppWindowRegistry.end()) {
      return it->second.Description;
    }
    static const std::string defaultDesc = "";
    return defaultDesc;
  }

  const std::string &Application::GetLogoPathForAppWindow(const std::string &windowId) {
    auto it = m_AppWindowRegistry.find(windowId);
    if (it != m_AppWindowRegistry.end()) {
      return it->second.LogoPath;
    }
    static const std::string emptyPath = "";
    return emptyPath;
  }

  // Simplicity utils

  std::shared_ptr<Cherry::Window> &GetCurrentRenderedWindow() {
    return Application::GetCurrentRenderedWindow();
  }

  void AddAppWindow(const std::shared_ptr<AppWindow> &win) {
    if (win) {
      Application::Get().PutWindow(win);
    }
  }

  std::shared_ptr<AppWindow> GetAppWindowByName(const std::string &win_name) {
    for (auto &appwin : s_Instance->GetAppWindows()) {
      if (appwin->m_IdName == win_name) {
        return appwin;
      }
    }
    return nullptr;
  }

  std::shared_ptr<Window> GetWindowByName(const std::string &win_name) {
    for (auto &win : s_Instance->GetWindows()) {
      if (win->GetName() == win_name) {
        return win;
      }
    }
    return nullptr;
  }

  void Application::DeleteAppWindow(const std::shared_ptr<AppWindow> &win) {
    if (win) {
      if (m_AppWindows.size() == 1) {
        EnableNoAppWindowSafety();
      }

      win->SetParentWindow("__blank");
      m_AppWindows.erase(std::remove(m_AppWindows.begin(), m_AppWindows.end(), win), m_AppWindows.end());
    }
  }

  void Application::EnableNoAppWindowSafety() {
    m_NoAppWindowSafetyEnabled = true;
    std::shared_ptr<AppWindow> app_window =
        std::make_shared<AppWindow>("cherryinternal_no_windows", "cherryinternal_no_windows");
    app_window->SetRenderCallback([this]() {
      m_NoAppWindowSafetyEnabled = true;
      ImGui::TextColored(
          Cherry::HexToRGBA("#FF2222FF"),
          "ERROR: You selected the Docking or Tabbed render mode, but you did "
          "not specify "
          "any app windows. Please create and add an "
          "app window. (Cherry::AddAppWindow(std::shared_ptr<AppWindow>))");
    });

    Cherry::AddAppWindow(app_window);
  }

  void Application::DisableNoAppWindowSafety() {
    Cherry::DeleteAppWindow(Cherry::GetAppWindowByName("cherryinternal_no_windows"));
    m_NoAppWindowSafetyEnabled = false;
  }

  void DeleteAppWindow(const std::shared_ptr<AppWindow> &win) {
    Application::Get().DeleteAppWindow(win);
  }

  ImTextureID GetTexture(const std::string &path) {
    return Application::Get().GetCurrentRenderedWindow()->GetTexture(path);
  }

  ImVec2 GetTextureSize(const std::string &path) {
    return Application::Get().GetCurrentRenderedWindow()->GetTextureSize(path);
  }

#ifdef _WIN32
  std::string convertPathToWindowsStyle(const std::string &path) {
    std::string windowsPath = path;
    std::replace(windowsPath.begin(), windowsPath.end(), '/', '\\');
    return windowsPath;
  }
#endif

  std::string GetPath(const std::string &path) {
#ifdef _WIN32
    return convertPathToWindowsStyle(Application::CookPath(path));
#else
    return Application::CookPath(path);
#endif
  }

#ifdef CHERRY_ENABLE_NET
  static std::unordered_map<std::string, std::shared_ptr<std::atomic<int>>> s_http_pending;
  static std::unordered_map<std::string, std::string> s_http_ready;
  static std::atomic<bool> s_http_shutdown{ false };

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    std::ofstream *ofs = static_cast<std::ofstream *>(userp);
    size_t totalSize = size * nmemb;
    ofs->write(static_cast<char *>(contents), totalSize);
    return totalSize;
  }

  static std::string SanitizeUrl(const std::string &url) {
    std::string sanitized;
    for (char c : url) {
      if (std::isalnum(static_cast<unsigned char>(c)) || c == '.' || c == '-') {
        sanitized += c;
      } else {
        sanitized += '_';
      }
    }
    if (sanitized.size() > 200) {
      sanitized = sanitized.substr(0, 200);
    }
    return sanitized;
  }

  static std::string GetTemporaryDirectory() {
#ifdef _WIN32
    char tempPath[MAX_PATH];
    if (GetTempPathA(MAX_PATH, tempPath)) {
      std::string p(tempPath);
      if (!p.empty() && p.back() == '\\')
        p.pop_back();
      return p;
    } else {
      throw std::runtime_error("Failed to get temporary directory path");
    }
#elif defined(__APPLE__)
    const char *tmpDir = getenv("TMPDIR");
    if (tmpDir) {
      std::string p(tmpDir);
      if (!p.empty() && p.back() == '/')
        p.pop_back();
      return p;
    }
    return "/tmp";
#else
    const char *tmpDir = getenv("TMPDIR");
    if (tmpDir) {
      return std::string(tmpDir);
    }
    return "/tmp";
#endif
  }

  void InitHttpFetcher() {
    curl_global_init(CURL_GLOBAL_ALL);
  }

  void ShutdownHttpFetcher() {
    s_http_shutdown = true;
    s_http_pending.clear();
    s_http_ready.clear();
    curl_global_cleanup();
  }

  std::string GetHttpPath(const std::string &url) {
    std::string cache_path = GetTemporaryDirectory() + "/" + Application::Get().GetHttpCacheFolderName() + "/";
    if (!fs::exists(cache_path)) {
      fs::create_directories(cache_path);
    }
    std::string filename = SanitizeUrl(url);
    std::string file_path = cache_path + filename;
    // fallback to trigger texture fail prevention of Cherry
    std::string fallback_path = cache_path + "no_file.png";

    if (s_http_ready.count(url)) {
      return s_http_ready[url];
    }

    if (fs::exists(file_path) && fs::file_size(file_path) > 0) {
      s_http_ready[url] = file_path;
      return file_path;
    }

    if (s_http_pending.count(url)) {
      auto &state = s_http_pending[url];
      int val = state->load();
      if (val == 1) {
        if (fs::exists(file_path) && fs::file_size(file_path) > 0) {
          s_http_ready[url] = file_path;
          s_http_pending.erase(url);
          return file_path;
        } else {
          std::cerr << "[HTTP] state==1 but file missing, retrying: " << url << std::endl;
          s_http_pending.erase(url);
        }
      } else if (val == -1) {
        std::cerr << "[HTTP] Thread failed, retrying: " << url << std::endl;
        s_http_pending.erase(url);
      } else {
        return fallback_path;
      }
    }

    auto state = std::make_shared<std::atomic<int>>(0);
    s_http_pending[url] = state;

    std::thread([url, file_path, state]() {
      CURL *curl = curl_easy_init();
      if (!curl) {
        std::cerr << "[HTTP] curl_easy_init failed" << std::endl;
        state->store(-1);
        return;
      }

      std::string tmp_path = file_path + ".tmp";
      std::ofstream ofs(tmp_path, std::ios::binary);
      if (!ofs.is_open()) {
        std::cerr << "[HTTP] Failed to open tmp file: " << tmp_path << std::endl;
        curl_easy_cleanup(curl);
        state->store(-1);
        return;
      }

      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
#ifdef _WIN32
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

      CURLcode res = curl_easy_perform(curl);
      ofs.close();

      if (res != CURLE_OK) {
        std::cerr << "[HTTP] curl error: " << curl_easy_strerror(res) << std::endl;
        fs::remove(tmp_path);
        curl_easy_cleanup(curl);
        state->store(-1);
        return;
      }

      long http_code = 0;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
      curl_easy_cleanup(curl);

      if (http_code != 200) {
        fs::remove(tmp_path);
        state->store(-1);
        return;
      }

      std::error_code ec;
      fs::rename(tmp_path, file_path, ec);
      if (ec) {
        std::cerr << "[HTTP] Rename failed: " << ec.message() << std::endl;
        fs::remove(tmp_path);
        state->store(-1);
        return;
      }

      state->store(1);
    }).detach();

    return fallback_path;
  }
#endif  // CHERRY_ENABLE_NET

  std::string GetLocale(const std::string &topic) {
    return Application::Get().GetLocale(topic);
  }

  bool IsReady() {
    if (&Application::Get() == 0) {
      return false;
    }
    return true;
  }

  float GetDeltaTime() {
    if (ImGui::GetCurrentContext()) {
      return ImGui::GetIO().DeltaTime;
    }
    return 0.0f;
  }

  float GetTime() {
    if (ImGui::GetCurrentContext()) {
      return (float)ImGui::GetTime();
    }
    return 0.0f;
  }

  std::string GetData(const Identifier &id, const std::string topic) {
    return CherryApp.GetComponentData(id, topic);
  }

  void PushPermanentProperty(const std::string &property, const std::string &value) {
    Application::Get().PushPermanentProperty(property, value);
  }

  void PopPermanentProperty(int number_of_pops) {
    Application::Get().PopPermanentProperty(number_of_pops);
  }

  void SetNextComponentProperty(const std::string &property, const std::string &value) {
    Application::Get().AddOneTimeProperty(property, value);
  }

  void PushParentComponent(const std::shared_ptr<Component> &component) {
    Application::Get().PushParentComponent(component);
  }

  void PopParentComponent() {
    Application::Get().PopParentComponent();
  }

  std::shared_ptr<Component> GetParent(int parent_number) {
    return Application::Get().GetParent(parent_number);
  }

  void AddNotification(const ImGuiToast &toast) {
    ImGui::InsertNotification(toast);
  }

  void PushFont(const std::string &font_name) {
    auto font = Cherry::Application::GetFont(font_name);

    if (font) {
      ImGui::PushFont(font);
    } else {
      ImGui::PushFont(ImGui::GetFont());
    }
  }

  void PopFont() {
    ImGui::PopFont();
  }

}  // namespace Cherry
