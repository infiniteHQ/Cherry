#pragma once

#include "./application.hpp"

#ifndef UIKIT_ENTRYPOINT_H
#define UIKIT_ENTRYPOINT_H

namespace Cherry {
extern Cherry::Application *CreateApplication(int argc, char **argv);
}

inline bool g_ApplicationRunning = true;
namespace Cherry {

// For multiple app usage
static int ThirdMain(int argc, char **argv,
                     Cherry::Application *(*create_app)(int argc,
                                                        char **argv)) {
  while (g_ApplicationRunning) {
    Cherry::Application *app = create_app(argc, argv);
    switch (app->m_DefaultSpecification.RuntimeMode) {
    case Runtime::SingleThread: {
      app->SingleThreadRuntime();
      break;
    }
    default: {
      app->SingleThreadRuntime();
      break;
    }
    }
    delete app;
  }

  return 0;
}

// For single app usage
static int Main(int argc, char **argv) {
  while (g_ApplicationRunning) {
    Cherry::Application *app = Cherry::CreateApplication(argc, argv);
    switch (app->m_DefaultSpecification.RuntimeMode) {
    case Runtime::SingleThread: {
      app->SingleThreadRuntime();
      break;
    }
    default: {
      app->SingleThreadRuntime();
      break;
    }
    }
    delete app;
  }

  return 0;
}

static bool IsRunning() { return g_ApplicationRunning; }

} // namespace Cherry

#if defined(WL_PLATFORM_WINDOWS) && !defined(WL_HEADLESS) && defined(WL_DIST)

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline,
                     int cmdshow) {
  return Cherry::Main(__argc, __argv, ctx);
}

#else

#endif // defined(WL_PLATFORM_WINDOWS) && defined(WL_DIST)
#endif // UIKIT_ENTRYPOINT_H