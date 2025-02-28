# Welcome to Cherry !
Cherry is a modern, minimalist yet comprehensive UI/UX framework for low-level native applications. It provides a complete solution, including backend support, a rendering engine, and UI components. Cherry is built using technologies like ImGui, Vulkan, and SDL, offering developers advanced tools to create efficient and high-performance interfaces.

The Cherry Framework is free, independant and open-source writted with modern C/C++ code respecting the best practices possibles in terms of memory safety, threads safety and best looking/quality code. The official repository is here, maintained by the Infinite project and the community https://github.com/infiniteHQ/Cherry .

For the history, Cherry was originally made for [Vortex](https://vortex.infinite.si/), the open creation platform of [Infinite](https://infinite.si/), a platform allowing creators to bring their own ideas to life, create systems, applications and services easely, with taking care of important values like ethical tech, security, ecological and healthy tech. Today, Cherry is a pretty big project helping people to create advanced like simple modern interfaces, used by numerous projects of many natures like games, debuggers, editors and more ! 

## Getting started
*This section will be explain you how to get Cherry and how to run your first build.*

> [!WARNING]  
> The only dependance you need to compile Cherry is the VulkanSDK installed & ready. Please refer to the https://www.lunarg.com/vulkan-sdk/ website to see how install Vulkan on your system (compatible on Linux & Windows).


> 🪟 On **Microsoft Windows**
> ```
> git clone https://github.com/infiniteHQ/Cherry
> cd Cherry
> git submodule update --init --recursive
> cd example/hello_world
> build.bat
> ```


> 🐧 On **Linux** based distributions
> ```
> git clone https://github.com/infiniteHQ/Cherry
> cd Cherry
> git submodule update --init --recursive
> cd example/hello_world
> bash build.sh
> ```

## Start to use Cherry
*If everything is ok during your first build, you be able now to develop your entier projects with Cherry !!! Let's enumerate all the ressources you have to learn and start to use cherry.*

- [Main documentation]()
- [API Reference]()
- [Components Reference]()
- [Templates Reference *(aio)*]()


## Hello World example
*This is a little example of how you can make your first hello world in C++*

```cpp
#include "cherry.hpp"

void Render()
{
  CherryKit::TitleThree("Features");
}

Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  Cherry::ApplicationSpecification config;
  config.SetName("Hello Cherry");                                   
  config.SetMinimumWidth(100);     
  config.SetMinimumHeight(100);  
  config.SetDefaultWidth(800);  
  config.SetDefaultHeight(500);  
  config.SetGlobalScale(0.84f);
  config.SetFontGlobalScale(0.84f); 
  config.SetFavIconPath(Cherry::GetPath("resources/imgs/icon.png"));
  config.SetRenderMode(WindowRenderingMethod::SimpleRender); 

  return new Cherry::Application(config);
}

int main(int argc, char *argv[])
{
  std::thread([&](){ Cherry::Main(argc, argv); }).detach();

  while (true) 
  {
    if (!Cherry::IsRunning())
    return 0;

    if (!Cherry::IsReady())
    continue;
  }
}
```

## Let's review this example
- Firstly, you always need to add the only header you need to runs cherry.
```cpp
#include "cherry.hpp"
```

---
- Then, you can create for example a little render function displaying "Hello World !" in a title component.
- You need to call the ```Cherry::CreateApplication(int argc, char **argv)``` function to create a instance of Cherry app, this is the entry point of the Cherry rendering process. In this example, we set the minium height and width the window will not be capable of resizing less of theses values, we also set the defaut size and the global zooming scale. Adding a favicon from our assets folder. Also set the render mode to a simple function rendered (our ```Render()``` function set just after). 

```cpp
void Render()
{
  CherryKit::TitleThree("Hello world !");
}

Cherry::Application *Cherry::CreateApplication(int argc, char **argv)
{
  Cherry::ApplicationSpecification config;
  config.SetName("Hello Cherry");                                   
  config.SetMinimumWidth(100);     
  config.SetMinimumHeight(100);  
  config.SetDefaultWidth(800);  
  config.SetDefaultHeight(500);  
  config.SetGlobalScale(0.84f);
  config.SetFontGlobalScale(0.84f); 
  config.SetFavIconPath(Cherry::GetPath("resources/imgs/icon.png"));
  config.SetRenderMode(WindowRenderingMethod::SimpleRender); 
  config.SetMainRenderCallback(Render);                      

  return new Cherry::Application(config);
}
```
---
- Finally, in the main function of your little example we just need to call the main function of cherry (where you want to bootstrapp Cherry), and the interface will be running in a separate thread !

```cpp
int main(int argc, char *argv[])
{
  // The Cherry runtime, detached from the main thread
  std::thread([&](){ Cherry::Main(argc, argv); }).detach();

  // Our main programm...
  while (true) 
  {
    if (!Cherry::IsRunning())
    return 0;

    if (!Cherry::IsReady())
    continue;
  }
}
```