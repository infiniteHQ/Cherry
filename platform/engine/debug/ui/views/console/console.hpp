#include "../../../../../../options.hpp"

#ifdef CHERRY_DEBUG
#ifndef CHERRY_DEBUG_VIEW_CONSOLE
#define CHERRY_DEBUG_VIEW_CONSOLE

#include "../../../../../../src/core/log.hpp"
#include "../../../../app_window/app_window.hpp"
#include "../../../../scripting/scripting.hpp"
struct ConsoleEntry {
  Cherry::Log::Level level;
  std::string message;
  int count;
};

static std::vector<ConsoleEntry> s_Entries;
static char s_InputBuffer[256] = "";
static bool s_ScrollToBottom = false;
static std::vector<std::string> s_CommandHistory;
static int s_HistoryPos = -1;

static int s_SelectionIdx = -1;
struct CommandDef {
  std::string name;
  std::string params;
  std::string description;
};

static std::vector<CommandDef> s_CommandDefs = {
    {"Cherry.DrawRect", "x, y, w, h, color", "Draws a solid rectangle."},
    {"Cherry.DrawCircle", "x, y, radius, color", "Draws a solid circle."},
    {"Cherry.DrawLine", "x1, y1, x2, y2, color, thickness",
     "Draws a line between two points."},
    {"Cherry.GetMousePos", "", "Returns x, y coordinates of the mouse."},
    {"print", "value", "Prints a value to the console."},
    {"Cherry.IsKeyPressed", "key_name", "Returns true if the key is held."}};

class ConsoleView : public std::enable_shared_from_this<ConsoleView> {
public:
  ConsoleView(const std::string &name);
  std::shared_ptr<Cherry::AppWindow> &GetAppWindow();
  static std::shared_ptr<ConsoleView> Create(const std::string &name);

  void SetupRenderCallback();
  void AddLogEntry(Cherry::Log::Level level, const std::string &msg);
  void Init();

  void Render();
  std::shared_ptr<Cherry::AppWindow> m_AppWindow;
};

#endif // CHERRY_DEBUG_VIEW_CONSOLE
#endif // CHERRY_DEBUG