#pragma once

#include "../../../../main/include/vortex.h"
#include "../../../../main/include/vortex_internals.h"

namespace Walnut {
    extern Walnut::Application* CreateApplication(int argc, char** argv, VxContext* ctx);
}

bool g_ApplicationRunning = true;
namespace Walnut {

	int Main(int argc, char** argv, VxContext* ctx)
	{
		while (g_ApplicationRunning)
		{
			Walnut::Application* app = Walnut::CreateApplication(argc, argv, ctx);
			app->Run();
			delete app;
		}

		return 0;
	}

}

#if defined(WL_PLATFORM_WINDOWS) && !defined(WL_HEADLESS) && defined(WL_DIST)

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow, VxContext* ctx)
{
	return Walnut::Main(__argc, __argv, ctx);
}

#else

namespace VortexMaker{
static int DebugTools(int argc, char** argv, VxContext* ctx)
{
	return Walnut::Main(argc, argv, ctx);
}
}
#endif // defined(WL_PLATFORM_WINDOWS) && defined(WL_DIST)