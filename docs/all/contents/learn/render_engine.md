The render engine is one of the most important parts of Cherry. It's the rendering orchestrator, deciding what gets rendered and where.

### Understand Multicontext
Additionally, the Render Engine manages multicontext between ImGui, Vulkan, and SDL. It can simultaneously render multiple windows from different contexts and enable communication between them using user data, navigation data, and other shared information.

### Layers
Rendering in Cherry is divided into three main layers: the `App`, the `Window`, and the `AppWindow`.

#### App
The `App` is the lowest layer. It includes the windowing context and serves as the foundation for everything built on top of it. An application can manage one or multiple windows and several app windows rendered within those windows. The App also contains translations, themes, fonts, and all other graphical information. Typically, there is only one App per executable/application, although more can be used in advanced setups.

#### Window
The `Window` is the second layer in Cherry applications. It directly hosts the visible render output and handles logic and user interaction. A Cherry window is essentially an independent OS window. In minimal configurations using `SimpleRender`, there is typically only one window, but in more advanced rendering modes like `TabWindow` or `DockingWindow`, there can be several. Each Cherry window maps to a desktop environment window. Multiple windows with various styles can exist within a single application.

If the configuration requires it, the App may display a menubar or additional interactive buttons.

#### AppWindow
The `AppWindow` is the highest rendering layer. It is rendered on top of the `Window` and there can be multiple `AppWindows` per window. An `AppWindow` can also contain other `AppWindows`.

Example: In an application using the `DockingWindow` rendering mode, the App may contain one or more Windows, each of which contains one or more AppWindows. The `AppWindows` are the components where the user’s rendering code is implemented.

### Dynamic Variables
The API gives you access to three dynamic variables that provide a reference to the desired rendering layer. This allows you to easily access the functions and API of a specific layer:

```cpp
// Will give you an access to the application API
CherryApp.
// Will give you an access to the current rendered window API
CherryWindow.
// Will give you an access to the current rendered appwindow API
CherryAppWindow.
```