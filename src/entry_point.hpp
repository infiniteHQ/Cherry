#pragma once

#include "./application.hpp"

#ifndef UIKIT_ENTRYPOINT_H
#define UIKIT_ENTRYPOINT_H

namespace UIKit {
    extern UIKit::Application* CreateApplication(int argc, char** argv);
}

inline bool g_ApplicationRunning = true;
namespace UIKit {

	// For multiple app usage
	static int ThirdMain(int argc, char** argv, UIKit::Application*(*create_app)(int argc, char** argv))
	{
		while (g_ApplicationRunning)
		{
			UIKit::Application* app = create_app(argc, argv);
			app->Run();
			delete app;
		}

		return 0;
	}

	// For single app usage
	static int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			UIKit::Application* app = UIKit::CreateApplication(argc, argv);
			app->Run();
			delete app;
		}

		return 0;
	}

}

#if defined(WL_PLATFORM_WINDOWS) && !defined(WL_HEADLESS) && defined(WL_DIST)

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return UIKit::Main(__argc, __argv, ctx);
}

#else

#endif // defined(WL_PLATFORM_WINDOWS) && defined(WL_DIST)
#endif // UIKIT_ENTRYPOINT_H