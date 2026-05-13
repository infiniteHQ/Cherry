This is all the high level C++ API reference of Cherry. You will find here all features and functions you can call in your C++ application.

<banner type="warning">This reference covers only the high-level API and does not include accessible functions from `CherryApp`, `CherryAppWindow`, or `CherryWindow` objects that can be called everywhere. Please refer to [our examples](https://github.com/infiniteHQ/Cherry/tree/main/examples/concepts) to learn more about how to create Cherry apps.</banner>

## Window

### GetCurrentRenderedWindow

Get the currently rendered `Cherry::Window`.

```cpp
std::shared_ptr<Cherry::Window>& Cherry::GetCurrentRenderedWindow();
```

Returns:
- The current rendered window reference.

Example:

```cpp
auto window = Cherry::GetCurrentRenderedWindow();

if (window)
{
    // Use the current rendered window
}
```

---

## AppWindow

### AddAppWindow

Add an `AppWindow` to the application.

```cpp
void Cherry::AddAppWindow(const std::shared_ptr<AppWindow>& win);
```

Parameters:
- `win` : The `AppWindow` instance to add.

Example:

```cpp
auto window = std::make_shared<AppWindow>();

Cherry::AddAppWindow(window);
```

---

### DeleteAppWindow

Delete an existing `AppWindow`.

```cpp
void Cherry::DeleteAppWindow(const std::shared_ptr<AppWindow>& win);
```

Parameters:
- `win` : The `AppWindow` instance to delete.

Example:

```cpp
Cherry::DeleteAppWindow(window);
```

---

### GetAppWindowByName

Get an `AppWindow` from its name.

```cpp
std::shared_ptr<AppWindow> Cherry::GetAppWindowByName(const std::string& win_name);
```

Parameters:
- `win_name` : Name of the window to search.

Returns:
- The matching `AppWindow`.

Example:

```cpp
auto window = Cherry::GetAppWindowByName("MainWindow");
```

---

### GetCurrentRenderedAppWindow

Get the currently rendered `AppWindow`.

```cpp
std::shared_ptr<AppWindow> Cherry::GetCurrentRenderedAppWindow();
```

Returns:
- The current rendered `AppWindow`.

Example:

```cpp
auto appWindow = Cherry::GetCurrentRenderedAppWindow();
```

---

### GetWindowByName

Get a `Window` from its name.

```cpp
std::shared_ptr<Window> Cherry::GetWindowByName(const std::string& win_name);
```

Parameters:
- `win_name` : Name of the window to search.

Returns:
- The matching `Window`.

Example:

```cpp
auto window = Cherry::GetWindowByName("Settings");
```

---

## Images

### GetTexture

Load or get a texture from a file path.

```cpp
ImTextureID Cherry::GetTexture(const std::string& path);
```

Parameters:
- `path` : Path to the texture file.

Returns:
- The loaded `ImTextureID`.

Example:

```cpp
ImTextureID texture = Cherry::GetTexture("assets/logo.png");

ImGui::Image(texture, ImVec2(128, 128));
```

---

### GetTextureSize

Get the size of a texture.

```cpp
ImVec2 Cherry::GetTextureSize(const std::string& path);
```

Parameters:
- `path` : Path to the texture file.

Returns:
- The texture size as `ImVec2`.

Example:

```cpp
ImVec2 size = Cherry::GetTextureSize("assets/logo.png");
```

---

## Notifications

### AddNotification

Display a notification toast.

```cpp
void Cherry::AddNotification(const ImGuiToast& toast);
```

Parameters:
- `toast` : The notification toast to display.

Example:

```cpp
ImGuiToast toast(ImGuiToastType_Info, 3000);

toast.set_title("Cherry");
toast.set_content("Notification example");

Cherry::AddNotification(toast);
```

---

## Fonts

### PushFont

Push a font into the current font stack.

```cpp
void Cherry::PushFont(const std::string& font_name);
```

Parameters:
- `font_name` : Name of the font to use.

Example:

```cpp
Cherry::PushFont("Bold");

ImGui::Text("Hello world");

Cherry::PopFont();
```

---

### PopFont

Pop the current font from the stack.

```cpp
void Cherry::PopFont();
```

Example:

```cpp
Cherry::PopFont();
```

---

## Runtime

### GetPath

Get a runtime path.

```cpp
std::string Cherry::GetPath(const std::string& path);
```

Parameters:
- `path` : Relative runtime path.

Returns:
- The resolved runtime path.

Example:

```cpp
std::string fullPath = Cherry::GetPath("assets/logo.png");
```

---

### GetLocale

Get a localized string from a topic.

```cpp
std::string Cherry::GetLocale(const std::string& topic);
```

Parameters:
- `topic` : Localization topic key.

Returns:
- The localized string.

Example:

```cpp
std::string title = Cherry::GetLocale("app.title");
```

---

## Networking

These functions are only available if `CHERRY_ENABLE_NET` is enabled.

You can enable it in CMake:

```cmake
set(CHERRY_ENABLE_NET ON)
```

---

### GetHttpPath

Fetch content from an HTTP URL.

```cpp
std::string Cherry::GetHttpPath(const std::string& url);
```

Parameters:
- `url` : HTTP or HTTPS URL.

Returns:
- The downloaded content as a string.

Example:

```cpp
std::string content =
    Cherry::GetHttpPath("https://example.com/data.json");
```

---

### ShutdownHttpFetcher

Shutdown the HTTP fetcher system.

```cpp
void Cherry::ShutdownHttpFetcher();
```

Example:

```cpp
Cherry::ShutdownHttpFetcher();
```

---

### InitHttpFetcher

Initialize the HTTP fetcher system.

```cpp
void Cherry::InitHttpFetcher();
```

Example:

```cpp
Cherry::InitHttpFetcher();
```

---

## Data

These functions can return JSON string data or legacy string data.

---

### GetData

Get data from an `Identifier` and topic.

```cpp
std::string Cherry::GetData(
    const Identifier& id,
    const std::string topic
);
```

Parameters:
- `id` : Identifier to search data from.
- `topic` : Data topic key.

Returns:
- The stored data string.

Example:

```cpp
std::string data =
    Cherry::GetData(myIdentifier, "config");
```

---

### GetWindowData

Get data attached to a window.

```cpp
std::string Cherry::GetWindowData(
    const std::string& id,
    const std::string topic
);
```

Parameters:
- `id` : Window identifier.
- `topic` : Data topic key.

Returns:
- The stored data string.

Example:

```cpp
std::string settings =
    Cherry::GetWindowData("MainWindow", "settings");
```

---

## Properties

### PushPermanentProperty

Push a permanent property.

```cpp
void Cherry::PushPermanentProperty(
    const std::string& property,
    const std::string& value
);
```

Parameters:
- `property` : Property name.
- `value` : Property value.

Example:

```cpp
Cherry::PushPermanentProperty("color", "red");
```

---

### PopPermanentProperty

Pop one or multiple permanent properties.

```cpp
void Cherry::PopPermanentProperty(
    int number_of_pops = 0
);
```

Parameters:
- `number_of_pops` : Number of properties to pop.

Example:

```cpp
Cherry::PopPermanentProperty(1);
```

---

### SetNextComponentProperty

Set a property for the next component.

```cpp
void Cherry::SetNextComponentProperty(
    const std::string& property,
    const std::string& value
);
```

Parameters:
- `property` : Property name.
- `value` : Property value.

Example:

```cpp
Cherry::SetNextComponentProperty("padding", "10");
```

---

## Components

### PushParentComponent

Push a parent component into the component stack.

```cpp
void Cherry::PushParentComponent(
    const std::shared_ptr<Component>& component
);
```

Parameters:
- `component` : Parent component to push.

Example:

```cpp
Cherry::PushParentComponent(component);
```

---

### PopParentComponent

Pop the current parent component.

```cpp
void Cherry::PopParentComponent();
```

Example:

```cpp
Cherry::PopParentComponent();
```

---

### GetParent

Get a parent component from the stack.

```cpp
std::shared_ptr<Component> Cherry::GetParent(
    int parent_number = 0
);
```

Parameters:
- `parent_number` : Parent stack index.

Returns:
- The matching parent component.

Example:

```cpp
auto parent = Cherry::GetParent();
```

---

## Runtime State

### IsReady

Check if Cherry is initialized and ready.

```cpp
bool Cherry::IsReady();
```

Returns:
- `true` if Cherry is ready.

Example:

```cpp
if (Cherry::IsReady())
{
    // Ready
}
```

---

### GetDeltaTime

Get frame delta time.

```cpp
float Cherry::GetDeltaTime();
```

Returns:
- Delta time between frames.

Example:

```cpp
float delta = Cherry::GetDeltaTime();
```

---

### GetTime

Get elapsed runtime time.

```cpp
float Cherry::GetTime();
```

Returns:
- Elapsed runtime time.

Example:

```cpp
float time = Cherry::GetTime();
```

---

## Colors

### HslaHex

Convert HSLA values to a hexadecimal color string.

```cpp
std::string Cherry::HslaHex(
    float h,
    float s,
    float l,
    float a = 1.0f
);
```

Parameters:
- `h` : Hue value in degrees.
- `s` : Saturation value between `0.0f` and `1.0f`.
- `l` : Lightness value between `0.0f` and `1.0f`.
- `a` : Alpha value between `0.0f` and `1.0f`.

Returns:
- The generated hexadecimal color string.

Example:

```cpp
std::string color =
    Cherry::HslaHex(
        240.0f,
        1.0f,
        0.5f
    );
```

---

### ParseHexSafe

Safely parse a hexadecimal color string into a `uint32_t`.

```cpp
uint32_t Cherry::ParseHexSafe(std::string hex);
```

Parameters:
- `hex` : Hexadecimal color string.

Returns:
- Parsed color as `uint32_t`.

Example:

```cpp
uint32_t color =
    Cherry::ParseHexSafe("#FF0000FF");
```

---

### HexToImU32

Convert a hexadecimal color string into an `ImU32`.

```cpp
ImU32 Cherry::HexToImU32(std::string hex);
```

Parameters:
- `hex` : Hexadecimal color string.

Returns:
- Converted `ImU32` color.

Example:

```cpp
ImU32 color =
    Cherry::HexToImU32("#00FF00FF");
```

---

### HexToRGBA

Convert a hexadecimal color string into an `ImVec4`.

```cpp
ImVec4 Cherry::HexToRGBA(std::string hex);
```

Parameters:
- `hex` : Hexadecimal color string.

Returns:
- Converted RGBA color as `ImVec4`.

Example:

```cpp
ImVec4 color =
    Cherry::HexToRGBA("#FFFFFFFF");
```

---

### HexToImColor

Convert a hexadecimal color string into an `ImColor`.

```cpp
ImColor Cherry::HexToImColor(std::string hex);
```

Parameters:
- `hex` : Hexadecimal color string.

Returns:
- Converted `ImColor`.

Example:

```cpp
ImColor color =
    Cherry::HexToImColor("#FFAA00FF");
```

---

### ImU32ToHex

Convert an `ImU32` color into a hexadecimal string.

```cpp
std::string Cherry::ImU32ToHex(ImU32 color);
```

Parameters:
- `color` : `ImU32` color value.

Returns:
- Hexadecimal color string.

Example:

```cpp
std::string hex =
    Cherry::ImU32ToHex(
        IM_COL32(255, 0, 0, 255)
    );
```

---

## Logging

### Log::SetLevel

Set the minimum log level displayed by the logger.

```cpp
void Cherry::Log::SetLevel(Level level);
```

Parameters:
- `level` : Minimum log level.

Available levels:
- `Cherry::Log::Level::Trace`
- `Cherry::Log::Level::Info`
- `Cherry::Log::Level::Warn`
- `Cherry::Log::Level::Error`
- `Cherry::Log::Level::Fatal`

Example:

```cpp
Cherry::Log::SetLevel(
    Cherry::Log::Level::Warn
);
```

---

### Log::AddCallback

Add a custom logging callback.

```cpp
void Cherry::Log::AddCallback(LogCallback cb);
```

Parameters:
- `cb` : Callback function executed on every log message.

Example:

```cpp
Cherry::Log::AddCallback(
    [](Cherry::Log::Level level,
       const std::string& message)
    {
        std::cout << message << std::endl;
    }
);
```

---

### Log::Print

Print a formatted log message.

```cpp
void Cherry::Log::Print(
    Level level,
    std::string_view tag,
    const char* fmt,
    ...
);
```

Parameters:
- `level` : Log severity level.
- `tag` : Log category tag.
- `fmt` : Formatted message.
- `...` : Format arguments.

Example:

```cpp
Cherry::Log::Print(
    Cherry::Log::Level::Info,
    "Renderer",
    "Loaded %d textures",
    12
);
```

---

## Logging Macros

### CH_TRACE

Print a trace log message.

```cpp
CH_TRACE(...);
```

Example:

```cpp
CH_TRACE("Initializing renderer");
```

---

### CH_INFO

Print an info log message.

```cpp
CH_INFO(...);
```

Example:

```cpp
CH_INFO("Application started");
```

---

### CH_WARN

Print a warning log message.

```cpp
CH_WARN(...);
```

Example:

```cpp
CH_WARN("Missing configuration file");
```

---

### CH_ERROR

Print an error log message.

```cpp
CH_ERROR(...);
```

Example:

```cpp
CH_ERROR("Failed to load texture");
```

---

### CH_FATAL

Print a fatal log message.

```cpp
CH_FATAL(...);
```

Example:

```cpp
CH_FATAL("Critical runtime error");
```

---

## Type Safety

Utility functions used to safely convert strings into primitive types without throwing exceptions. These are mainly used internally by Cherry components when dealing with unsafe or external data sources.

---

### SafeInt

Safely convert a `std::string` to an `int`.

```cpp
int Cherry::SafeInt(
    const std::string& s,
    int def = 0
);
```

Parameters:
- `s` : Input string to convert.
- `def` : Default value returned if conversion fails or string is empty.

Returns:
- Converted integer value, or `def` on failure.

Example:

```cpp
int value = Cherry::SafeInt("42", 0);
```

---

### SafeFloat

Safely convert a `std::string` to a `float`.

```cpp
float Cherry::SafeFloat(
    const std::string& s,
    float def = 0.f
);
```

Parameters:
- `s` : Input string to convert.
- `def` : Default value returned if conversion fails or string is empty.

Returns:
- Converted float value, or `def` on failure.

Example:

```cpp
float value = Cherry::SafeFloat("3.14", 0.f);
```

---

### SafeBool

Safely convert a `std::string` to a `bool`.

```cpp
bool Cherry::SafeBool(
    const std::string& s,
    bool def = false
);
```

Parameters:
- `s` : Input string to convert.
- `def` : Default value returned if string is not `"true"` or `"false"`.

Returns:
- `true` if string equals `"true"`.
- `false` if string equals `"false"`.
- Otherwise returns `def`.

Example:

```cpp
bool value = Cherry::SafeBool("true", false);
```

---

