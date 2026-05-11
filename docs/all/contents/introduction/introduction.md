### Welcome to Cherry !
Cherry is a modern, powerfull and simple C++/Lua library for building cross-platform application or cool user interfaces.

Whether you’re a beginner or an experienced developer, Cherry makes it easy to create real, efficient, and professional interfaces.

### Main goals
Cherry philosophy is to provide a powerfull library to make applications and interfaces, it's avoids heavy web technologies and bloated solutions, focusing instead on being simple yet complete, powered by ImGui, Vulkan, and SDL.
Whether you’re a beginner or an experienced developer, Cherry makes it easy to create real, efficient, and professional interfaces.


### Use cases and Compatibility

<h4>Use cases :</h4>

- Cherry is perfect to create tools with a complete interface quickly.
- Cherry is capable of making advanced and complexe user interfaces for applications or softwares.

 
### Compatibility

<table style="width:100%;">
  <tr>
    <th>Technology</th>
    <th>Windows</th>
    <th>Linux</th>
    <th>Linux DRM</th>
    <th>macOS</th>
    <th>iOS</th>
    <th>Android</th>
    <th>Web</th>
  </tr>
  <tr>
    <td><strong>Vulkan</strong></td>
    <td>✅</td>
    <td>✅</td>
    <td>🟧</td>
    <td>✅</td>
    <td>❌</td>
    <td>❌</td>
    <td>❌</td>
  </tr>
</table>

<p style="font-size: 12px;">
✅ <em>: Fully compatible</em><br>
🟧 <em>: Not fully tested, partially incompatible but planned</em><br>
❌ <em>: Not planned, incompatible (for the moment...)</em>
</p>


### Choices and style
Cherry follows [the Infinite C++ style convention](https://github.com/infiniteHQ/InfiniteCodingStyle-cpp) for coding style. Concerning naming, we wanted to keep Dear ImGui's conventions (PascalCase for functions and methods), primarily to maintain consistency across UI codebases and avoid making ImGui-style coding more complex.


### History
Cherry was originally created for [Vortex](https://vortex.infinite.si/), our platform dedicated to hacking, creation, DIY project and giving healthy and ethical tools for creators.

Everything from one need : having a reactive, advanced, and efficient interface. To accomplish this, we spend a lot of time trying many technologies and liked a lot the [Immediate Mode GUI concept](https://en.wikipedia.org/wiki/Immediate_mode_(computer_graphics)#Immediate_mode_GUI), implemented by the amazing [Dear ImGui library, from ocornut](https://github.com/ocornut/imgui).

Immediate mode is really great but can be tricky and not fully adapted with ui components, retained/static renders, using and manipulate object components and basically when we want to create a complete application.

The ideal would be to take the best of immediate mode and classic static retained renders, like classical applications. Well, Cherry was created for that !

Now, Cherry is a big project helping people to create advanced like simple modern interfaces, used by numerous projects of many natures like games, debuggers, editors and more !

### Special Thanks

- For all contributors of SDL2
- For all contributors of Vulkan/VulkanSDK
- For all contributors of STB
- For the log library of gabime and contributors (https://github.com/gabime/spdlog)
- For the json library of nlohmann and contributors (https://github.com/nlohmann/json)
- For the amazing immediate mode interface ImGui from Ocornut and contributors (https://github.com/ocornut/imgui)
- For the work of from "Studio Cherno" on Walnut (https://github.com/StudioCherno/Walnut)
