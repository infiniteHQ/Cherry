#include "../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_TOOL
#define CHERRY_DEBUG_TOOL

#include <memory>

#include "views/components/components.hpp"
#include "views/console/console.hpp"

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

     private:
    };
  }  // namespace Tools
}  // namespace Cherry

#endif  // CHERRY_DEBUG_TOOL
#endif  // CHERRY_DEBUG
