<a href="https://infinite.si">
  <h1 align="center">
    <picture>
      <source media="(prefers-color-scheme: dark)" srcset="./.github/imgs/banner.png">
      <img height="250px" src="./.github/imgs/banner.png">
    </picture>
  </h1>
</a>

  <a title="'Build' workflow Status" href="https://github.com/infiniteHQ/uikit/actions?query=workflow%3Abuild"><img alt="'Build' workflow Status" src="https://img.shields.io/github/actions/workflow/status/infiniteHQ/uikit/.github/workflows/build.yml?longCache=true&style=for-the-badge&label=Build&logoColor=fff&logo=GitHub%20Actions&branch=master"></a>
<a title="Code Quality" href="https://www.codefactor.io/repository/github/infinitehq/uikit"><img alt="Code Quality" src="https://img.shields.io/codefactor/grade/github/infinitehq/uikit?longCache=true&style=for-the-badge&label=Code%20Quality&logoColor=fff&logo=CodeFactor&branch=master"></a>


This is a simple & low level UI Kit for moderns low level applications running with a graphical user interface.
Used by the Vortex Creation platform.

## What this library can do
![interface](/.github/imgs/interface.png)

## Buinding example
```
cd example
mkdir build && cd build
cmake ..
make
```

## Usage
```cpp
#include "../uikit.h"
#include "../src/EntryPoint.h"

#include <thread>
#include <memory>

class Layer : public UIKit::Layer
{
public:
  Layer(){};

  virtual void OnUIRender() override
  {
    // The render channel
    ImGui::ShowDemoWindow();
  }
};

UIKit::Application *UIKit::CreateApplication(int argc, char **argv)
{
  UIKit::ApplicationSpecification spec;
  std::shared_ptr<Layer> layer = std::make_shared<Layer>();
  
  std::string name = "UIKit example";
  spec.Name = name;
  spec.CustomTitlebar = true;

  UIKit::Application *app = new UIKit::Application(spec);

  app->PushLayer(layer);
  app->SetMenubarCallback([app, layer]()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                app->Close();
            }
            ImGui::EndMenu();
        }
   });

  return app;
}

int main(int argc, char *argv[])
{
    std::thread mainthread;
    std::thread Thread([&]() { VortexMaker::VortexInterface(argc, argv); });
    mainthread.swap(Thread);

    while(true){
        /*Your program loop...*/
    }

    return 0;
}
```



## Features
- Immediate rendering with advanced layer management.
- Custom widgets
- ImGui mods & addons.

## Special thanks
- The amazing immediate mode interface ImGui from Ocornut (https://github.com/ocornut/imgui)
- A ImGui all in one base from "Studio Cherno" (https://github.com/StudioCherno/Walnut)