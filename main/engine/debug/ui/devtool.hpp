//
//  devtools.hpp
//  Main headers for Cherry devtools window
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_TOOL
#define CHERRY_DEBUG_TOOL

#include <memory>

#include "views/components/components.hpp"
#include "views/console/console.hpp"
#include "views/hooks/hooks.hpp"
#include "views/windows/windows.hpp"

namespace Cherry {
  namespace Tools {

    class Devtool {
     public:
      Devtool();
      ~Devtool();

      void Start();
      void Stop();

      std::shared_ptr<ConsoleView> ConsoleAppWindow;
      std::shared_ptr<ComponentsView> ComponentsAppWindow;
      std::shared_ptr<HooksView> HooksAppWindow;
      std::shared_ptr<WindowsView> WindowsAppWindow;

     private:
    };
  }  // namespace Tools
}  // namespace Cherry

#endif  // CHERRY_DEBUG_TOOL
#endif  // CHERRY_DEBUG
