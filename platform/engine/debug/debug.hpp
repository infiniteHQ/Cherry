#pragma once

#include "../../../options.hpp"

#ifdef CHERRY_DEBUG

namespace Cherry {
class Debugger {
public:
  static void StartDevTools();
  static void StopDevTools();
};
} // namespace Cherry

#endif // CHERRY_DEBUG