

The entire context, windows, and rendering methods are fully customizable. You can control the behavior of a window, and define whether you want the native desktop environment title bar or your own custom one.

While creating your application main implementation, you can define the sizes, behaviors, default states, customizations, settings, etc...

For more informations, please [check how to create and configure your application](https://cherry.infinite.si/docpage?content_name=understand_cherry&page_name=create_configure_app&version=1.6&section=all)

```cpp
CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  // Render mode (4 differents render modes availables, including docking, simple rendering, windowed rendering)
  config.SetRenderMode(Cherry::WindowRenderingMethod::SimpleRender);

  // Setup default and minimum window sizes
  config.SetMinimumHeight(200);
  config.SetMinimumWidth(100);
  config.SetDefaultHeight(450);
  config.SetDefaultWidth(450);

  // Custom bar customizations
  config.SetCustomTitlebar(true);
  config.DisableDesktopTitleBar();

  // Set rendering scales
  config.SetGlobalScale(1.25f);
  config.SetFontGlobalScale(1.15f);

  // Icon functions
  config.SetFavIconPath(Cherry::GetPath("resources/imgs/icon.png"));
  config.SetIconPath(Cherry::GetPath("resources/imgs/icon.png"));
  
  // Themes functions
  config.AddTheme(CherryThemes::DarkVortex());

  // Enable audio services
  config.UseAudio();


  return new CherryApplication(config);
}
```

You can call theses configurations for the main application window, or even additional window.

For example, this is how create another window from your first one after starting your application :

```cpp
if (CherryKit::ButtonText("Spawn window with custom titlebar").GetDataAs<bool>("isClicked")) {
    auto ap = CherryKit::WindowSimple("Custom titlebar", RenderCustomTitleBar);
    Cherry::ApplicationSpecification spec;
    spec.SetName("Custom titlebar");
    spec.SetRenderMode(Cherry::WindowRenderingMethod::SimpleWindow);
    spec.SetUniqueAppWindowName("Custom titlebar");
    spec.SetMinimumHeight(200);
    spec.SetMinimumWidth(100);
    spec.SetDefaultHeight(450);
    spec.SetDefaultWidth(450);
    spec.SetCustomTitlebar(true);
    spec.DisableWindowManagerTitleBar = true;
    ap->AttachOnNewWindow(spec);
    Cherry::AddAppWindow(ap);
}
```