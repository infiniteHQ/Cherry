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
    // --- Logic & Input Functions ---
    {"Cherry.IsMouseClickedOnPos", "x, y, w, h, button, [repeat]",
     "Checks if a mouse button is clicked within a specific window-relative "
     "area."},

    {"Cherry.IsMouseDoubleClickedOnPos", "x, y, w, h, button",
     "Checks if a mouse button is double-clicked within a specific "
     "window-relative area."},

    {"Cherry.IsKeyPressedOnPos", "x, y, w, h, key_name",
     "Checks if a key is pressed while the mouse is hovering a specific "
     "window-relative area."},

    {"Cherry.IsMouseClicked", "button, [repeat]",
     "Returns true if the specified mouse button was clicked anywhere this "
     "frame."},

    {"Cherry.IsMouseDoubleClicked", "button",
     "Returns true if the specified mouse button was double-clicked anywhere."},

    {"Cherry.GetMouseScreenPos", "",
     "Returns the absolute screen coordinates (x, y) of the mouse cursor."},

    {"Cherry.GetMouseScreenPosY", "",
     "Returns the absolute vertical screen position (y) of the mouse cursor."},

    {"Cherry.GetMouseScreenPosX", "",
     "Returns the absolute horizontal screen position (x) of the mouse "
     "cursor."},

    {"Cherry.GetMousePos", "",
     "Returns the mouse cursor position (x, y) relative to the current "
     "window."},

    {"Cherry.GetMousePosY", "",
     "Returns the vertical mouse position (y) relative to the current window."},

    {"Cherry.GetMousePosX", "",
     "Returns the horizontal mouse position (x) relative to the current "
     "window."},

    {"Cherry.GetDrawCursorPos", "",
     "Returns the current (x, y) position of the UI drawing cursor."},

    {"Cherry.GetDrawCursorPosY", "",
     "Returns the current vertical position (y) of the UI drawing cursor."},

    {"Cherry.GetDrawCursorPosX", "",
     "Returns the current horizontal position (x) of the UI drawing cursor."},

    {"Cherry.GetWindowSize", "",
     "Returns the width and height of the active window."},

    {"Cherry.GetWindowSizeX", "", "Returns the width of the active window."},

    {"Cherry.GetWindowSizeY", "", "Returns the height of the active window."},

    {"Cherry.GetWindowPos", "",
     "Returns the absolute screen coordinates (x, y) of the window origin."},

    {"Cherry.GetWindowPosX", "",
     "Returns the absolute horizontal screen position (x) of the window "
     "origin."},

    {"Cherry.GetWindowPosY", "",
     "Returns the absolute vertical screen position (y) of the window origin."},

    {"Cherry.GetPath", "relative_path",
     "Converts a relative path to an absolute engine filesystem path."},

    {"Cherry.PlaySound", "audio_file_path",
     "Plays an audio file. Requires CHERRY_ENABLE_AUDIO compilation flag."},

    {"Cherry.GetLocale", "key",
     "Retrieves a translated string based on the current engine locale."},

    {"Cherry.SetTheme", "theme_name",
     "Globally changes the application theme (e.g., 'Dark', 'Light')."},

    {"Cherry.PushTheme", "theme_name",
     "Pushes a theme onto the stack; following elements use this theme until "
     "PopTheme."},

    {"Cherry.PopTheme", "",
     "Removes the current theme from the stack and reverts to the previous "
     "one."},

    {"Cherry.GetThemeProperty", "theme_name, key",
     "Retrieves a specific property value from a named theme."},

    {"Cherry.GetActiveThemeProperty", "key",
     "Retrieves a property value from the currently active engine theme."},

    {"Cherry.Log", "message",
     "Prints an informational message to the engine console."},

    {"Cherry.LogInfo", "message",
     "Prints an informational message to the engine console."},

    {"Cherry.LogWarn", "message",
     "Prints a warning message to the console (usually yellow)."},

    {"Cherry.LogError", "message",
     "Prints an error message to the console (usually red)."},

    {"Cherry.LogFatal", "message",
     "Prints a critical failure message regarding application stability."},

    {"Cherry.LogTrace", "message",
     "Prints a detailed trace-level debugging message."},

    {"Cherry.IsKeyPressed", "key_name",
     "Returns true if the specified keyboard key is currently held down."},
    {"Cherry.SetDrawCursorPos", "x, y",
     "Sets the current (x, y) position of the UI drawing cursor relative to "
     "the window."},

    {"Cherry.SetDrawCursorPosX", "x",
     "Sets the current horizontal position (x) of the UI drawing cursor."},

    {"Cherry.SetDrawCursorPosY", "y",
     "Sets the current vertical position (y) of the UI drawing cursor."},

    {"Cherry.SetComponentProperty", "id, key, value",
     "Sets a property for a component identified by its ID string."},

    {"Cherry.GetComponentProperty", "id, key",
     "Returns the value of a property for a specific component."},

    {"Cherry.SetComponentData", "id, key, value",
     "Sets internal data for a component identified by its ID string."},

    {"Cherry.GetComponentData", "id, key",
     "Returns internal data for a specific component."},
};

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