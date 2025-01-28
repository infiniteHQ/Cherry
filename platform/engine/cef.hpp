#pragma once

#include "base.hpp"

#ifdef CHERRY_CEF
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/cef_task.h"
#include "include/wrapper/cef_helpers.h"

#ifndef CHERRY_CEF_H
#define CHERRY_CEF_H

// CEF GLoba Variable
// create browser-window
static VkImage vulkanImage;
static ImTextureID cefTextureId = nullptr;

static VkImage cefImage;
static VkImageView cefImageView;
static VkDeviceMemory cefImageMemory;
static ImVec2 g_windowPos, g_cursorPos;

namespace Cherry
{

void CreateCefImage(int width, int height);
void UpdateCefTexture(const void* buffer, int width, int height);
	void UpdateBrowserMouse(ImVec2 windowPos, ImVec2 cursorPos);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CreateVulkanImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
						   VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
	void CreateVulkanImageView(VkDevice device, VkImage image, VkFormat format, VkImageView &imageView);
	void VulkanCreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
	void VulkanTransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
	void VulkanCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void VulkanSubmitCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);
	ImTextureID ImGui_ImplSDL2_GetCefTexture();
int ImGui_ImplSDL2_CefInit(int argc, char** argv);

	class RenderHandler : public CefRenderHandler
	{
	public:
		RenderHandler(int w, int h) : width(w),
									  height(h)
		{
		}

		~RenderHandler()
		{
		}

		void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect);

		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height);
		void resize(int w, int h);
		void render();

	public:
		int width;
		int height;

		IMPLEMENT_REFCOUNTING(RenderHandler);
	};

	// for manual render handler
	class BrowserClient : public CefClient,
						  public CefLifeSpanHandler,
						  public CefLoadHandler
	{
	public:
		BrowserClient(CefRefPtr<CefRenderHandler> ptr) : handler(ptr)
		{
		}

		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler()
		{
			return this;
		}

		virtual CefRefPtr<CefLoadHandler> GetLoadHandler()
		{
			return this;
		}

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler()
		{
			return handler;
		}

		// CefLifeSpanHandler methods.
		void OnAfterCreated(CefRefPtr<CefBrowser> browser)
		{
			// Must be executed on the UI thread.
			CEF_REQUIRE_UI_THREAD();

			browser_id = browser->GetIdentifier();
		}

		bool DoClose(CefRefPtr<CefBrowser> browser)
		{
			// Must be executed on the UI thread.
			CEF_REQUIRE_UI_THREAD();

			// Closing the main window requires special handling. See the DoClose()
			// documentation in the CEF header for a detailed description of this
			// process.
			if (browser->GetIdentifier() == browser_id)
			{
				// Set a flag to indicate that the window close should be allowed.
				closing = true;
			}

			// Allow the close. For windowed browsers this will result in the OS close
			// event being sent.
			return false;
		}

		void OnBeforeClose(CefRefPtr<CefBrowser> browser)
		{
		}

		void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
		{
			printf("OnLoadEnd(%d)\n", httpStatusCode);
			loaded = true;
		}

		bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString &failedUrl, CefString &errorText)
		{
			printf("OnLoadError(%d)\n", errorCode);
			loaded = true;
		}

		void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
		{
			printf("OnLoadingStateChange()\n");
		}

		void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
		{
			printf("OnLoadStart()\n");
		}

		bool closeAllowed() const
		{
			return closing;
		}

		bool isLoaded() const
		{
			return loaded;
		}

	private:
		int browser_id;
		bool closing = false;
		bool loaded = false;
		CefRefPtr<CefRenderHandler> handler;

		IMPLEMENT_REFCOUNTING(BrowserClient);
	};

	static CefRefPtr<CefBrowser> browser;
	static CefRefPtr<BrowserClient> browserClient;
	static CefRefPtr<RenderHandler> renderHandler;

	/*CefBrowserHost::MouseButtonType translateMouseButton(SDL_MouseButtonEvent const &e)
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

	void ChangeBrowserURL(char *URL);
	void ShowBrowserWindow(bool *p_open, ImTextureID tex_id);
	void InitCEF(int width, int height);
	void OnCEFFrame();

}

#endif // CHERRY_CEF_H
#endif // CHERRY_CEF