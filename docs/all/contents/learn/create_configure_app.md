This section explains how to create and configure your application. You’ll learn how to set it up, choose a rendering mode, customize the window, and more.

### Creating an Application
To create a Cherry application, two main steps are required:
- Include the `CherryRun` macro in your `main` function to start the Cherry runtime.
- Define a `CherryMain` function, which will act as the entry point for your application logic.

##### Add Cherry to your runtime 

<banner type="note">To integrate Cherry into your project in a more advanced way—such as embedding it into an existing rendering pipeline or using it on the web—please refer to the [Advanced Purposes of Cherry]() section.</banner>

To add cherry to your application runtime, include the main Cherry header into your application :
```cpp
#include "cherry/cherry.hpp"
// 
```

Then, you need to start Cherry using the `CherryRun` runtime function. Cherry and all rendering will run independently on a dedicated thread.

```cpp
int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}
```
##### Create App
Once Cherry has been added to your runtime, all that’s left is to define the environment creation function using `CherryMain`.

```cpp
CherryApplication CherryMain(int argc, char **argv) {
  CherryAppConfig config;
  config.SetRenderMode(Cherry::WindowRenderingMethod::SimpleRender);
  return new CherryApplication(config);
}
```
And that’s it! Cherry is now included and will create its own environment.

### Configure application
You can configure your application within the application creation function we just saw. Let’s now take a look at the available configuration options for `CherryAppConfig config;`.

##### Sizes
Cherry allows you to configure and set the default sizes for your application.

```cpp
// This function will set the maximum width of your application to 500 pixels.
config.SetMinimumWidth(500);
```
```cpp
// This function will set the maximum height of your application to 250 pixels.
config.SetMinimumHeight(250);
```
```cpp
// This function will set the initial height to 400 pixels.
config.SetDefaultHeight(400);
```
```cpp
// This function will set the initial width to 800 pixels.
config.SetDefaultWidth(800);
```

##### Scales
Cherry allows you to set the default scaling factor for your applications, especially for zooming graphical elements and fonts.

```cpp
// This function will add 25% zoom to the overall rendering.
config.SetGlobalScale(1.25f);
```
```cpp
// This function will add 15% zoom to the font rendering.
config.SetFontGlobalScale(1.15f);
```

##### Titlebar
Cherry allows you to customize and configure several parts and properties of the TitleBar.

First, you can choose to show or hide the desktop environment’s title bar using `DisableDesktopTitleBar`.

<banner type="warning">Removing the title bar will also remove the close, maximize, and minimize buttons of your app, so make sure to handle these controls yourself.</banner>

```cpp
// This function will remove the title bar from your desktop environment.
config.DisableDesktopTitleBar();
```

##### Custom Titlebar
You can also choose to display Cherry’s custom title bar and customize it more extensively using `SetCustomTitlebar(bool)`.

```cpp
// This function will enable the use of a customizable title bar with maximize, minimize, and close buttons.
config.SetCustomTitlebar(true);
```

**Tip :**
```cpp
// Using these two functions simply replaces your system’s title bar with Cherry’s fully customizable title bar.
config.DisableDesktopTitleBar();
config.SetCustomTitlebar(true);
```

In your custom title bar, you can also choose to display only the close button without showing the maximize and minimize buttons by setting the `WindowOnlyClosable` option.
```cpp
// This option will make sure that only the close button is displayed.
config.WindowOnlyClosable = true;
```

##### App icons
Cherry allows you to customize your application's icons.

You can personalize the application icon shown in your desktop environment using the function `SetFavIconPath(string)`.
```cpp
// This function will define the displayed favicon picture path.
config.SetFavIconPath(Cherry::GetPath("resources/imgs/icon.png"));
```

You can also customize the internal logo of your Cherry app, which Cherry might display in a custom taskbar or menubar, for example.
```cpp
// This function will define the displayed main application icon picture path.
config.SetIconPath(Cherry::GetPath("resources/imgs/icon.png"));
// 
```

##### Locales
Cherry allows you to load translation files into your application to create a multilingual app.

The `AddLocale()` function lets you define a translation language by adding its file along with the translation name. You can then use these translations throughout your application via tags.
```cpp
// This function will add a translation file, identified by the tag "en" in this example.
CherryApp.AddLocale("en", Cherry::GetPath("resources/locales/en.json"));
// 
```

This is what the translation file looks like :
###### en.json
```json
{
    "locales": [ 
        {"loc.menubar.menu.vortex":"Vortex"},
        {"loc.menubar.menu.options":"Options"},
        {"loc.menubar.summary.general":"General"},
        {"loc.menubar.summary.vortex_labs":"Vortex Labs"},
        {"loc.menubar.summary.manage":"Manage"},
        {"loc.menubar.summary.other":"Other"},
    ]
}
```
> In this example, calling Cherry::GetLocale("loc.menubar.menu.manage") will return "Manage" string.

<banner type="note">To learn more about translations and locales with Cherry, please refer to the [translation section]()</banner>

The `SetDefaultLocale()` function has two effects: it sets the default language and also specifies which file to use if a translation is missing in another language.

```cpp
// This function will define the default locale file to use.
CherryApp.SetDefaultLocale("en");
```

The `SetLocale()` function sets the translation file to use (the file must have been added via `AddLocale`). If the translations defined in this file are missing or invalid, Cherry will fall back to the default file specified with `SetDefaultLocale`.
```cpp
// This function sets which translation file to use. If a requested translation is missing, Cherry will look it up in the default file provided. 
CherryApp.SetLocale("en");
// 
```

##### Render mode
Application configuration allows you to select the rendering mode. To learn more about the different rendering modes offered by Cherry, please refer to [Rendering & Paradigms]().

To select a rendering mode, you can directly call the `SetRenderMode(RenderingMethod)` function.

```cpp
// This function sets the display and rendering mode of the window to Docking mode
config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);
// You can then add multiple App Windows that will be displayed within a viewport.
```
```cpp
// This function sets the display and rendering mode of the window to Simple Render mode
config.SetRenderMode(Cherry::WindowRenderingMethod::SimpleRender);
// You can then add a simple rendering function.
```
```cpp
// This function sets the display and rendering mode of the window to Simple Window mode
config.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);
// You can then add a single, simple App Window to render.
```
```cpp
// This function sets the display and rendering mode of the window to Tab Windows mode
config.SetRenderMode(Cherry::WindowRenderingMethod::TabWidows);
// You can then add multiple App Windows that will be displayed in a viewport without docking.
```

##### Themes
To add a theme available throughout your application, call the `AddTheme(Theme)` function. The theme will be added to your application and available in all layers.

```cpp
// This function will add the "DarkVortex" theme to your application.
config.AddTheme(CherryThemes::DarkVortex());
```

You can also customize the theme's tag to give it a personalized and easier-to-remember name.

```cpp
// This function will add the "DarkVortex" theme to your application with a custom tag.
config.AddTheme(CherryThemes::DarkVortex(), "my_custom_theme");
```

---

You can select a default theme by calling `SetDefaultTheme(string)` and passing the tag of the theme you want to set as default.

```cpp
// This function will set the theme "my_custom_theme" as the default for the entire application.
config.SetDefaultTheme("my_custom_theme");
```
##### Fonts
Cherry allows you to define fonts that will be accessible in your application using the `AddFont(string, string)` function.
```cpp
// This function adds the font consola.ttf, accessible via the name "Consola".
CherryApp.AddFont("Consola", Cherry::GetPath("resources/fonts/consola.ttf"), 20f);
```

##### Window Callbacks
Cherry allows you to define certain window behaviors via callbacks, letting you, for example, specify what happens when the user clicks the close button.

The function `SetCloseCallback(std::function<bool()>)` lets you define the application’s behavior when the user clicks the close button. This way, you can close other parts of the app or only close the specific app windows in docking mode.

```cpp
// This function will define the behavior when the user presses the application's close button; in this case, it will only hide an app window.
config.SetCloseCallback([this](){m_AppWindow->SetVisibility(false);});
```

<banner type="warning">By default, pressing the close button will simply close the application. There is no need to use `SetCloseCallback` if you want this same behavior—Cherry handles it automatically.</banner>

##### Audio config
Cherry allows you to configure and enable audio services using the `UseAudio()` function.

<banner type="warning">You must compile Cherry with the `CHERRY_ENABLE_AUDIO` parameter set to ON in your build configuration file.</banner>

```cpp
// Enable audio services
config.UseAudio();
```

### Compilation
[The compilation guide](https://i.n:content_name=learn&section=all&page_name=compilation) will help you compile and configure the build of your projects.