### Logic API
This section provides functions for all logic features of Cherry.

<bloc>

#### IsMouseClickedOnPos
Checks if a specific mouse button has been clicked while the cursor is within a defined rectangular area relative to the current window.

**Signature and Parameters:**
```lua
Cherry.IsMouseClickedOnPos(x, y, w, h, button, repeat)
```
* **x**: Horizontal offset relative to the window position.
* **y**: Vertical offset relative to the window position.
* **w**: Width of the detection area.
* **h**: Height of the detection area.
* **button**: Mouse button index (0: Left, 1: Right, 2: Middle).
* **repeat**: (Optional) Boolean, if true, the click is detected even if held down.

**Example:**
```lua
if Cherry.IsMouseClickedOnPos(0, 0, 100, 50, 0) then
    Cherry.Log("Clicked in the top-left corner region!")
end
```
</bloc>

<bloc>

#### IsMouseDoubleClickedOnPos
Checks if a specific mouse button has been double-clicked within a rectangular area relative to the current window.

**Signature and Parameters:**
```lua
Cherry.IsMouseDoubleClickedOnPos(x, y, w, h, button)
```
* **x**: Horizontal offset relative to the window position.
* **y**: Vertical offset relative to the window position.
* **w**: Width of the detection area.
* **h**: Height of the detection area.
* **button**: Mouse button index.

**Example:**
```lua
if Cherry.IsMouseDoubleClickedOnPos(10, 10, 50, 50, 0) then
    Cherry.Log("Double click detected!")
end
```
</bloc>

<bloc>

#### IsKeyPressedOnPos
Checks if a keyboard key is pressed, but only if the mouse cursor is currently hovering over a specific rectangular area relative to the window.

**Signature and Parameters:**
```lua
Cherry.IsKeyPressedOnPos(x, y, w, h, key_name)
```
* **x**: Horizontal offset relative to the window position.
* **y**: Vertical offset relative to the window position.
* **w**: Width of the detection area.
* **h**: Height of the detection area.
* **key_name**: String representation of the key (e.g., "Space", "Enter", "A").

**Example:**
```lua
if Cherry.IsKeyPressedOnPos(0, 0, 200, 200, "R") then
    Cherry.Log("Key R pressed while hovering the area!")
end
```
</bloc>

<bloc>

#### IsMouseClicked
Global check to see if a mouse button was clicked during the current frame.

**Signature and Parameters:**
```lua
Cherry.IsMouseClicked(button, repeat)
```
* **button**: Mouse button index (0: Left, 1: Right, 2: Middle).
* **repeat**: (Optional) Boolean, set to true to detect held-down repetition.

**Example:**
```lua
if Cherry.IsMouseClicked(1) then
    Cherry.Log("Right click detected anywhere!")
end
```
</bloc>

<bloc>

#### IsMouseDoubleClicked
Global check to see if a mouse button was double-clicked.

**Signature and Parameters:**
```lua
Cherry.IsMouseDoubleClicked(button)
```
* **button**: Mouse button index.

**Example:**
```lua
if Cherry.IsMouseDoubleClicked(0) then
    Cherry.Log("Global double click!")
end
```
</bloc>

<bloc>

#### GetMouseScreenPos
Retrieves the current absolute coordinates of the mouse cursor on the screen.

**Signature and Parameters:**
```lua
local x, y = Cherry.GetMouseScreenPos()
```
* **Returns x**: The absolute horizontal position in pixels.
* **Returns y**: The absolute vertical position in pixels.

**Example:**
```lua
local mouse_x, mouse_y = Cherry.GetMouseScreenPos()
Cherry.DrawTextFixed("Mouse is at: " .. mouse_x .. ", " .. mouse_y, mouse_x, mouse_y, "#FFFFFFFF")
```
</bloc>

<bloc>

#### GetMouseScreenPosY
Retrieves the current absolute vertical position of the mouse cursor on the screen.

**Signature and Parameters:**
```lua
local y = Cherry.GetMouseScreenPosY()
```
* **Returns y**: The absolute vertical position in pixels.

**Example:**
```lua
local my = Cherry.GetMouseScreenPosY()
if my > 540 then
    Cherry.Log("Mouse is in the bottom half of the screen")
end
```
</bloc>

<bloc>

#### GetMouseScreenPosX
Retrieves the current absolute horizontal position of the mouse cursor on the screen.

**Signature and Parameters:**
```lua
local x = Cherry.GetMouseScreenPosX()
```
* **Returns x**: The absolute horizontal position in pixels.

**Example:**
```lua
local mx = Cherry.GetMouseScreenPosX()
```
</bloc>

<bloc>

#### GetMousePos
Retrieves the mouse cursor position relative to the top-left corner of the current window.

**Signature and Parameters:**
```lua
local x, y = Cherry.GetMousePos()
```
* **Returns x**: Horizontal position relative to the window.
* **Returns y**: Vertical position relative to the window.

**Example:**
```lua
local lx, ly = Cherry.GetMousePos()
if lx < 0 or ly < 0 then
    Cherry.Log("Mouse is outside the window bounds")
end
```
</bloc>

<bloc>

#### GetMousePosY
Retrieves the mouse cursor's vertical position relative to the current window.

**Signature and Parameters:**
```lua
local y = Cherry.GetMousePosY()
```
* **Returns y**: Vertical position relative to the window.

**Example:**
```lua
local ly = Cherry.GetMousePosY()
```
</bloc>

<bloc>

#### GetMousePosX
Retrieves the mouse cursor's horizontal position relative to the current window.

**Signature and Parameters:**
```lua
local x = Cherry.GetMousePosX()
```
* **Returns x**: Horizontal position relative to the window.

**Example:**
```lua
local lx = Cherry.GetMousePosX()
```
</bloc>

<bloc>

#### GetDrawCursorPos
Retrieves the current position of the UI drawing cursor within the window. This is where the next element will be rendered by default.

**Signature and Parameters:**
```lua
local x, y = Cherry.GetDrawCursorPos()
```
* **Returns x**: The current cursor X position.
* **Returns y**: The current cursor Y position.

**Example:**
```lua
local cx, cy = Cherry.GetDrawCursorPos()
Cherry.DrawText("Element starting here", cx, cy, "#FFFFFFFF")
```
</bloc>

<bloc>

#### GetDrawCursorPosY
Retrieves the current vertical position of the UI drawing cursor.

**Signature and Parameters:**
```lua
local y = Cherry.GetDrawCursorPosY()
```
* **Returns y**: The current cursor Y position.

**Example:**
```lua
local cy = Cherry.GetDrawCursorPosY()
```
</bloc>

<bloc>

#### GetDrawCursorPosX
Retrieves the current horizontal position of the UI drawing cursor.

**Signature and Parameters:**
```lua
local x = Cherry.GetDrawCursorPosX()
```
* **Returns x**: The current cursor X position.

**Example:**
```lua
local cx = Cherry.GetDrawCursorPosX()
```
</bloc>

<bloc>

#### GetWindowSize
Retrieves the current dimensions (width and height) of the active window.

**Signature and Parameters:**
```lua
local width, height = Cherry.GetWindowSize()
```
* **Returns width**: The window width in pixels.
* **Returns height**: The window height in pixels.

**Example:**
```lua
local w, h = Cherry.GetWindowSize()
Cherry.DrawLineFixed(0, 0, w, h, "#FF0000FF", 1.0) -- Diagonal across the window
```
</bloc>

<bloc>

#### GetWindowSizeX
Retrieves the current width of the active window.

**Signature and Parameters:**
```lua
local width = Cherry.GetWindowSizeX()
```
* **Returns width**: The window width in pixels.

**Example:**
```lua
local w = Cherry.GetWindowSizeX()
if w < 800 then
    Cherry.Log("Window is too narrow!")
end
```
</bloc>

<bloc>

#### GetWindowSizeY
Retrieves the current height of the active window.

**Signature and Parameters:**
```lua
local height = Cherry.GetWindowSizeY()
```
* **Returns height**: The window height in pixels.

**Example:**
```lua
local h = Cherry.GetWindowSizeY()
```
</bloc>

<bloc>

#### GetWindowPos
Retrieves the absolute screen coordinates of the current window's top-left corner.

**Signature and Parameters:**
```lua
local x, y = Cherry.GetWindowPos()
```
* **Returns x**: The horizontal position on the screen.
* **Returns y**: The vertical position on the screen.

**Example:**
```lua
local wx, wy = Cherry.GetWindowPos()
Cherry.Log("Window origin is at: " .. wx .. ", " .. wy)
```
</bloc>

<bloc>

#### GetWindowPosX
Retrieves the absolute horizontal position of the window on the screen.

**Signature and Parameters:**
```lua
local x = Cherry.GetWindowPosX()
```
* **Returns x**: The screen X position.

**Example:**
```lua
local wx = Cherry.GetWindowPosX()
```
</bloc>

<bloc>

#### GetWindowPosY
Retrieves the absolute vertical position of the window on the screen.

**Signature and Parameters:**
```lua
local y = Cherry.GetWindowPosY()
```
* **Returns y**: The screen Y position.

**Example:**
```lua
local wy = Cherry.GetWindowPosY()
```
</bloc>

<bloc>

#### GetPath
Converts a relative resource path into an absolute filesystem path used by the Cherry engine.

**Signature and Parameters:**
```lua
local absolute_path = Cherry.GetPath(relative_path)
```
* **relative_path**: The path to resolve (e.g., "assets/textures/logo.png").

**Example:**
```lua
local full_path = Cherry.GetPath("scripts/main.lua")
Cherry.Log("Resolved path: " .. full_path)
```
</bloc>

<bloc>

#### PlaySound
Plays an audio file.
> **Note:** This function requires the compilation flag `CHERRY_ENABLE_AUDIO` to be enabled.

**Signature and Parameters:**
```lua
Cherry.PlaySound(audio_file_path)
```
* **audio_file_path**: The path to the audio file to play.

**Example:**
```lua
Cherry.PlaySound("assets/sounds/click.wav")
```
</bloc>

<bloc>

#### GetLocale
Retrieves a translated string based on the provided key and the current engine locale.

**Signature and Parameters:**
```lua
local translated_text = Cherry.GetLocale(key)
```
* **key**: The translation key (e.g., "MENU_START").

**Example:**
```lua
local label = Cherry.GetLocale("UI_WELCOME_MSG")
Cherry.DrawText(label, 10, 10, "#FFFFFFFF")
```
</bloc>

<bloc>

#### SetTheme
Globally changes the application theme.

**Signature and Parameters:**
```lua
Cherry.SetTheme(theme_name)
```
* **theme_name**: The name of the theme to apply (e.g., "Dark", "Light", "CherryClassic").

**Example:**
```lua
Cherry.SetTheme("Dark")
```
</bloc>

<bloc>

#### PushTheme
Pushes a new theme onto the theme stack. All following elements will use this theme until `PopTheme` is called.

**Signature and Parameters:**
```lua
Cherry.PushTheme(theme_name)
```
* **theme_name**: The name of the temporary theme.

**Example:**
```lua
Cherry.PushTheme("HighContrast")
-- Custom UI code here
Cherry.PopTheme()
```
</bloc>

<bloc>

#### PopTheme
Removes the current theme from the top of the theme stack, reverting to the previous one.

**Signature and Parameters:**
```lua
Cherry.PopTheme()
```

**Example:**
```lua
Cherry.PushTheme("Light")
-- Draw something in light mode
Cherry.PopTheme()
```
</bloc>

<bloc>

#### GetThemeProperty
Retrieves a specific property value from a named theme.

**Signature and Parameters:**
```lua
local value = Cherry.GetThemeProperty(theme_name, key)
```
* **theme_name**: The name of the theme to query.
* **key**: The property key (e.g., "ButtonBg", "FontSize").

**Example:**
```lua
local btnColor = Cherry.GetThemeProperty("Dark", "ButtonPrimary")
Cherry.Log("The primary button color in Dark theme is: " .. btnColor)
```
</bloc>

<bloc>

#### GetActiveThemeProperty
Retrieves a property value from the theme currently active in the engine.

**Signature and Parameters:**
```lua
local value = Cherry.GetActiveThemeProperty(key)
```
* **key**: The property key to look up.

**Example:**
```lua
local currentBg = Cherry.GetActiveThemeProperty("WindowBg")
```
</bloc>

<bloc>

#### Log / LogInfo
Prints an informational message to the engine console and log files.

**Signature and Parameters:**
```lua
Cherry.Log(message)
Cherry.LogInfo(message)
```
* **message**: The string message to log.

**Example:**
```lua
Cherry.Log("Script initialization complete.")
```
</bloc>

<bloc>

#### LogWarn
Prints a warning message to the console. Usually highlighted in yellow.

**Signature and Parameters:**
```lua
Cherry.LogWarn(message)
```
* **message**: The warning message.

**Example:**
```lua
Cherry.LogWarn("Asset not found, using placeholder.")
```
</bloc>

<bloc>

#### LogError
Prints an error message to the console. Usually highlighted in red.

**Signature and Parameters:**
```lua
Cherry.LogError(message)
```
* **message**: The error message.

**Example:**
```lua
if not file_exists then
    Cherry.LogError("Failed to load configuration file!")
end
```
</bloc>

<bloc>

#### LogFatal
Prints a fatal error message. These logs often represent critical failures that might lead to an application crash.

**Signature and Parameters:**
```lua
Cherry.LogFatal(message)
```
* **message**: The critical error message.

**Example:**
```lua
Cherry.LogFatal("Graphics context lost!")
```
</bloc>

<bloc>

#### LogTrace
Prints a trace-level message. Used for very detailed debugging information that is normally hidden in production.

**Signature and Parameters:**
```lua
Cherry.LogTrace(message)
```
* **message**: The detailed trace message.

**Example:**
```lua
Cherry.LogTrace("Update loop iteration: " .. frame_count)
```
</bloc>

<bloc>

#### IsKeyPressed
Checks if a specific keyboard key is currently being pressed down.

**Signature and Parameters:**
```lua
local pressed = Cherry.IsKeyPressed(key_name)
```
* **key_name**: The string name of the key (e.g., "W", "Escape", "Space", "LeftArrow").

**Example:**
```lua
if Cherry.IsKeyPressed("Escape") then
    Cherry.Log("Exit requested via keyboard.")
end
```
</bloc>

<bloc>

#### SetComponentProperty
Modifies a property of a specific component using its unique string identifier.

**Signature and Parameters:**
```lua
Cherry.SetComponentProperty(id, key, value)
```
* **id**: The string identifier of the component (e.g., "#main_window" or "submit_btn").
* **key**: The property name to modify.
* **value**: The new value as a string.

**Example:**
```lua
-- Change the label of a button dynamically
Cherry.SetComponentProperty("login_button", "label", "Connecting...")
```
</bloc>

<bloc>

#### GetComponentProperty
Retrieves the value of a property from a specific component.

**Signature and Parameters:**
```lua
local val = Cherry.GetComponentProperty(id, key)
```
* **id**: The string identifier of the component.
* **key**: The property name to query.
* **Returns**: The property value as a string, or an empty string if the component/key doesn't exist.

**Example:**
```lua
local current_theme = Cherry.GetComponentProperty("root_node", "theme_variant")
Cherry.Log("Current node theme: " .. current_theme)
```
</bloc>

<bloc>

#### SetComponentData
Sets internal state data for a component. Unlike properties, data is often used for logic rather than visual configuration.

**Signature and Parameters:**
```lua
Cherry.SetComponentData(id, key, value)
```
* **id**: The string identifier of the component.
* **key**: The data key to store.
* **value**: The value to store.

**Example:**
```lua
-- Store a temporary score in a UI component
Cherry.SetComponentData("score_display", "last_points", "500")
```
</bloc>

<bloc>

#### GetComponentData
Retrieves internal data stored in a specific component.

**Signature and Parameters:**
```lua
local val = Cherry.GetComponentData(id, key)
```
* **id**: The string identifier of the component.
* **key**: The data key to query.
* **Returns**: The data value as a string, or an empty string if the component/key doesn't exist.

**Example:**
```lua
local health = Cherry.GetComponentData("player_stats", "hp")
if health == "0" then
    Cherry.LogWarn("Player is dead!")
end
```
</bloc>