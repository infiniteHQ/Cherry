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

#### CalcTextSizeX
Check the size of a rendered text on the X axis.

**Signature and Parameters:**
```lua
Cherry.CalcTextSizeX(text)
```
* **text**: Text to check.

**Example:**
```lua
Cherry.CalcTextSizeX(title)
```
</bloc>

<bloc>

#### CalcTextSizeY
Check the size of a rendered text on the Y axis.

**Signature and Parameters:**
```lua
Cherry.CalcTextSizeY(text)
```
* **text**: Text to check.

**Example:**
```lua
Cherry.CalcTextSizeY(title)
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

<bloc>

#### Script
Execute another lua script

**Signature and Parameters:**
```lua
Cherry.Script(url)
```
* **url**: Relative url of the script.

Note : The script can handle args (with nargs) and return values.

**Example:**
```lua
local clicked = Cherry.Script("ui/components/button")
```
</bloc>

<bloc>
#### FreshScript
Execute another lua script with live scripting

**Signature and Parameters:**
```lua
Cherry.Script(url)
```
* **url**: Relative url of the script.

Note : The script can handle args (with nargs) and return values.

**Example:**
```lua
local clicked = Cherry.Script("ui/components/button")
```
</bloc>

<bloc>

#### AbsoluteScript
Execute another lua script with absolute path

**Signature and Parameters:**
```lua
Cherry.AbsoluteScript(url)
```
* **url**: Relative url of the script.

Note : The script can handle args (with nargs) and return values.

**Example:**
```lua
local clicked = Cherry.Script("(full path)/ui/components/button")
```
</bloc>

<bloc>

#### FreshAbsoluteScript
Execute another lua script with live scripting and absolute path

**Signature and Parameters:**
```lua
Cherry.FreshAbsoluteScript(url)
```
* **url**: Relative url of the script.

Note : The script can handle args (with nargs) and return values.

**Example:**
```lua
local clicked = Cherry.Script("(full path)/ui/components/button")
```
</bloc>



<bloc>

#### Return and arguments example

**Example of main script:**
```lua
local string, int = Cherry.Script("resources/scripts", "hi component!", 500)

Cherry.DrawText(5, 40, 30, "#FF0000", string) -- hello
Cherry.DrawText(5, 70, 30, "#00F0FF", int) -- 42
```

**Example of subcomponent:**
```lua
local button_label, width = ...

button_label = button_label or "Default"
width = width or 50

Cherry.DrawRect(4, 4, width, 50, "#232323") -- 500
Cherry.DrawText(4, 4, 50, "#B1FF31", button_label) -- hi component!

return "hello", 42
```
</bloc>

<bloc>

#### CreateHook
Registers a hook that executes a Lua callback on a given schedule. An optional condition callback can gate execution.

**Signature and Parameters:**
```lua
Cherry.CreateHook(id, mode, callback, [condition])
```

| Parameter | Type | Description |
|---|---|---|
| `id` | string | Unique identifier for the hook |
| `mode` | string | Execution schedule: `"frame"` (every frame) or `"second"` (every second) |
| `callback` | function | Lua function called when the hook fires |
| `condition` | function | *(optional)* Lua function returning a boolean — hook only fires when it returns `true` |

**Example:**
```lua
Cherry.CreateHook("my_hook", "frame", function()
    -- called every frame
end)

Cherry.CreateHook("my_cond_hook", "second", function()
    -- called every second, only when condition is met
end, function()
    return someValue == true
end)
```
</bloc>

---

<bloc>

#### GetHookData
Retrieves a data value associated with a hook by key.

**Signature and Parameters:**
```lua
Cherry.GetHookData(id, key)
```

| Parameter | Type | Description |
|---|---|---|
| `id` | string | Identifier of the hook |
| `key` | string | Key of the data entry to retrieve |

**Example:**
```lua
local val = Cherry.GetHookData("my_hook", "counter")
```
</bloc>

---

<bloc>

#### SetHookData
Stores a data value on an existing hook by key.

**Signature and Parameters:**
```lua
Cherry.SetHookData(id, key, value)
```

| Parameter | Type | Description |
|---|---|---|
| `id` | string | Identifier of the hook |
| `key` | string | Key of the data entry to set |
| `value` | string | Value to store |

**Example:**
```lua
Cherry.SetHookData("my_hook", "counter", "42")
```
</bloc>

---

<bloc>

#### ClearHooks
Removes all registered hooks.

**Signature and Parameters:**
```lua
Cherry.ClearHooks()
```

**Example:**
```lua
Cherry.ClearHooks()
```
</bloc>

---

<bloc>

#### Use
Defines or updates a component in the component factory. Binds a name to a Lua render function, with optional default props. This is how you register a reusable component implementation.

**Signature and Parameters:**
```lua
Cherry.Use(name, render_fn)
Cherry.Use(name, defaults, render_fn)
```

| Parameter | Type | Description |
|---|---|---|
| `name` | string | Unique name for the component |
| `defaults` | table | *(optional)* Default props passed to the component if not overridden at draw time |
| `render_fn` | function | Lua function containing the component's rendering logic |

**Example:**
```lua
Cherry.Use("MyButton", { label = "Click me", color = "#FF0000" }, function()
    -- rendering logic here
end)

-- Without defaults
Cherry.Use("MyLabel", function()
    -- rendering logic here
end)
```
</bloc>

---

<bloc>

#### Draw
Draws a component that has been registered via `Use`. Instantiates it from the factory with the given props and renders it. Returns the component's identifier string.

**Signature and Parameters:**
```lua
local id = Cherry.Draw(name, [props])
```

| Parameter | Type | Description |
|---|---|---|
| `name` | string | Name of the registered component to draw |
| `props` | table | *(optional)* Props passed to the component, merged with its defaults |

**Example:**
```lua
local id = Cherry.Draw("MyButton", { label = "Submit" })
```
</bloc>

---

<bloc>

#### ClearComponentsRegistry
Clears all component definitions registered via `Use`. The factory will no longer know about any previously defined components.

**Signature and Parameters:**
```lua
Cherry.ClearComponentsRegistry()
```

**Example:**
```lua
Cherry.ClearComponentsRegistry()
```
</bloc>

---

<bloc>

#### ClearComponentsCache
Clears the component instance cache. Previously drawn component instances are discarded, but their definitions (registered via `Use`) remain intact.

**Signature and Parameters:**
```lua
Cherry.ClearComponentsCache()
```

**Example:**
```lua
Cherry.ClearComponentsCache()
```
</bloc>

---

<bloc>

#### RefreshComponents
Fully resets the component system: clears both the registry and the cache, and resets the component pool. Intended for development use only — **do not call every frame**, as it is resource-intensive.

**Signature and Parameters:**
```lua
Cherry.RefreshComponents()
```

**Example:**
```lua
-- Use during hot-reload or dev tooling only
Cherry.RefreshComponents()
```
</bloc>

---

<bloc>

#### BeginComponent
Pushes a component onto the component stack and sets it as the current active component. Must always be paired with `EndComponent`. All rendering or property calls made between `BeginComponent` and `EndComponent` are applied to this component.

**Signature and Parameters:**
```lua
Cherry.BeginComponent(id)
```

| Parameter | Type | Description |
|---|---|---|
| `id` | string | Identifier of the component to push and activate |

**Example:**
```lua
Cherry.BeginComponent("my_component")
-- rendering or property calls scoped to this component
Cherry.EndComponent()
```

</bloc>

---

<bloc>

#### EndComponent
Resets the current active component, closing the scope opened by `BeginComponent`. Must always be called after a `BeginComponent`.

**Signature and Parameters:**
```lua
Cherry.EndComponent()
```

**Example:**
```lua
Cherry.BeginComponent("my_component")
-- rendering or property calls scoped to this component
Cherry.EndComponent()
```

</bloc>

---

<bloc>

#### SetDrawCursorPos
Sets the current drawing cursor position on both axes, relative to the current window or container.

**Signature and Parameters:**
```lua
Cherry.SetDrawCursorPos(x, y)
```

| Parameter | Type | Description |
|---|---|---|
| `x` | number | Horizontal position of the cursor |
| `y` | number | Vertical position of the cursor |

**Example:**
```lua
Cherry.SetDrawCursorPos(100, 50)
```

</bloc>

---

<bloc>

#### SetDrawCursorPosX
Sets only the horizontal position of the drawing cursor, leaving the vertical position unchanged.

**Signature and Parameters:**
```lua
Cherry.SetDrawCursorPosX(x)
```

| Parameter | Type | Description |
|---|---|---|
| `x` | number | Horizontal position of the cursor |

**Example:**
```lua
Cherry.SetDrawCursorPosX(100)
```

</bloc>

---

<bloc>

#### SetDrawCursorPosY
Sets only the vertical position of the drawing cursor, leaving the horizontal position unchanged.

**Signature and Parameters:**
```lua
Cherry.SetDrawCursorPosY(y)
```

| Parameter | Type | Description |
|---|---|---|
| `y` | number | Vertical position of the cursor |

**Example:**
```lua
Cherry.SetDrawCursorPosY(50)
```

</bloc>

---

<bloc>

#### PushFont
Pushes a font onto the font stack, making it the active font for all subsequent rendering calls. Must always be paired with `PopFont`.

**Signature and Parameters:**
```lua
Cherry.PushFont(font)
```

| Parameter | Type | Description |
|---|---|---|
| `font` | string | Identifier of the font to activate |

**Example:**
```lua
Cherry.PushFont("roboto_bold")
-- widgets rendered here use roboto_bold
Cherry.PopFont()
```

</bloc>

---

<bloc>


#### PopFont
Pops the current font from the font stack, restoring the previously active font. Must always be called after a `PushFont`.

**Signature and Parameters:**
```lua
Cherry.PopFont()
```

**Example:**
```lua
Cherry.PushFont("roboto_bold")
-- widgets rendered here use roboto_bold
Cherry.PopFont()
```

</bloc>


<bloc>

#### GetCurrentComponentID
Returns the identifier string of the currently active component.

**Signature and Parameters:**
```lua
local id = Cherry.GetCurrentComponentID()
```

**Example:**
```lua
Cherry.BeginComponent("my_component")
local id = Cherry.GetCurrentComponentID()
Cherry.EndComponent()
```
</bloc>

<bloc>

#### SetCurrentComponentProperty
Sets a property on the currently active component by key. Properties are typically used for styling or configuration values that affect rendering.

**Signature and Parameters:**
```lua
Cherry.SetCurrentComponentProperty(key, value)
```

* **key**: Name of the property to set.
* **value**: Value to assign (string).

**Example:**
```lua
Cherry.BeginComponent("my_component")
Cherry.SetCurrentComponentProperty("color", "#FF0000FF")
Cherry.EndComponent()
```
</bloc>

<bloc>

#### GetCurrentComponentProperty
Retrieves a property value from the currently active component by key.

**Signature and Parameters:**
```lua
local value = Cherry.GetCurrentComponentProperty(key)
```

* **key**: Name of the property to retrieve.

**Example:**
```lua
Cherry.BeginComponent("my_component")
local color = Cherry.GetCurrentComponentProperty("color")
Cherry.EndComponent()
```
</bloc>

<bloc>

#### SetCurrentComponentData
Stores a data entry on the currently active component by key. Data is typically used for runtime state that persists across frames, as opposed to props or properties.

**Signature and Parameters:**
```lua
Cherry.SetCurrentComponentData(key, value)
```

* **key**: Name of the data entry to set.
* **value**: Value to assign (string).

**Example:**
```lua
Cherry.BeginComponent("my_component")
Cherry.SetCurrentComponentData("counter", "0")
Cherry.EndComponent()
```
</bloc>

<bloc>

#### GetCurrentComponentData
Retrieves a data value from the currently active component by key.

**Signature and Parameters:**
```lua
local value = Cherry.GetCurrentComponentData(key)
```

* **key**: Name of the data entry to retrieve.

**Example:**
```lua
Cherry.BeginComponent("my_component")
local counter = Cherry.GetCurrentComponentData("counter")
Cherry.EndComponent()
```
</bloc>

<bloc>

#### IsMouseOverRect
Returns `true` if the mouse is currently hovering over a rectangle defined in relative window coordinates.

**Signature and Parameters:**
```lua
local hovered = Cherry.IsMouseOverRect(x, y, w, h)
```

* **x, y**: Top-left corner of the rectangle, relative to the current window position.
* **w, h**: Width and height of the rectangle.

**Example:**
```lua
if Cherry.IsMouseOverRect(10, 10, 200, 40) then
    -- mouse is hovering the zone
end
```
</bloc>

<bloc>

#### IsMouseClickedOnCurrentPos
Returns `true` if the specified mouse button was clicked inside a rectangle defined in relative window coordinates. Also requires the current window to be hovered.

**Signature and Parameters:**
```lua
local clicked = Cherry.IsMouseClickedOnCurrentPos(x, y, w, h, button, [repeat])
```

* **x, y**: Top-left corner of the rectangle, relative to the current window position.
* **w, h**: Width and height of the rectangle.
* **button**: Mouse button index (`0` = left, `1` = right, `2` = middle).
* **repeat**: *(optional)* Whether to fire repeatedly while the button is held. Default: `false`.

**Example:**
```lua
if Cherry.IsMouseClickedOnCurrentPos(10, 10, 200, 40, 0) then
    -- left click detected inside the zone
end

if Cherry.IsMouseClickedOnCurrentPos(10, 60, 200, 40, 1, true) then
    -- right click with repeat enabled
end
```
</bloc>