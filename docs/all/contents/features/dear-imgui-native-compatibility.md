
Cherry has native compatibility with [Dear ImGui](https://github.com/ocornut/imgui)! That means you can directly add Dear ImGui code into your Cherry application rendering, and it will work just perfectly.

``` cpp
// Rendered inside a Cherry application
// All the Dear ImGui native code will work natively !
void Render() {
  
  static bool show_demo_window = true;
  static bool show_another_window = false;
  static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  static ImGuiIO &io = ImGui::GetIO();
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");  
    ImGui::Text("This is some useful text.");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float *)&clear_color);
    
    if (ImGui::Button("Button")) 
      counter++;

    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
  }

  if (show_another_window) {
    ImGui::Begin("Another Window", &show_another_window);  
    
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
      show_another_window = false;
    ImGui::End();
  }
}
```

Result :

<picture>
  <img width=500 src="https://static.infinite.si/cherrydocs/1.6/all/media/nativeimgui.png">
</picture>

> In fact, Cherry uses [Dear ImGui](https://github.com/ocornut/imgui) to render interfaces internally. We added many abstractions to allow you to create interfaces without Dear ImGui, just with simple high-level features, components, or widgets. But it is highly modular.
