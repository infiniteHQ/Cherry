//
//  debug.hpp
//  Headers for Cherry debug tools
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include "../../../options.hpp"

#ifdef CHERRY_DEBUG

namespace Cherry {
  class Debugger {
   public:
    static void StartDevTools();
    static void StopDevTools();
  };
}  // namespace Cherry

#endif  // CHERRY_DEBUG