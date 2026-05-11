Cherry allows you to create and manage color and property themes for your application. These themes are directly applied to components, windows, and app windows.  

You can choose not to manage themes manually, in that case, Cherry will automatically assign a default theme.  

You can also create or load incomplete themes; Cherry will always fall back to the default or base complete theme to fill in any missing values.

### Add a theme
To add a theme to your application, simply call the `AddTheme()` function.  

This function is available through the `CherryApp` variable, or directly within the configuration block defined inside the `CreateApplication` function at the root of your project.

###### Add a theme from main application config

```cpp
CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  // Other configs...
  config.AddTheme(CherryThemes::Dark());

  // You can also define your custom names
  config.AddTheme(CherryThemes::Dark(), "my_theme");
  // Other configs...
  return new CherryApplication(config);
}
```

###### Add a theme from app runtime
```cpp
  CherryApp.AddTheme(CherryThemes::Dark());
```

### Use themes
You can define the default theme at three different levels: the global application level, a specific window, or an app window.

<banner type="note">
The default theme acts as a fallback—components and layers will refer to it when a specified theme does not include a particular property.
</banner>


###### Define the default app theme from CreateApplication
```cpp
CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  // Other configs...
  config.SetDefaultTheme("dark");
  // Other configs...
  return new CherryApplication(config);
}
```
You can also define specific themes for each layer of your application.

###### Set the theme of entire App
```cpp
CherryApp.SetTheme("light");
```
###### Set the theme of a Window
```cpp
CherryWindow.SetTheme("light");
```
###### Set the theme of a App Window
```cpp
CherryAppWindow.SetTheme("light");
```

<banner type="note">
The priority of which theme to use flows from the highest level (components & app window) down to the lowest level (the app).
</banner>

You can also assign a theme to a specific part of your application, so that only the elements rendered within that area are affected by the selected theme.

###### Set the theme of a App Window
```cpp
CherryAppWindow.SetTheme("light");

// Use the pushed dark theme
CherryApp.PushTheme("dark");
CherryKit::TitleSimple("Super text");
CherryApp.PopTheme();

// Use the default light theme
CherryKit::TitleSimple("Other super text");
```

### Create themes
Of course, you can create your own custom themes or import ones from the community. Here is how to create your themes:

<banner type="note">
Your theme can be incomplete without any problem; Cherry will look for missing properties in the default or base theme.
</banner>

```cpp
namespace Cherry {
namespace Themes {
class Light : public Theme {
public:
  Light() {
    SetName("light");

    // In our theme, we can add color properties
    SetProperty("color_header", "#EAEAEA");
    SetProperty("color_header_hovered", "#DCDCDC");
    SetProperty("color_header_active", "#D0D0D0");

    // We can also add other properties
    // Such as size, margin, padding, etc...
    SetProperty("block_border_radius", std::to_string(0.0f));
    SetProperty("block_border_size", std::to_string(1.0f));
  }
}
} // namespace Themes
} // namespace Cherry
  ```