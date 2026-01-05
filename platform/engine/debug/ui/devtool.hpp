#include "../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_TOOL
#define CHERRY_DEBUG_TOOL

#include "views/about/about.hpp"
#include "views/general/general.hpp"
#include "views/inspector/inspector.hpp"

#include <memory>

namespace Cherry {
namespace Tools {

class Devtool {
public:
  Devtool();
  ~Devtool();

  void Start();
  void Stop();

  std::shared_ptr<GeneralView> GeneralAppWindow;
  std::shared_ptr<AboutView> AboutAppWindow;
  std::shared_ptr<InspectorView> InspectorAppWindow;

private:
};
} // namespace Tools
} // namespace Cherry

#endif // CHERRY_DEBUG_TOOL
#endif // CHERRY_DEBUG
