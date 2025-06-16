#pragma once

#include "../../base.hpp"

#ifdef CHERRY_CEF

#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_task.h"
#include "include/wrapper/cef_helpers.h"

#ifndef CHERRY_CEF_H
#define CHERRY_CEF_H

// CEF GLoba Variable
// create browser-window

namespace Cherry {

int ImGui_ImplSDL2_CefInit(int argc, char **argv);

// for manual render handler
class BrowserClient : public CefClient,
                      public CefRenderHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler {
public:
  BrowserClient(int w, int h) : width(w), height(h) {}

  CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }

  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

  CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

  int width;
  int height;
  void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading,
                            bool canGoBack, bool canGoForward) override {
    std::cout << "🔄 OnLoadingStateChange: isLoading=" << isLoading
              << std::endl;
    if (!isLoading) {
      std::cout << "✅ Page fully loaded!" << std::endl;
    }
  }

  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
    CEF_REQUIRE_UI_THREAD();
    browser_id = browser->GetIdentifier();
    std::cout << "✅ OnAfterCreated: Browser ID " << browser_id << std::endl;
  }

  bool DoClose(CefRefPtr<CefBrowser> browser) override {
    // Must be executed on the UI thread.
    CEF_REQUIRE_UI_THREAD();

    // Closing the main window requires special handling. See the DoClose()
    // documentation in the CEF header for a detailed description of this
    // process.
    if (browser->GetIdentifier() == browser_id) {
      // Set a flag to indicate that the window close should be allowed.
      closing = true;
    }

    // Allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    return false;
  }

  void OnBeforeClose(CefRefPtr<CefBrowser> browser) {}

  void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                 int httpStatusCode) {
    printf("OnLoadEnd(%d)\n", httpStatusCode);
    loaded = true;
  }

  void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                   ErrorCode errorCode, const CefString &errorText,
                   const CefString &failedUrl) {
    printf("OnLoadError: %s (Error code: %d)\n", errorText.ToString().c_str(),
           errorCode);
  }

  void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
    printf("OnLoadStart()\n");
  }

  bool closeAllowed() const { return closing; }

  bool isLoaded() const { return loaded; }

  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect);

  void OnPaint(CefRefPtr<CefBrowser> browser,
               CefRenderHandler::PaintElementType type,
               const CefRenderHandler::RectList &dirtyRects, const void *buffer,
               int width, int height);

  /*void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const
  RectList &dirtyRects, const void *buffer, int width, int height) override
  {
          std::cout << "OnPaint Buffer received from CEF: "<< buffer <<
  std::endl;

  }*/

  void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                          const RectList &dirtyRects,
                          const CefAcceleratedPaintInfo &info) override {
    std::cout << "OnAcceleratedPaint Buffer received from CEF: "
              << info.shared_texture_handle << std::endl;
    if (type == PET_VIEW) {
      //	UpdateCefTexture(info.shared_texture_handle, 500, 500);
    }
  }
  void resize(int w, int h);
  void render();

  std::mutex cefTextureMutex;
  std::atomic<bool> runningCEF = true;

private:
  int browser_id;
  bool closing = false;
  bool loaded = false;

  IMPLEMENT_REFCOUNTING(BrowserClient);
};

class MyCefApp : public CefApp, public CefRenderProcessHandler {
public:
  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
    return this;
  }

  IMPLEMENT_REFCOUNTING(MyCefApp);
};

class CherryBrowser {
public:
  CherryBrowser(const std::string &id, int width, int height,
                const std::string &url)
      : m_Id(id), m_Width(width), m_Height(height), m_Url(url) {
    InitCEF();
  };

  std::chrono::steady_clock::time_point lastWakeTime;

  void WakeUp();
  bool IsReady() const;
  void RunCEFLoop();
  void ChangeBrowserURL(char *URL);
  void ShowBrowserWindow(bool *p_open, ImTextureID tex_id);
  void InitCEF();
  void OnCEFFrame();
  void ProcessSDLEvent(const SDL_Event &event);
  ImTextureID GetCefTexture();
  void ShutdownCEF();
  void CreateCefImage(int width, int height);
  void UpdateCefTexture(const void *buffer, int width, int height);
  void UpdateBrowserMouse(ImVec2 windowPos, ImVec2 cursorPos);
  void CreateVulkanImage(VkDevice device, uint32_t width, uint32_t height,
                         VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties, VkImage &image,
                         VkDeviceMemory &imageMemory);
  void CreateVulkanImageView(VkDevice device, VkImage image, VkFormat format,
                             VkImageView &imageView);
  void VulkanCreateBuffer(VkDevice device, VkDeviceSize size,
                          VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties, VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);
  void VulkanTransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image,
                                   VkImageLayout oldLayout,
                                   VkImageLayout newLayout);
  void VulkanCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer,
                               VkImage image, uint32_t width, uint32_t height);
  void VulkanSubmitCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);
  int GetCefModifiersFromImGui(const ImGuiIO &io);
  void ForwardImGuiInputsToCEF(CefRefPtr<CefBrowser> browser, const ImGuiIO &io,
                               int browserWidth, int browserHeight);
  int currentTextureWidth = 0;
  int currentTextureHeight = 0;

  int m_Width;
  int m_Height;
  bool m_ShowWindow;
  bool m_NeedResize = false;
  std::string m_Url;
  std::string m_Id;
  CefRefPtr<CefBrowser> m_Browser;
  CefRefPtr<BrowserClient> m_BrowserClient;
  VkImage vulkanImage;
  ImTextureID cefTextureId = nullptr;
  std::thread cefThread;
  VkDeviceMemory textureMemory;
  VkImage textureImage;
  VkImageView textureImageView;
  VkSampler textureSampler;
  ImVec2 g_windowPos, g_cursorPos;
};

/*CefBrowserHost::MouseButtonType translateMouseButton(SDL_MouseButtonEvent
const &e)
{
        CefBrowserHost::MouseButtonType result;
        switch (e.button)
        {
        case SDL_BUTTON_LEFT:
        case SDL_BUTTON_X1:
                result = MBT_LEFT;
                break;

        case SDL_BUTTON_MIDDLE:
                result = MBT_MIDDLE;
                break;

        case SDL_BUTTON_RIGHT:
        case SDL_BUTTON_X2:
                result = MBT_RIGHT;
                break;
        }
        return result;
}*/
extern std::vector<std::shared_ptr<CherryBrowser>> registered_browsers;
void BrowserView(const std::string &id, int width, int height,
                 const std::string &url = "");
int HandleCEF();

} // namespace Cherry

#endif // CHERRY_CEF_H
#endif // CHERRY_CEF