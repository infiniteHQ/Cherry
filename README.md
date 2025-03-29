

<a href="https://infinite.si">
  <h1 align="center">
    <picture>
      <source media="(prefers-color-scheme: dark)" srcset="./.github/imgs/banner.png">
      <img src="./.github/imgs/banner.png">
    </picture>
  </h1>
</a>

<div align="center">
<a title="Code Quality" href="https://www.codefactor.io/repository/github/infinitehq/uikit"><img alt="Code Quality" src="https://img.shields.io/codefactor/grade/github/infinitehq/uikit?longCache=true&style=for-the-badge&label=Code%20Quality&logoColor=fff&logo=CodeFactor&branch=master"></a>
  <a title="Discord Server" href="https://discord.gg/H2wptkecUg"><img alt="Discord Server" src="https://img.shields.io/discord/1095333825762046194?label=Discord&logo=Discord&logoColor=fff&style=for-the-badge"></a>
<a title="'Linux Build' workflow Status" href="https://img.shields.io/github/actions/workflow/status/infiniteHQ/uikit/build.yml"><img alt="Linux Build" src="https://img.shields.io/github/actions/workflow/status/infiniteHQ/uikit/build.yml?longCache=true&style=for-the-badge&label=Build&logoColor=fff&logo=GitHub%20Actions&branch=main"></a>
</div>

> *Important: Cherry is part of [Infinite](https://infinite.si/)'s initiative to create free, independent, and ethical technology and computing. Cherry is a library powering many parts of this project, but maintaining libraries like Cherry is a time-consuming and costly process. If you appreciate Cherry, you can support us [on our funding page](https://fund.infinite.si/) if you can.*

### Summary
*Cherry is a modern, minimalist yet comprehensive UI/UX framework for low-level native applications. It provides a complete solution, including backend support, a rendering engine, and UI components. Cherry is built using technologies like ImGui, Vulkan, and SDL, offering developers advanced tools to create efficient and high-performance interfaces.*

*A presentation video will be apear here*

## What you can do ?

|                         | Status       |
|--------------------------------|-------------|
| **Desktop Applications**       | ✅ Ready    |
| **Debug Tools & Utilities**    | ✅ Ready    |
| **Web Tech-Based Apps**        | 🧪 Experimental |
| **Embedded Systems & UIs**     | ⏳ Planned  |
| **Web Apps**                   | ⏳ Planned  |
| **Mobile Apps**                | ⏳ Planned  |
| **UI in Games & Renders**      | ⏳ Planned  |

##### Legend  
✅ : *Ready*  
🧪 : *Experimental*  
⏳ : *Planned*  

## Compatibility

| Technology | Windows | Linux | macOS | iOS | Android | Web |
|-----------|---------|-------|-------|-----|---------|-----|
| **Vulkan**  | ✅ | ✅ | ❌ | ❌ | ❌ | ❌ |
| **OpenGL**  | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| **WebGL**   | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |

##### Legend  
✅ *: Fully compatible*    
🟧 *: Not fully tested, partially incompatible*   
❌ *: Not planned, incompatible*  


## Features

> **Components & Kits**
> - 🧰 Many single components ready to use with data managment
> - 📦 Many all in one windows, ready to use and manage
> - 📦 ImGui abstractions for simplicity
> - 📦 SDL abstractions for inputs & user managment
> - 📦 Short inlines components

> **Render engine**
> - 📈 Data handler for windows & components
> - 💬 Localization & Traduction managment
> - 🎨 Themes manager, Dark & Ligth themes
> - ❓ Fonts manager
> - 🖼️ Images & textures manager
> - 🗂️ Menubar & Bottombar simple callbacks
> - 💪 Custom window close behaviors
> - 🗄️ Advanced Docking (WIP)
> - 🖼️ Toasts, Notifications & pop-ups
> - 🛠️ Behaviros callbacks (closing, moving, etc...)
> - 🏗️ Windows customisations (size, colors, properties & styles)

> **Backends**
> - 🛠️ Multi context with contexts cohesion & communication (ImGui/SDL)
> - 🛠️ Complete image load & show, cache & optimization
> - 🛠️ Window builder


> **Planned features for next updates :**
> - 🏗️ Webviews & Website loading
> - 🏗️ More components with more properties & customizations
> - 🏗️ Internet images loading & cache managment


> [!NOTE]  
> This framework is new and still under active development, so some bugs may occur, and content is subject to change. We are also working to improve the overall stability of Cherry, particularly with regard to memory safety and the rendering stack, as well as the graphical libraries used. Thank you for your understanding! Stay tuned :)

## Getting started
> [!WARNING]  
> The only dependance you need to compile Cherry is the VulkanSDK installed & ready. Please refer to the https://www.lunarg.com/vulkan-sdk/ website to see how install Vulkan on your system (compatible on Linux & Windows).


> 🪟 On **Microsoft Windows**
> ```
> git clone https://github.com/infiniteHQ/Cherry
> cd Cherry
> git submodule update --init --recursive
> cd example/demo
> build.bat
> ```


> 🐧 On **Linux** based distributions
> ```
> git clone https://github.com/infiniteHQ/Cherry
> cd Cherry
> git submodule update --init --recursive
> cd example/demo
> bash build.sh
> ```

## Examples & Preview
  
> [!NOTE]  
> These examples are real-world examples of the use of Cherry. More examples will be uploaded.
<table>
  <tr>
    <td align="center">
      <img src="./.github/imgs/Cherry1.png" alt="Cherry 1" width="250">
      <p>Simple window with sidebar</p>
    </td>
    <td align="center">
      <img src="./.github/imgs/Cherry2.png" alt="Cherry 2" width="250">
      <p>Toast notification</p>
    </td>
    <td align="center">
      <img src="./.github/imgs/Cherry3.png" alt="Cherry 3" width="250">
      <p>Rich menubar</p>
    </td>
  </tr>
  <tr>
    <td align="center">
      <img src="./.github/imgs/Cherry4.png" alt="Cherry 4" width="250">
      <p>Project selector with custom widgets</p>
    </td>
    <td align="center">
      <img src="./.github/imgs/Cherry5.png" alt="Cherry 5" width="250">
      <p>Item selection & navigation</p>
    </td>
    <td align="center">
      <img src="./.github/imgs/Cherry6.png" alt="Cherry 6" width="250">
      <p>Rich windows & Docking</p>
    </td>
  </tr>
  <tr>
    <td align="center">
      <img src="./.github/imgs/Cherry7.png" alt="Cherry 7" width="250">
      <p>Multi-windows on single app</p>
    </td>
  </tr>
</table>

## Special thanks
- For all contributors of SDL2
- For all contributors of Vulkan/VulkanSDK
- For all contributors of STB
- For the log library of gabime and contributors (https://github.com/gabime/spdlog)
- For the json library of nlohmann and contributors (https://github.com/nlohmann/json)
- For the amazing immediate mode interface ImGui from Ocornut and contributors (https://github.com/ocornut/imgui)
- For the work of from "Studio Cherno" on Walnut (https://github.com/StudioCherno/Walnut)
