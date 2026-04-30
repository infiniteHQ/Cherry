//
//  options.hpp
//  Options for Cherry API symbols
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

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