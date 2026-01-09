#include "app.hpp"

#include "../../../lib/sdl2/include/SDL.h"
#include "../../../lib/sdl2/include/SDL_vulkan.h"
#include "../../../src/core/log.hpp"
#include "../app_window/app_window.hpp"
#include "../base.hpp"
#include "../components/components.hpp"
#include "../window/window.hpp"

/**
 * @file app.cpp
 * @brief All sources of master window behaviors & render engine.
 */

#include <stdio.h>  // printf, fprintf
#include <stdlib.h> // abort
#include <vulkan/vulkan.h>

#include <algorithm>
#include <iostream>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread> // thread

#ifdef CHERRY_ENABLE_NET

#include <filesystem>
#include <fstream>
#include <string>

#include "../../../lib/httpcl/httpcl.h"

#ifdef _WIN32
#include <windows.h>
#define NOMINMAX // std::max

#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif // _WIN32
#endif // CHERRY_ENABLE_NET

// Emedded font
#include "../imgui/Hack-Regular.embed"
#include "../imgui/Inconsolatas.embed"
#include "../imgui/Roboto-Bold.embed"
#include "../imgui/Roboto-Italic.embed"
#include "../imgui/Roboto-Regular.embed"
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
static std::vector<std::shared_ptr<Cherry::AppWindow>>
    g_TempAppWindows; // To be able to create app window before create
                      // applications

static bool g_LogicalDeviceInitialized = false;
static int g_MinImageCount = 0;
static int c_WindowsCount = 0;
static std::string LastWindowPressed = "";
static int RedockCount = 0;
static bool DragRendered = false;

static std::shared_ptr<Cherry::RedockRequest> LatestRequest;
static std::vector<std::pair<Cherry::ProcessCallback, std::function<void()>>>
    g_ProcessCallbacks;

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
static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
             uint64_t object, size_t location, int32_t messageCode,
             const char *pLayerPrefix, const char *pMessage, void *pUserData) {
  (void)flags;
  (void)object;
  (void)location;
  (void)messageCode;
  (void)pUserData;
  (void)
      pLayerPrefix; // Unused arguments
                    //	fprintf(stderr, "[vulkan] Debug report from
                    // ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
  return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace Cherry {

#include "../embed/not_found_img.embed"
#include "../embed/window.embed"

Application::Application(const ApplicationSpecification &specification)
    : m_DefaultSpecification(specification) {
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

Application::~Application() { Shutdown(); }

void Application::SetMasterSwapChainRebuild(const bool &new_state) {
  c_MasterSwapChainRebuild = new_state;
}

void Application::IncrementWindowNumber() { c_WindowsCount++; }

void Application::SetupVulkan(const char **extensions,
                              uint32_t extensions_count) {
  VkResult err;

  // Create Vulkan Instance
  if (!g_LogicalDeviceInitialized) {
    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.enabledExtensionCount = extensions_count;
    create_info.ppEnabledExtensionNames = extensions;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
    // Enabling validation layers
    const char *layers[] = {"VK_LAYER_KHRONOS_validation"};
    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = layers;

    // Enable debug report extension (we need additional storage, so we
    // duplicate the user array to add our new extension to it)
    const char **extensions_ext =
        (const char **)malloc(sizeof(const char *) * (extensions_count + 1));
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
        (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
            g_Instance, "vkCreateDebugReportCallbackEXT");
    IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

    // Setup the debug report callback
    VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
    debug_report_ci.sType =
        VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                            VK_DEBUG_REPORT_WARNING_BIT_EXT |
                            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    debug_report_ci.pfnCallback = debug_report;
    debug_report_ci.pUserData = NULL;
    err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci,
                                         g_Allocator, &g_DebugReport);
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

    VkPhysicalDevice *gpus =
        (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * gpu_count);
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
    VkQueueFamilyProperties *queues = (VkQueueFamilyProperties *)malloc(
        sizeof(VkQueueFamilyProperties) * count);
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
    const char *device_extensions[] = {"VK_KHR_swapchain"};
    const float queue_priority[] = {1.0f};
    VkDeviceQueueCreateInfo queue_info[1] = {};
    queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info[0].queueFamilyIndex = g_QueueFamily;
    queue_info[0].queueCount = 1;
    queue_info[0].pQueuePriorities = queue_priority;
    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount =
        sizeof(queue_info) / sizeof(queue_info[0]);
    create_info.pQueueCreateInfos = queue_info;
    create_info.enabledExtensionCount = device_extension_count;
    create_info.ppEnabledExtensionNames = device_extensions;
    err =
        vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
    check_vk_result(err);
    vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
    g_LogicalDeviceInitialized = true;
  }

  // Create Descriptor Pool
  {
    VkDescriptorPoolSize pool_sizes[] = {
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
    err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator,
                                 &g_DescriptorPool);
    check_vk_result(err);
  }
}

// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used
// by the demo. Your real engine/app may not use them.
void Application::SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd,
                                    VkSurfaceKHR surface, int width, int height,
                                    Cherry::Window *win) {
  wd->Surface = surface;

  // Check for WSI support
  VkBool32 res;
  vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily,
                                       wd->Surface, &res);
  if (res != VK_TRUE) {
    fprintf(stderr, "Error no WSI support on physical device 0\n");
    exit(-1);
  }

  // Select Surface Format
  const VkFormat requestSurfaceImageFormat[] = {
      VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM,
      VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
  const VkColorSpaceKHR requestSurfaceColorSpace =
      VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
      g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat,
      (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
      requestSurfaceColorSpace);

  // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR,
                                      VK_PRESENT_MODE_IMMEDIATE_KHR,
                                      VK_PRESENT_MODE_FIFO_KHR};
#else
  VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
  wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(
      g_PhysicalDevice, wd->Surface, &present_modes[0],
      IM_ARRAYSIZE(present_modes));
  // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

  // Create SwapChain, RenderPass, Framebuffer, etc.
  IM_ASSERT(g_MinImageCount >= 2);
  ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device,
                                         wd, g_QueueFamily, g_Allocator, width,
                                         height, g_MinImageCount);
}

void Application::CleanupVulkanWindow(Cherry::Window *win) {
  if (!win)
    return;

  win->m_WinData.Surface = win->m_Surface;

  ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &win->m_WinData,
                                  g_Allocator);

  win->m_Surface = VK_NULL_HANDLE;
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
      (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
          g_Instance, "vkDestroyDebugReportCallbackEXT");
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
Application &Application::Get() { return *s_Instance; }

VkDevice &Application::GetVkDevice() { return g_Device; }

VkPhysicalDevice &Application::GetVkPhysicalDevice() {
  return g_PhysicalDevice;
}

uint32_t &Application::GetQueueFamily() { return g_QueueFamily; }

VkQueue &Application::GetVkQueue() { return g_Queue; }

VkAllocationCallbacks &Application::GetVkAllocationCallbacks() {
  return *g_Allocator;
}

VkDebugReportCallbackEXT &Application::GetVkDebugReportCallbackEXT() {
  return g_DebugReport;
}

void Application::SetExecutablePath() { g_ExecutablePath = "test"; }
std::string &Application::GetExecutablePath() { return g_ExecutablePath; }

VkPipelineCache &Application::GetVkPipelineCache() { return g_PipelineCache; }

VkDescriptorPool &Application::GetVkDescriptorPool() {
  return g_DescriptorPool;
}

int &Application::GetMinImageCount() { return g_MinImageCount; }

std::shared_ptr<Cherry::Window> &Application::GetCurrentRenderedWindow() {
  return c_CurrentRenderedWindow;
}

Cherry::Component &Application::GetSafeLastComponent() {
  return c_LastComponent ? *c_LastComponent : s_EmptyComponent;
}

void Application::SetLastComponent(Component *component) {
  c_LastComponent = component;
}

void Application::ResetLastComponent() { c_LastComponent = nullptr; }

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

void Application::ResetCurrentComponent() { c_CurrentComponent = nullptr; }

std::shared_ptr<Cherry::AppWindow> &Application::GetCurrentRenderedAppWindow() {
  return c_CurrentRenderedAppWindow;
}

Cherry::Window &Application::GetSafeCurrentRenderedWindow() {
  return c_CurrentRenderedWindow ? *c_CurrentRenderedWindow : s_EmptyWindow;
}

Cherry::AppWindow &Application::GetSafeCurrentRenderedAppWindow() {
  return c_CurrentRenderedAppWindow ? *c_CurrentRenderedAppWindow
                                    : s_EmptyAppWindow;
}

std::shared_ptr<Cherry::WindowDragDropState> &
Application::GetCurrentDragDropState() {
  return c_CurrentDragDropState;
}

bool &Application::GetDockIsDragging() { return c_DockIsDragging; }

std::string &Application::GetLastWindowPressed() { return LastWindowPressed; }

std::vector<std::shared_ptr<Cherry::AppWindow>> &
Application::GetTempAppWindows() {
  return g_TempAppWindows;
}

std::vector<std::pair<std::string, std::pair<std::string, float>>> &
Application::GetCustomFonts() {
  return m_CustomFonts;
}

bool &Application::GetValidDropZoneFounded() { return c_ValidDropZoneFounded; }

void Application::SetValidDropZoneFounded(const bool &founded) {
  c_ValidDropZoneFounded = founded;
}

void Application::SetCurrentDragDropState(
    const std::shared_ptr<Cherry::WindowDragDropState> &state) {
  c_CurrentDragDropState = state;
}

void Application::SetCurrentDragDropStateDragOwner(
    const std::string &new_name) {
  c_CurrentDragDropState->DragOwner = new_name;
}

void Application::SetCurrentDragDropStateDockIsDragging(
    const bool &is_dragging) {
  c_CurrentDragDropState->DockIsDragging = is_dragging;
}

void Application::SetCurrentDragDropStateAppWindow(
    const std::string &new_name) {
  c_CurrentDragDropState->LastDraggingAppWindow = new_name;
}

void Application::SetCurrentDragDropStateAppWindowHost(
    const std::string &new_name) {
  c_CurrentDragDropState->LastDraggingAppWindowHost = new_name;
}

void Application::SetCurrentDragDropStateWindow(const std::string &new_name) {
  c_CurrentDragDropState->LastDraggingWindow = new_name;
}

void Application::SetLastWindowPressed(const std::string &name) {
  LastWindowPressed = name;
}

void Application::SetCurrentDragDropStateDraggingPlace(
    const DockEmplacement &place) {
  c_CurrentDragDropState->LastDraggingPlace = place;
}

std::unordered_map<std::string, ImFont *> &Application::GetFontList() {
  return s_Fonts;
}

void Application::AddProcessCallback(ProcessCallback process,
                                     const std::function<void()> callback) {
  g_ProcessCallbacks.push_back({process, callback});
}

const std::unordered_map<CherryKey, SDL_Scancode> &Application::GetKeyMap() {
  static std::unordered_map<CherryKey, SDL_Scancode> keyMap = {
      {CherryKey::A, SDL_SCANCODE_A},
      {CherryKey::B, SDL_SCANCODE_B},
      {CherryKey::C, SDL_SCANCODE_C},
      {CherryKey::D, SDL_SCANCODE_D},
      {CherryKey::E, SDL_SCANCODE_E},
      {CherryKey::F, SDL_SCANCODE_F},
      {CherryKey::G, SDL_SCANCODE_G},
      {CherryKey::H, SDL_SCANCODE_H},
      {CherryKey::I, SDL_SCANCODE_I},
      {CherryKey::J, SDL_SCANCODE_J},
      {CherryKey::K, SDL_SCANCODE_K},
      {CherryKey::L, SDL_SCANCODE_L},
      {CherryKey::M, SDL_SCANCODE_M},
      {CherryKey::N, SDL_SCANCODE_N},
      {CherryKey::O, SDL_SCANCODE_O},
      {CherryKey::P, SDL_SCANCODE_P},
      {CherryKey::Q, SDL_SCANCODE_Q},
      {CherryKey::R, SDL_SCANCODE_R},
      {CherryKey::S, SDL_SCANCODE_S},
      {CherryKey::T, SDL_SCANCODE_T},
      {CherryKey::U, SDL_SCANCODE_U},
      {CherryKey::V, SDL_SCANCODE_V},
      {CherryKey::W, SDL_SCANCODE_W},
      {CherryKey::X, SDL_SCANCODE_X},
      {CherryKey::Y, SDL_SCANCODE_Y},
      {CherryKey::Z, SDL_SCANCODE_Z},
      {CherryKey::NUM_0, SDL_SCANCODE_0},
      {CherryKey::NUM_1, SDL_SCANCODE_1},
      {CherryKey::NUM_2, SDL_SCANCODE_2},
      {CherryKey::NUM_3, SDL_SCANCODE_3},
      {CherryKey::NUM_4, SDL_SCANCODE_4},
      {CherryKey::NUM_5, SDL_SCANCODE_5},
      {CherryKey::NUM_6, SDL_SCANCODE_6},
      {CherryKey::NUM_7, SDL_SCANCODE_7},
      {CherryKey::NUM_8, SDL_SCANCODE_8},
      {CherryKey::NUM_9, SDL_SCANCODE_9},
      {CherryKey::ESCAPE, SDL_SCANCODE_ESCAPE},
      {CherryKey::SPACE, SDL_SCANCODE_SPACE},
      {CherryKey::KEY_DELETE, SDL_SCANCODE_DELETE},
      {CherryKey::ENTER, SDL_SCANCODE_RETURN},
      {CherryKey::BACKSPACE, SDL_SCANCODE_BACKSPACE},
      {CherryKey::TAB, SDL_SCANCODE_TAB},
      {CherryKey::SHIFT, SDL_SCANCODE_LSHIFT},
      {CherryKey::CTRL, SDL_SCANCODE_LCTRL},
      {CherryKey::ALT, SDL_SCANCODE_LALT},
      {CherryKey::LEFT, SDL_SCANCODE_LEFT},
      {CherryKey::RIGHT, SDL_SCANCODE_RIGHT},
      {CherryKey::UP, SDL_SCANCODE_UP},
      {CherryKey::DOWN, SDL_SCANCODE_DOWN},
      {CherryKey::F1, SDL_SCANCODE_F1},
      {CherryKey::F2, SDL_SCANCODE_F2},
      {CherryKey::F3, SDL_SCANCODE_F3},
      {CherryKey::F4, SDL_SCANCODE_F4},
      {CherryKey::F5, SDL_SCANCODE_F5},
      {CherryKey::F6, SDL_SCANCODE_F6},
      {CherryKey::F7, SDL_SCANCODE_F7},
      {CherryKey::F8, SDL_SCANCODE_F8},
      {CherryKey::F9, SDL_SCANCODE_F9},
      {CherryKey::F10, SDL_SCANCODE_F10},
      {CherryKey::F11, SDL_SCANCODE_F11},
      {CherryKey::F12, SDL_SCANCODE_F12}};
  return keyMap;
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

void Application::PushRedockEvent(
    const std::shared_ptr<Cherry::WindowDragDropState> &state) {
  for (auto app_win : s_Instance->m_AppWindows) {
    if (app_win->m_IdName == state->LastDraggingAppWindowHost) {
      std::shared_ptr<Cherry::RedockRequest> req = app_win->CreateRedockEvent(
          state->LastDraggingWindow, state->LastDraggingPlace, state->FromSave,
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
  Cherry::Application::DebugToolState().load(std::memory_order_acquire);
  Cherry::Application::DebugToolState().store(false);
#endif
#ifdef CHERRY_ENABLE_AUDIO
  // Init audio service if needed
  if (app->m_DefaultSpecification.UseAudioService) {
    StartAudioService();
  }
#endif

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
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
    name =
        CertifyWindowName(s_Instance->m_DefaultSpecification.DefaultWindowName);
  }

  this->m_Windows.push_back(std::make_shared<Window>(
      name, app->m_DefaultSpecification.Width,
      app->m_DefaultSpecification.Height, app->m_DefaultSpecification));
}

std::atomic<bool> &Application::RunningState() {
  static std::atomic<bool> running{true};
  return running;
}

void Application::RequestShutdown() {
  RunningState().store(false, std::memory_order_release);
}

#ifdef CHERRY_DEBUG
std::atomic<bool> &Application::DebugToolState() {
  static std::atomic<bool> devtools{true};
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
#endif // CHERRY_DEBUG

void Application::Shutdown() {
  RequestShutdown();

  for (auto &layer : m_LayerStack)
    layer->OnDetach();
  m_LayerStack.clear();

#ifdef CHERRY_ENABLE_AUDIO
  if (app->m_DefaultSpecification.UseAudioService) {
    StopAudioService();
  }
#endif

  if (g_Device != VK_NULL_HANDLE) {
    vkDeviceWaitIdle(g_Device);
  }

  for (size_t i = 0; i < m_Windows.size(); ++i) {
    auto &window = m_Windows[i];
    if (!window)
      continue;

    window->m_IsClosing = true;
    if (window->m_ImGuiContext) {
      ImGui::SetCurrentContext(window->m_ImGuiContext);
      ImGui_ImplVulkan_Shutdown();
      ImGui_ImplSDL2_Shutdown();
      ImGui::DestroyContext(window->m_ImGuiContext);
      window->m_ImGuiContext = nullptr;
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
      window->m_ImageMap.clear();
      window->m_HexImageMap.clear();
    }
  }

  m_Windows.clear();

  SDL_Quit();
  s_Instance = nullptr;
}

void Application::FrameRender(ImGui_ImplVulkanH_Window *wd, Cherry::Window *win,
                              ImDrawData *draw_data) {
  VkResult err;
  VkSemaphore image_acquired_semaphore =
      wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
  VkSemaphore render_complete_semaphore =
      wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;

  err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX,
                              image_acquired_semaphore, VK_NULL_HANDLE,
                              &wd->FrameIndex);

  if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
    win->g_SwapChainRebuild = true;

    return;
  }

  check_vk_result(err);

  ImGui_ImplVulkanH_Frame *fd = &wd->Frames[wd->FrameIndex];

  win->s_CurrentFrameIndex =
      (win->s_CurrentFrameIndex + 1) % win->m_WinData.ImageCount;

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
    VkPipelineStageFlags wait_stage =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
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

void Application::FramePresent(ImGui_ImplVulkanH_Window *wd,
                               Cherry::Window *win) {
  if (win->g_SwapChainRebuild)
    return;
  VkSemaphore render_complete_semaphore =
      wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
  VkPresentInfoKHR info = {};
  info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.waitSemaphoreCount = 1;
  info.pWaitSemaphores = &render_complete_semaphore;
  info.swapchainCount = 1;
  info.pSwapchains = &wd->Swapchain;
  info.pImageIndices = &wd->FrameIndex;
  VkResult err = vkQueuePresentKHR(g_Queue, &info);
  if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
    win->g_SwapChainRebuild = true;
    return;
  }
  check_vk_result(err);
  wd->SemaphoreIndex =
      (wd->SemaphoreIndex + 1) %
      wd->ImageCount; // Now we can use the next set of semaphores
}

std::string Application::SpawnWindow(ApplicationSpecification spec) {
  std::string name;

  if (spec.DefaultWindowName.empty()) {
    name = CertifyWindowName(spec.Name);
  } else {
    name = CertifyWindowName(spec.DefaultWindowName);
  }

  ImGuiContext *res_ctx = ImGui::GetCurrentContext();

  std::shared_ptr<Window> new_win =
      std::make_shared<Window>(name, spec.Width, spec.Height, spec);

  this->m_Windows.push_back(new_win);

  ImGui::SetCurrentContext(res_ctx);

  return name;
}

void Application::SpawnWindow(const std::string &winname,
                              ApplicationSpecification spec) {
  std::string name = CertifyWindowName(winname);

  ImGuiContext *res_ctx = ImGui::GetCurrentContext();

  std::shared_ptr<Window> new_win =
      std::make_shared<Window>(name, spec.Width, spec.Height, spec);

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
      name, app->m_DefaultSpecification.Width,
      app->m_DefaultSpecification.Height, app->m_DefaultSpecification);

  this->m_Windows.push_back(new_win);

  ImGui::SetCurrentContext(res_ctx);
  return name;
}

void Application::SpawnWindow(const std::string &winname) {
  std::string name = CertifyWindowName(winname);

  ImGuiContext *res_ctx = ImGui::GetCurrentContext();

  std::shared_ptr<Window> new_win = std::make_shared<Window>(
      name, app->m_DefaultSpecification.Width,
      app->m_DefaultSpecification.Height, app->m_DefaultSpecification);

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
    if (s_Instance->m_PreviousSaveData.find("data") !=
        s_Instance->m_PreviousSaveData.end()) {
      auto windowsJson = s_Instance->m_PreviousSaveData["data"].value(
          "windows", nlohmann::json::array());

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

std::vector<std::shared_ptr<AppWindow>>
Application::GetLastSaveInstanciableAppWindows() {
  std::vector<std::shared_ptr<AppWindow>> results;
  for (auto &savedappwins : Application::Get().m_SavedAppWindows) {
    if (savedappwins->m_AppWindowType == AppWindowTypes::InstanciableWindow) {
      results.push_back(savedappwins);
    }
  }
  return results;
}

void Application::ApplyDockingFromSave() {
  if (s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::DockingWindows ||
      s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::TabWidows ||
      s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::SimpleWindow) {
    for (auto &appwin : s_Instance->m_AppWindows) {
      bool dockplace_initialized = false;
      bool parent_initialized = false;
      bool win_initialized = false;
      bool sizex_initialized = false;

      if (!appwin->m_WindowRebuilded) {
        std::shared_ptr<WindowDragDropState> dragdropstate =
            std::make_shared<WindowDragDropState>();
        dragdropstate->LastDraggingAppWindowHost = appwin->m_IdName;
        dragdropstate->FromSave = true;
        LastWindowPressed = dragdropstate->LastDraggingAppWindowHost;
        for (auto &savedappwin : s_Instance->m_SavedAppWindows) {
          if (appwin->m_IdName == savedappwin->m_IdName) {
            if (savedappwin->GetFetchedSaveData("docknodeparent") !=
                "undefined") {
              dragdropstate->LastDraggingAppWindow =
                  savedappwin->GetFetchedSaveData("docknodeparent");
            }

            if (savedappwin->GetFetchedSaveData("dockplace") != "undefined") {
              dragdropstate->DragOwner = savedappwin->m_IdName;

              if (savedappwin->GetFetchedSaveData("dockplace") == "right") {
                dragdropstate->LastDraggingPlace = DockEmplacement::DockRight;
              } else if (savedappwin->GetFetchedSaveData("dockplace") ==
                         "left") {
                dragdropstate->LastDraggingPlace = DockEmplacement::DockLeft;
              }

              else if (savedappwin->GetFetchedSaveData("dockplace") == "up") {
                dragdropstate->LastDraggingPlace = DockEmplacement::DockUp;
              } else if (savedappwin->GetFetchedSaveData("dockplace") ==
                         "down") {
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
              dragdropstate->LastDraggingAppWindow =
                  savedappwin->GetFetchedSaveData("dockparent");
              dragdropstate->DragOwner =
                  savedappwin->GetFetchedSaveData("dockparent");
              bool parent_initialized = true;
            }

            if (savedappwin->GetFetchedSaveData("win") != "undefined") {
              dragdropstate->LastDraggingWindow =
                  savedappwin->GetFetchedSaveData("win");
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

          if (appwin->GetDefaultBehavior(
                  DefaultAppWindowBehaviors::DefaultDocking) == "right") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockRight;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "left") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockLeft;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "up") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockUp;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "down") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockDown;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "full") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
          } else {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
          }
        }

        if (!win_initialized && s_Instance->m_Windows[0]) {
          dragdropstate->LastDraggingWindow =
              s_Instance->m_Windows[0]->GetName();
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
  if (s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::DockingWindows ||
      s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::TabWidows ||
      s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::SimpleWindow) {
    for (auto &appwin : s_Instance->m_AppWindows) {
      if (!appwin->m_WindowRebuilded) {
        bool dockplace_initialized = false;
        bool parent_initialized = false;
        bool win_initialized = false;
        bool sizex_initialized = false;

        std::shared_ptr<WindowDragDropState> dragdropstate =
            std::make_shared<WindowDragDropState>();
        dragdropstate->LastDraggingAppWindowHost = appwin->m_IdName;
        LastWindowPressed = dragdropstate->LastDraggingAppWindowHost;
        dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;

        if (!dockplace_initialized) {
          dragdropstate->DragOwner = appwin->m_IdName;

          if (appwin->GetDefaultBehavior(
                  DefaultAppWindowBehaviors::DefaultDocking) == "right") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockRight;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "left") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockLeft;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "up") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockUp;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "down") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockDown;
          } else if (appwin->GetDefaultBehavior(
                         DefaultAppWindowBehaviors::DefaultDocking) == "full") {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
          } else {
            dragdropstate->LastDraggingPlace = DockEmplacement::DockFull;
          }
        }

        if (!win_initialized && s_Instance->m_Windows[0]) {
          dragdropstate->LastDraggingWindow =
              s_Instance->m_Windows[0]->GetName();
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

void Application::SetCurrentRedockEvent(
    const std::shared_ptr<Cherry::WindowDragDropState> &state) {
  c_CurrentDragDropState = state;
}

void Application::PresentAllWindows() {
  for (auto &window : m_Windows) {
    ImGui::SetCurrentContext(window->m_ImGuiContext);

    // Push window selected theme if defined
    if (window->m_SelectedTheme != "undefined") {
      PushTheme(window->m_SelectedTheme);
    }

    c_CurrentRenderedWindow = window;

    if (c_MasterSwapChainRebuild) {
      window->g_SwapChainRebuild = true;
    }

    if (window->g_SwapChainRebuild) {
      int width, height;
      SDL_GetWindowSize(window->GetWindowHandle(), &width, &height);
      if (width > 0 && height > 0) {
        ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
        ImGui_ImplVulkanH_CreateOrResizeWindow(
            g_Instance, g_PhysicalDevice, g_Device, &window->m_WinData,
            g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
        window->m_WinData.FrameIndex = 0;
        window->g_SwapChainRebuild = false;
      }
    }

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::SetCurrentContext(window->m_ImGuiContext);

    ImGui::NewFrame();

    window->LoadTheme();
    ImGui::PushFont(Application::GetFontList()["Default"]);

    app->RenderWindow(window.get());

    if (c_DockIsDragging && c_CurrentDragDropState) {
      SDL_GetGlobalMouseState(&c_CurrentDragDropState->mouseX,
                              &c_CurrentDragDropState->mouseY);

      float oldScale = ImGui::GetFont()->Scale;
      ImGui::GetFont()->Scale *= window->m_Specifications.FontGlobalScale;
      ImGui::PushFont(ImGui::GetFont());

      ImGui::PushStyleColor(ImGuiCol_WindowBg,
                            ImVec4(0.12f, 0.12f, 0.12f, 0.95f));
      ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.30f, 0.30f, 0.30f, 1.0f));

      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

      ImGui::SetNextWindowPos(
          ImVec2((float)c_CurrentDragDropState->mouseX + 15,
                 (float)c_CurrentDragDropState->mouseY + 15));

      ImGuiWindowFlags previewFlags =
          ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
          ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav |
          ImGuiWindowFlags_NoFocusOnAppearing |
          ImGuiWindowFlags_NoSavedSettings;

      if (ImGui::Begin("##DragPreviewOverlay", NULL, previewFlags)) {
        float iconSize = 30.0f;
        ImVec2 p = ImGui::GetCursorScreenPos();

        const std::string &windowId =
            c_CurrentDragDropState->LastDraggingAppWindowHost;
        const std::string &logoPath = GetLogoPathForAppWindow(windowId);
        const std::string &descStr = GetDescriptionForAppWindow(windowId);

        const char *titleText = windowId.c_str();
        const char *descText = descStr.c_str();
        bool hasDesc = (descText && descText[0] != '\0');

        if (!logoPath.empty()) {
          ImGui::SetCursorScreenPos(p);
          CherryGUI::Image(Cherry::GetTexture(logoPath),
                           ImVec2(iconSize, iconSize));
        } else {
          ImDrawList *drawList = ImGui::GetWindowDrawList();
          ImU32 colGray = IM_COL32(200, 200, 200, 255);
          ImU32 colAccent = IM_COL32(230, 40, 40, 255);

          drawList->AddRectFilled(p, ImVec2(p.x + iconSize, p.y + iconSize),
                                  IM_COL32(50, 50, 50, 255), 2.0f);

          float winX = p.x + 5.0f;
          float winY = p.y + 7.0f;
          float winW = iconSize - 10.0f;
          float winH = iconSize - 14.0f;

          drawList->AddRectFilled(ImVec2(winX, winY),
                                  ImVec2(winX + winW, winY + 4.0f), colAccent,
                                  1.0f);
          drawList->AddRect(ImVec2(winX, winY),
                            ImVec2(winX + winW, winY + winH), colGray, 1.0f);
        }

        float textStartX = p.x + iconSize + 10.0f;
        ImVec2 titleSize = ImGui::CalcTextSize(titleText);

        if (hasDesc) {
          ImGui::SetCursorScreenPos(ImVec2(textStartX, p.y - 1.0f));
          ImGui::TextUnformatted(titleText);

          ImGui::SetCursorScreenPos(
              ImVec2(textStartX, p.y + titleSize.y + 1.0f));
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
      ImGui::End();

      ImGui::PopStyleVar(3);
      ImGui::PopStyleColor(2);
      ImGui::GetFont()->Scale = oldScale;
      ImGui::PopFont();
    }
    ImGui::PopFont();
    window->UnloadTheme();

    ImGui_ImplVulkanH_Window *wd = &window->m_WinData;
    ImGuiIO &io = ImGui::GetIO();
    ImGui::Render();
    ImDrawData *main_draw_data = ImGui::GetDrawData();
    const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f ||
                                    main_draw_data->DisplaySize.y <= 0.0f);

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
    if (window->m_SelectedTheme != "undefined") {
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

void Application::CleanupEmptyWindows() {
  if (s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::DockingWindows ||
      s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::TabWidows ||
      s_Instance->m_DefaultSpecification.RenderMode ==
          WindowRenderingMethod::SimpleWindow) {
    std::vector<std::shared_ptr<Window>> to_remove;

    for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it) {
      int app_wins_inside = 0;

      for (const auto &app_win : m_AppWindows) {
        if (app_win->CheckWinParent((*it)->GetName())) {
          app_wins_inside++;
        }
      }

      if (app_wins_inside == 0) {
        to_remove.push_back(*it);
      }
    }
    for (const auto &win : to_remove) {
      if (win->m_ImGuiContext) {
        ImGui::SetCurrentContext(win->m_ImGuiContext);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(win->m_ImGuiContext);
        win->m_ImGuiContext = nullptr;
      }

      CleanupVulkanWindow(win.get());
      if (win->GetWindowHandle()) {
        SDL_DestroyWindow(win->GetWindowHandle());
      }
      m_Windows.erase(std::remove(m_Windows.begin(), m_Windows.end(), win),
                      m_Windows.end());
    }
  }
}

void Application::RebuildDockingLinks() {
  //
}

void Application::MultiThreadWindowRuntime() {
  const std::chrono::milliseconds frameDuration(
      1000 / s_Instance->m_DefaultSpecification.MaxFps);

  int frameCount = 0;
  auto startTime = std::chrono::steady_clock::now();
  auto frameStart = std::chrono::steady_clock::now();

  m_Running = true;
  this->BoostrappWindow();
  while (m_Running) {
    frameStart = std::chrono::steady_clock::now();
    frameCount++;

    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - startTime)
            .count();
    if (elapsed >= 1) {
      frameCount = 0;
      startTime = std::chrono::steady_clock::now();
    }

    auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - frameStart);
    if (frameTime < frameDuration) {
      std::this_thread::sleep_for(frameDuration - frameTime);
    }

    Identifier::ResetUniqueIndex(); // Reset anonymous components indexes
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
      if (window->m_Specifications.FavIconPath !=
          window->m_Specifications.LastFavIconPath) {
        window->SetFavIcon(window->m_Specifications.FavIconPath);
        window->m_Specifications.LastFavIconPath =
            window->m_Specifications.FavIconPath;
      }

      c_CurrentRenderedWindow = window;
      if (window->drag_dropstate->DockIsDragging) {
        c_DockIsDragging = true;
      }
    }

    for (auto &layer : m_LayerStack) {
      layer->OnUpdate(m_TimeStep);
    }

#ifdef CHERRY_DEBUG
    m_TriggerDevtools = false;
#endif // CHERRY_DEBUG

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      SDL_Window *focusedWindow = SDL_GetMouseFocus();
      Uint32 focusedWindowID =
          focusedWindow ? SDL_GetWindowID(focusedWindow) : 0;

      bool eventHandled = false;

      for (auto &window : m_Windows) {
        c_CurrentRenderedWindow = window;
        Uint32 windowID = SDL_GetWindowID(window->GetWindowHandle());

        if (focusedWindowID != 0 && windowID != focusedWindowID) {
          continue;
        }

        ImGui::SetCurrentContext(window->m_ImGuiContext);
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
          m_Running = false;
          eventHandled = true;
          break;
        }

        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == windowID) {
          if (window->m_Specifications.UsingCloseCallback) {
            if (window->m_Specifications.CloseCallback) {
              window->m_Specifications.CloseCallback();
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
    if (s_Instance->m_DefaultSpecification.RenderMode ==
            WindowRenderingMethod::DockingWindows ||
        Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode ==
            WindowRenderingMethod::TabWidows ||
        Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode ==
            WindowRenderingMethod::SimpleWindow) {
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

    if (s_Instance->m_DefaultSpecification.RenderMode ==
            WindowRenderingMethod::DockingWindows ||
        Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode ==
            WindowRenderingMethod::TabWidows ||
        Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode ==
            WindowRenderingMethod::SimpleWindow) {
      for (auto &appwin : s_Instance->m_AppWindows) {
        if (!AppWindowRedocked) {
          if (!appwin->m_AttachRequest.m_IsFinished) {
            std::string win = Application::Get().SpawnWindow(
                appwin->m_AttachRequest.m_Specification);
            appwin->SetParentWindow(win);
            appwin->m_AttachRequest.m_IsFinished = true;
          }
        }
      }
    }

    DragRendered = false;

    PresentAllWindows();

    float time = GetTime();
    m_FrameTime = time - m_LastFrameTime;
    m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
    m_LastFrameTime = time;

    c_MasterSwapChainRebuild = false;
    c_DockIsDragging = false;

    // Erase empty main windows
    CleanupEmptyWindows();

    for (auto &appwin : s_Instance->m_AppWindows) {
      appwin->m_WindowJustRebuilded = false;
    }

    AppWindowRedocked = false;
  }
}

void Application::SingleThreadRuntime() {
  const std::chrono::milliseconds frameDuration(
      1000 / s_Instance->m_DefaultSpecification.MaxFps);

  int frameCount = 0;
  auto startTime = std::chrono::steady_clock::now();
  auto frameStart = std::chrono::steady_clock::now();

  m_Running = true;
  this->BoostrappWindow();
  while (m_Running) {
    frameStart = std::chrono::steady_clock::now();
    frameCount++;

    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - startTime)
            .count();
    if (elapsed >= 1) {
      frameCount = 0;
      startTime = std::chrono::steady_clock::now();
    }

    auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - frameStart);
    if (frameTime < frameDuration) {
      std::this_thread::sleep_for(frameDuration - frameTime);
    }

    Identifier::ResetUniqueIndex(); // Reset anonymous components indexes

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
      if (window->m_Specifications.FavIconPath !=
          window->m_Specifications.LastFavIconPath) {
        window->SetFavIcon(window->m_Specifications.FavIconPath);
        window->m_Specifications.LastFavIconPath =
            window->m_Specifications.FavIconPath;
      }

      c_CurrentRenderedWindow = window;
      if (window->drag_dropstate->DockIsDragging) {
        c_DockIsDragging = true;
      }
    }

    for (auto &layer : m_LayerStack) {
      layer->OnUpdate(m_TimeStep);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      SDL_Window *focusedWindow = SDL_GetMouseFocus();
      Uint32 focusedWindowID =
          focusedWindow ? SDL_GetWindowID(focusedWindow) : 0;

      bool eventHandled = false;

      for (auto &window : m_Windows) {
        c_CurrentRenderedWindow = window;
        Uint32 windowID = SDL_GetWindowID(window->GetWindowHandle());

        if (focusedWindowID != 0 && windowID != focusedWindowID) {
          continue;
        }

        ImGui::SetCurrentContext(window->m_ImGuiContext);
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
          m_Running = false;
          eventHandled = true;
          break;
        }

        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == windowID) {
          if (window->m_Specifications.UsingCloseCallback) {
            if (window->m_Specifications.CloseCallback) {
              window->m_Specifications.CloseCallback();
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
            win = Application::Get().SpawnWindow(
                appwin->m_AttachRequest.m_Specification);
          }
          appwin->SetParentWindow(win);
          appwin->m_AttachRequest.m_IsFinished = true;
        }
      }
    }

    DragRendered = false;
    m_DevtoolsSwitchUsed = false;

    PresentAllWindows();

    float time = GetTime();
    m_FrameTime = time - m_LastFrameTime;
    m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
    m_LastFrameTime = time;

    c_MasterSwapChainRebuild = false;
    c_DockIsDragging = false;

    // Erase empty main windows
    CleanupEmptyWindows();

    // Pop Selected theme if defined
    if (m_SelectedTheme != "undefined") {
      PopTheme();
    }

    for (auto &appwin : s_Instance->m_AppWindows) {
      appwin->m_WindowJustRebuilded = false;
    }

    AppWindowRedocked = false;
  }
}

void Application::Close() { m_Running = false; }

bool Application::IsMaximized(const std::shared_ptr<Window> &win) const {
  Uint32 flags = SDL_GetWindowFlags(win->GetWindowHandle());
  return (flags & SDL_WINDOW_MAXIMIZED) != 0;
}

float Application::GetTime() { return (float)SDL_GetTicks() / 1000.0f; }

VkInstance Application::GetInstance() { return g_Instance; }

VkPhysicalDevice Application::GetPhysicalDevice() { return g_PhysicalDevice; }

VkDevice Application::GetDevice() { return g_Device; }

VkCommandBuffer Application::GetCommandBufferOfWin(const std::string &win_name,
                                                   bool begin) {
  ImGui_ImplVulkanH_Window *wd = nullptr;

  for (auto layer : app->m_LayerStack) {
    if (layer->ParentWindow == win_name) {
      for (auto win : app->m_Windows) {
        if (win->GetName() == layer->ParentWindow) {
          wd = &win->m_WinData;

          VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

          VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
          cmdBufAllocateInfo.sType =
              VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
          cmdBufAllocateInfo.commandPool = command_pool;
          cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
          cmdBufAllocateInfo.commandBufferCount = 1;

          VkCommandBuffer &command_buffer =
              win->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
          auto err = vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo,
                                              &command_buffer);

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

VkCommandBuffer
Application::GetCommandBuffer(bool begin, const std::shared_ptr<Window> &win) {
  ImGui_ImplVulkanH_Window *wd = &win->m_WinData;

  VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;

  VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
  cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmdBufAllocateInfo.commandPool = command_pool;
  cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmdBufAllocateInfo.commandBufferCount = 1;

  VkCommandBuffer &command_buffer =
      win->s_AllocatedCommandBuffers[wd->FrameIndex].emplace_back();
  auto err =
      vkAllocateCommandBuffers(g_Device, &cmdBufAllocateInfo, &command_buffer);

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

void Application::SubmitResourceFree(std::function<void()> &&func,
                                     const std::string &winname) {
  for (auto win : app->m_Windows) {
    if (win->GetName() == winname) {
      win->s_ResourceFreeQueue[win->s_CurrentFrameIndex].emplace_back(func);
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

void Application::HandleResizing(Window *window) {}

void Application::HandleDockingModeRendering(Window *window) {
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGuiID dockspaceID = ImGui::GetID("MainDockspace");

  float oldsize = ImGui::GetFont()->Scale;
  ImGui::GetFont()->Scale *= window->m_Specifications.FontGlobalScale;
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

  if (c_DockIsDragging) {
    for (auto &appwin : m_AppWindows) {
      if (c_CurrentDragDropState->LastDraggingAppWindowHost ==
          appwin->m_IdName) {
        if (!appwin->m_HaveParentAppWindow) {
          Window::ShowDockingPreview(dockspaceID, window,
                                     c_CurrentDragDropState, appwin);
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
            c_CurrentRenderedAppWindow = appwindow;
            appwindow->CtxRender(&m_RedockRequests, window->GetName());
            context_loaded = true;
          }
        }
      }

      if (appwindow->CheckWinParent(window->GetName()) &&
          !appwindow->m_HaveParentAppWindow) {
        c_CurrentRenderedAppWindow = appwindow;
        appwindow->CtxRender(&m_RedockRequests, window->GetName());
        context_loaded = true;
      }
    }
  }
}

void Application::HandleTabsModeRendering(Window *window) {
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGuiID dockspaceID = ImGui::GetID("MainDockspace");

  float oldsize = ImGui::GetFont()->Scale;
  ImGui::GetFont()->Scale *= window->m_Specifications.FontGlobalScale;
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
            c_CurrentRenderedAppWindow = appwindow;
            appwindow->CtxRender(&m_RedockRequests, window->GetName());
            context_loaded = true;
          }
        }
      }

      if (appwindow->CheckWinParent(window->GetName()) &&
          !appwindow->m_HaveParentAppWindow) {
        c_CurrentRenderedAppWindow = appwindow;
        appwindow->CtxRender(&m_RedockRequests, window->GetName());
        context_loaded = true;
      }
    }
  }
}

void Application::HandleSimpleRendering(Window *window) {
  float oldsize = ImGui::GetFont()->Scale;
  ImGui::GetFont()->Scale *= window->m_Specifications.FontGlobalScale;
  ImGui::PushFont(ImGui::GetFont());
  if (m_MainRenderCallback) {
    m_MainRenderCallback();
  }
  ImGui::GetFont()->Scale = oldsize;
  ImGui::PopFont();
}

void Application::HandleSimpleWindowRendering(Window *window) {
  bool finded = false;
  for (auto &appwin : Application::Get().m_AppWindows) {
    if (appwin) {
      if (appwin->m_Name == Application::GetCurrentRenderedWindow()
                                ->m_Specifications.UniqueAppWindowName) {
        c_CurrentRenderedAppWindow = appwin;
        appwin->CtxRender(nullptr, window->GetName());
        finded = true;
      }
    }
  }

  if (!finded) {
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f),
                       "UIKit Error : You need to specify the "
                       "\"UniqueAppWindowName\" in spec !");
  }
}

/*std::string Application::GetThemeProperty(const std::string &key) {
  auto app_window_theme_prop = CherryAppWindow.GetThemeProperty(key);
  if (app_window_theme_prop == "undefined") {

    auto window_theme_prop = CherryWindow.GetThemeProperty(key);
    if (window_theme_prop == "undefined") {

      if (!m_SelectedTheme) {
        return;
      } else {
        return m_SelectedTheme->GetProperty(key);
      }
    }
  }
}*/

void Application::HandleLayerStackUpdate(Window *window) {
  for (auto &layer : m_LayerStack) {
    if (!layer->initialized) {
      layer->ParentWindow = window->GetName();
      layer->initialized = true;
    }

    // layer->m_WindowControlCallbalck = [window, io](ImGuiWindow *win) {
    //
    // };

    if (layer->ParentWindow == window->GetName()) {
      layer->OnUIRender();
    }
  }
}

void Application::PrepareViewport(Window *window) {
  ImGuiViewport *viewport = ImGui::GetMainViewport();

  // N'utilisez plus window->WinID s'il contient un gros nombre (hash)
  // Laissez ImGui gérer ses propres viewports internes pour l'instant

  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);
}

ImDrawData *Application::RenderWindow(Window *window) {
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

  PrepareViewport(window);

  float minWidth = window->m_Specifications.MinWidth;
  float minHeight = window->m_Specifications.MinHeight;
  ImGui::SetNextWindowSizeConstraints(ImVec2(minWidth, minHeight),
                                      ImVec2(FLT_MAX, FLT_MAX));

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  if (Application::GetCurrentRenderedWindow()) {
    if (!Application::GetCurrentRenderedWindow()
             ->m_Specifications.WindowResizeable) {
      window_flags |= ImGuiWindowFlags_NoResize;
    }
  }

  if (!window->m_Specifications.CustomTitlebar && m_MenubarCallback)
    window_flags |= ImGuiWindowFlags_MenuBar;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
  ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

  std::string label = "DockSpaceWindow." + window->GetName();
  ImGui::SetNextWindowDockID(0);
  ImGui::Begin(label.c_str(), nullptr, window_flags);
  window->m_ImGuiWindow = ImGui::GetCurrentWindow();

  ImVec2 newSize = ImGui::GetWindowSize();

  SDL_Window *sdlWindow = window->GetWindowHandle();
  int sdlWidth, sdlHeight;
  SDL_GetWindowSize(sdlWindow, &sdlWidth, &sdlHeight);

  if (newSize.x < minWidth)
    newSize.x = minWidth;
  if (newSize.y < minHeight)
    newSize.y = minHeight;

  ImVec2 deltaSize = ImVec2(newSize.x - ImGui::GetWindowSize().x,
                            newSize.y - ImGui::GetWindowSize().y);

  ImVec2 windowPos = ImGui::GetWindowPos();

  window->m_Resizing = false;
  if (newSize.x != sdlWidth || newSize.y != sdlHeight) {
    window->m_Resizing = true;

    if (ImGui::IsMouseHoveringRect(
            windowPos, ImVec2(windowPos.x + sdlWidth, windowPos.y))) {
      windowPos.y -= deltaSize.y;
    }

    if (ImGui::IsMouseHoveringRect(
            windowPos, ImVec2(windowPos.x, windowPos.y + sdlHeight))) {
      windowPos.x -= deltaSize.x;
    }

    SDL_SetWindowSize(sdlWindow, static_cast<int>(newSize.x),
                      static_cast<int>(newSize.y));
    SDL_SetWindowPosition(sdlWindow, static_cast<int>(windowPos.x),
                          static_cast<int>(windowPos.y));
  }

  ImGuiWindow *win = ImGui::GetCurrentWindow();

  ImGui::PopStyleColor();
  ImGui::PopStyleVar(4);

  if (window->m_Specifications.CustomTitlebar &&
      !window->m_Specifications.DisableTitleBar) {
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

  switch (window->m_Specifications.RenderMode) {
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
  if (Application::DebugToolState().load() &&
      window->GetName() != "Cherry devtools") {
    const float PAD = 10.0f;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + PAD, work_pos.y + PAD),
                            ImGuiCond_Always);

    ImGui::SetNextWindowBgAlpha(0.55f);
    ImGuiWindowFlags overlay_flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoInputs;

    if (ImGui::Begin("##DebugOverlay", nullptr, overlay_flags)) {
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "CHERRY ENGINE DEBUG");
      ImGui::Separator();

      ImGui::Text("Application: %s", window->GetName().c_str());
      ImGui::Text("FPS: %.1f (%.3f ms/frame)", ImGui::GetIO().Framerate,
                  1000.0f / ImGui::GetIO().Framerate);

      int w, h;
      SDL_GetWindowSize(window->GetWindowHandle(), &w, &h);
      ImGui::Text("Resolution: %dx%d", w, h);

      if (ImGui::IsMousePosValid())
        ImGui::Text("Mouse: %.1f, %.1f", ImGui::GetIO().MousePos.x,
                    ImGui::GetIO().MousePos.y);
      else
        ImGui::Text("Mouse: Off-screen");
    }
    ImGui::End();
  }
#endif
  ImGui::End();

  return &window->DrawData;
}

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

void Application::SetWindowSaveDataFile(const std::string &input_path,
                                        const bool &relative) {
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
      std::cout << "The specified path is not a valid file: " << path
                << std::endl;
    }
  }

  std::ofstream outputFile(path);
  if (!outputFile) {
    std::cout << "Unable to create or open file: " << path << std::endl;
  }

  nlohmann::json jsonData = {{"save", true}};

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

ComponentsPool *Application::GetComponentPool() {
  if (!m_ComponentPoolStack.empty()) {
    return m_ComponentPoolStack.back();
  } else {
    return &m_ApplicationComponentPool;
  }
}

void Application::SetDataOnGroup(const std::string &group_name,
                                 const std::string &key,
                                 const std::string &value) {
  for (auto component : CherryApp.GetComponentPool()->IdentifiedComponents) {
    if (component->GetIdentifier().component_group() == group_name) {
      component->SetData(key, value);
    }
  }
}
void Application::SetPropertyOnGroup(const std::string &group_name,
                                     const std::string &key,
                                     const std::string &value) {
  for (auto component : CherryApp.GetComponentPool()->IdentifiedComponents) {
    if (component->GetIdentifier().component_group() == group_name) {
      component->SetProperty(key, value);
    }
  }
}

// Audio service if CHERRY_ENABLE_AUDIO
#ifdef CHERRY_ENABLE_AUDIO
void Application::StartAudioService() {
  ma_result result;

  result = ma_engine_init(NULL, &this->m_AudioEngine);
  if (result != MA_SUCCESS) {
    std::cout << "Error: Failed to start audio service" << std::endl;
  }
}

void Application::StopAudioService() { ma_engine_uninit(&this->m_AudioEngine); }

void Application::PlaySound(const std::string &wav_file_path) {
  ma_engine_play_sound(&this->m_AudioEngine, wav_file_path.c_str(), NULL);
}

void Application::CreateChannel(const std::string &name) {
  m_Channels[name] = std::make_shared<AudioChannel>(&m_AudioEngine, name);
}

std::shared_ptr<AudioChannel> Application::GetChannel(const std::string &name) {
  if (m_Channels.find(name) != m_Channels.end())
    return m_Channels[name];
  return nullptr;
}

std::map<std::string, std::shared_ptr<AudioChannel>> &
Application::GetChannels() {
  return m_Channels;
}
#else
void Application::StartAudioService() {
  //
}

void Application::StopAudioService() {
  //
}

void Application::PlaySound(const std::string &wav_file_path) {
  std::cout << "To use audio, please enable CHERRY_ENABLE_AUDIO (to 1) on "
               "options.hpp of the Cherry framework"
            << std::endl;
}
#endif

bool Application::IsKeyPressed(CherryKey key) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  const auto &keyMap = GetKeyMap();
  auto it = keyMap.find(key);
  return it != keyMap.end() && state[it->second];
}

void Application::PushCurrentComponent(
    const std::shared_ptr<Component> &component) {
  m_PushedCurrentComponent.push_back(component);
}

void Application::PopCurrentComponent(int pop_number) {
  if (m_PushedCurrentComponent.empty())
    return;

  if (pop_number <= 0 ||
      pop_number >= static_cast<int>(m_PushedCurrentComponent.size())) {
    m_PushedCurrentComponent.clear();
  } else {
    m_PushedCurrentComponent.erase(m_PushedCurrentComponent.end() - pop_number,
                                   m_PushedCurrentComponent.end());
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

void Application::RemoveTheme() { m_SelectedTheme = "undefined"; }

void Application::PushComponentGroup(const std::string &groupname) {
  m_PushedComponentGroups.push_back(groupname);
}

void Application::PopComponentGroup(int pop_number) {
  if (m_PushedComponentGroups.empty())
    return;

  if (pop_number <= 0 ||
      pop_number >= static_cast<int>(m_PushedComponentGroups.size())) {
    m_PushedComponentGroups.clear();
  } else {
    m_PushedComponentGroups.erase(m_PushedComponentGroups.end() - pop_number,
                                  m_PushedComponentGroups.end());
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
      std::remove_if(components.begin(), components.end(),
                     [](const std::shared_ptr<Component> &component) {
                       return !component->m_IsComponentRendered;
                     }),
      components.end());

  anonymous_components.erase(
      std::remove_if(anonymous_components.begin(), anonymous_components.end(),
                     [](const std::shared_ptr<Component> &component) {
                       return !component->m_IsComponentRendered;
                     }),
      anonymous_components.end());
}

void Application::RefreshComponentsRenderFlags(ComponentsPool *pool) {
  if (!pool) {
    pool = &m_ApplicationComponentPool;
  }

  for (auto &component : pool->IdentifiedComponents) {
    component->m_IsComponentRendered = false;
  }

  for (auto &component : pool->AnonymousComponents) {
    component->m_IsComponentRendered = false;
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

  auto it = std::remove_if(components.begin(), components.end(),
                           [&id](const std::shared_ptr<Component> &component) {
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

std::string Application::GetThemeProperty(const std::string &theme_name,
                                          const std::string &key) {
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

std::string Application::CookPath(std::string_view input_path) {
  static const std::string root_path = []() {
    std::string path;
#ifdef _WIN32
    char result[MAX_PATH];
    if (GetModuleFileNameA(NULL, result, MAX_PATH))
      path = std::filesystem::path(result).parent_path().string();
    else
      std::cerr << "Failed to get root path" << std::endl;
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, sizeof(result) - 1);
    if (count != -1) {
      result[count] = '\0';
      path = std::filesystem::path(result).parent_path().string();
    } else {
      std::cerr << "Failed to get root path" << std::endl;
    }
#endif
    return path;
  }();

  return (input_path.empty() || input_path[0] == '/')
             ? std::string(input_path)
             : root_path + "/" + std::string(input_path);
}

void Application::PushLayer(const std::shared_ptr<Layer> &layer) {
  m_LayerStack.emplace_back(layer);
  layer->OnAttach();
}

std::string Application::GetComponentData(const Identifier &id,
                                          const std::string &topic) {
  if (id.component_array_ptr() != nullptr) {
    for (const auto &component :
         id.component_array_ptr()->IdentifiedComponents) {
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

void Application::AddFont(const std::string &name,
                          const std::string &ttf_file_path, const float &size) {
  m_CustomFonts.push_back({name, {ttf_file_path, size}});
};

void Application::SetFavIconPath(const std::string &icon_path) {
  m_FavIconPath = icon_path;
}

void Application::AddLocale(const std::string &locale_name,
                            const std::string &data_path) {
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

void Application::PushPermanentProperty(const std::string &property,
                                        const std::string &value) {
  m_PermanentProperties.push_back({property, value});
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

void Application::ClearOneTimeProperties() { m_OnTimeProperties.clear(); }

void Application::RemoveOneTimeProperty(const std::string &key) {
  m_OnTimeProperties.erase(key);
}

const std::unordered_map<std::string, std::string> &
Application::GetOneTimeProperties() {
  return m_OnTimeProperties;
}

void Application::AddOneTimeProperty(const std::string &property,
                                     const std::string &value) {
  m_OnTimeProperties[property] = value;
}

void Application::PushParentComponent(
    const std::shared_ptr<Component> &component) {
  m_ParentComponentsStack.push_back(component);
}

void Application::PopParentComponent() {
  if (!m_ParentComponentsStack.empty()) {
    m_ParentComponentsStack.pop_back();
  }
}

std::shared_ptr<Component> Application::GetParent(int parent_number) {
  if (parent_number < 0 ||
      parent_number >= static_cast<int>(m_ParentComponentsStack.size())) {
    return nullptr;
  }
  return m_ParentComponentsStack[m_ParentComponentsStack.size() - 1 -
                                 parent_number];
}

std::string Application::GetLocale(const std::string &locale_type) {
  if (m_SelectedLocale.empty() ||
      m_Locales.find(m_SelectedLocale) == m_Locales.end()) {
    return "locale_undefined";
  }

  const nlohmann::json &current_locale = m_Locales[m_SelectedLocale];

  for (const auto &item : current_locale["locales"]) {
    if (item.contains(locale_type)) {
      return item[locale_type].get<std::string>();
    }
  }

  // Get the default traductor if wanted locales not finded.
  if (m_DefaultLocale.empty() ||
      m_Locales.find(m_DefaultLocale) == m_Locales.end()) {
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

Component &Application::GetAnonymousComponent(const Identifier &identifier) {
  if (identifier.component_array_ptr() != nullptr) {
    for (const auto &existing_component :
         identifier.component_array_ptr()->IdentifiedComponents) {
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
  auto &components =
      identifier.component_array_ptr() != nullptr
          ? identifier.component_array_ptr()->IdentifiedComponents
          : Application::Get().GetComponentPool()->IdentifiedComponents;

  for (const auto &existing_component : components) {
    if (existing_component &&
        existing_component->GetIdentifier() == identifier) {
      return *existing_component;
    }
  }

  return s_EmptyComponent;
}

std::shared_ptr<Component>
Application::GetComponentPtr(const Identifier &identifier) {
  auto &components =
      identifier.component_array_ptr() != nullptr
          ? identifier.component_array_ptr()->IdentifiedComponents
          : Application::Get().GetComponentPool()->IdentifiedComponents;

  for (const auto &existing_component : components) {
    if (existing_component &&
        existing_component->GetIdentifier() == identifier) {
      return existing_component;
    }
  }

  return nullptr;
}

void Application::SetDescriptionForAppWindow(const std::string &windowId,
                                             const std::string &description) {
  m_AppWindowRegistry[windowId].Description = description;
}

void Application::SetLogoPathForAppWindow(const std::string &windowId,
                                          const std::string &path) {
  m_AppWindowRegistry[windowId].LogoPath = path;
}

const std::string &
Application::GetDescriptionForAppWindow(const std::string &windowId) {
  auto it = m_AppWindowRegistry.find(windowId);
  if (it != m_AppWindowRegistry.end()) {
    return it->second.Description;
  }
  static const std::string defaultDesc = "";
  return defaultDesc;
}

const std::string &
Application::GetLogoPathForAppWindow(const std::string &windowId) {
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
  for (auto &appwin : s_Instance->m_AppWindows) {
    if (appwin->m_IdName == win_name) {
      return appwin;
    }
  }
  return nullptr;
}

std::shared_ptr<Window> GetWindowByName(const std::string &win_name) {
  for (auto &win : s_Instance->m_Windows) {
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
    m_AppWindows.erase(
        std::remove(m_AppWindows.begin(), m_AppWindows.end(), win),
        m_AppWindows.end());
  }
}

void Application::EnableNoAppWindowSafety() {
  m_NoAppWindowSafetyEnabled = true;
  std::shared_ptr<AppWindow> app_window = std::make_shared<AppWindow>(
      "cherryinternal_no_windows", "cherryinternal_no_windows");
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
  Cherry::DeleteAppWindow(
      Cherry::GetAppWindowByName("cherryinternal_no_windows"));
  m_NoAppWindowSafetyEnabled = false;
}

void DeleteAppWindow(const std::shared_ptr<AppWindow> &win) {
  Application::Get().DeleteAppWindow(win);
}

ImTextureID GetTexture(const std::string &path) {
  return Application::Get().GetCurrentRenderedWindow()->get_texture(path);
}

ImVec2 GetTextureSize(const std::string &path) {
  return Application::Get().GetCurrentRenderedWindow()->get_texture_size(path);
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
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  std::ofstream *ofs = static_cast<std::ofstream *>(userp);
  size_t totalSize = size * nmemb;
  ofs->write(static_cast<char *>(contents), totalSize);
  return totalSize;
}

std::string SanitizeUrl(const std::string &url) {
  std::string sanitized = url;
  for (char &c : sanitized) {
    if (c == '/') {
      c = '-';
    }
  }
  return sanitized;
}

std::string GetTemporaryDirectory() {
#ifdef _WIN32
  char tempPath[MAX_PATH];
  if (GetTempPathA(MAX_PATH, tempPath)) {
    return std::string(tempPath);
  } else {
    throw std::runtime_error("Failed to get temporary directory path");
  }
#else
  const char *tmpDir = getenv("TMPDIR");
  if (tmpDir) {
    return std::string(tmpDir);
  }
  return "/tmp";
#endif
}

std::string GetHttpPath(const std::string &url) {
  bool use_cache = true;
  std::string cache_path = GetTemporaryDirectory() + "/" +
                           Application::Get().GetHttpCacheFolderName() + "/";

  if (!fs::exists(cache_path)) {
    fs::create_directories(cache_path);
  }

  std::string filename = SanitizeUrl(url);
  std::string file_path = cache_path + filename;

  if (use_cache && fs::exists(file_path)) {
    return file_path;
  }

  naettReq *req = nullptr;

  const char *URL = url.c_str();
  req =
      naettRequest_va(URL, 2, naettMethod("GET"), naettHeader("accept", "*/*"));

  naettRes *res = naettMake(req);

  while (!naettComplete(res)) {
#ifdef _WIN32
    Sleep(100);
#else
    usleep(100 * 1000);
#endif
  }

  int status = naettGetStatus(res);
  if (status != 200) {
    std::cerr << "HTTP request failed: " << status << " - ";
    int len = 0;
    const void *err_body = naettGetBody(res, &len);
    if (err_body)
      std::cerr.write(static_cast<const char *>(err_body), len);
    std::cerr << std::endl;
    naettClose(res);
    naettFree(req);
    return "";
  }

  int bodyLength = 0;
  const void *body = naettGetBody(res, &bodyLength);

  std::ofstream ofs(file_path, std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "Failed to open file for writing: " << file_path << std::endl;
    naettClose(res);
    naettFree(req);
    return "";
  }

  ofs.write(static_cast<const char *>(body), bodyLength);
  ofs.close();

  naettClose(res);
  naettFree(req);

  return file_path;
}

#endif // CHERRY_ENABLE_NET

std::string GetLocale(const std::string &topic) {
  return Application::Get().GetLocale(topic);
}

bool IsReady() {
  if (&Application::Get() == 0) {
    return false;
  }
  return true;
}

std::string GetData(const Identifier &id, const std::string topic) {
  return CherryApp.GetComponentData(id, topic);
}

void PushPermanentProperty(const std::string &property,
                           const std::string &value) {
  Application::Get().PushPermanentProperty(property, value);
}

void PopPermanentProperty(int number_of_pops) {
  Application::Get().PopPermanentProperty(number_of_pops);
}

void SetNextComponentProperty(const std::string &property,
                              const std::string &value) {
  Application::Get().AddOneTimeProperty(property, value);
}

void PushParentComponent(const std::shared_ptr<Component> &component) {
  Application::Get().PushParentComponent(component);
}

void PopParentComponent() { Application::Get().PopParentComponent(); }

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

void PopFont() { ImGui::PopFont(); }

} // namespace Cherry
