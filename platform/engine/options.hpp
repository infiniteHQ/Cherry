#pragma once

#if defined(_WIN32) || defined(_WIN64)
  #if defined(CHERRY_EXPORT_SYM)
    #define CHERRY_API __declspec(dllexport)
  #elif defined(CHERRY_IMPORT_DLL)
    #define CHERRY_API __declspec(dllimport)
  #else
    #define CHERRY_API
  #endif
#else
  #define CHERRY_API
#endif