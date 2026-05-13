Cherry can show advanced dockspaces with multiple context-sharing abilities, giving you the possibility to create complex applications with multiple windows and dockspaces that can share ImGui windows between each other.

This is a short video showing the Docking features of Cherry (from [Vortex](https://vortex.infinite.si/)):

<img width="800" height="450" alt="advanced_docking" src="https://static.infinite.si/cherrydocs/1.6/all/media/advanced_docking.gif" />

To enable docking, set the RenderMode of Cherry to DockingWindows. Then, every app window of Cherry will be docked into a Dockspace.
```cpp
CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);

  return new CherryApplication(config);
}
```