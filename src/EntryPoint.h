#pragma once

#include "./Application.h"

#ifndef UIKIT_ENTRYPOINT_H
#define UIKIT_ENTRYPOINT_H

namespace UIKit {
    extern UIKit::Application* CreateApplication(int argc, char** argv);
}

bool g_ApplicationRunning = true;
namespace UIKit {

	int Main(int argc, char** argv)
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

namespace VortexMaker{
static int VortexInterface(int argc, char** argv)
{
	return UIKit::Main(argc, argv);
}
}
#endif // defined(WL_PLATFORM_WINDOWS) && defined(WL_DIST)
#endif // UIKIT_ENTRYPOINT_H