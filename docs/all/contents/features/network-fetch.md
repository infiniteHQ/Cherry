
#### HTTP fetches

There are built-in network features to fetch files from URLs, primarily for the purpose of downloading images from the web.

```cpp
void Render() {
  // Net components from the Cherry Kit
  CherryKit::ImageHttp("https://infinite.si/assets/in_projects.c64028e7.png");

  // Or fetch as Dear ImGui texture directly
  auto texture = Cherry::GetTexture(Cherry::GetHttpPath("https://infinite.si/assets/in_projects.c64028e7.png"));
  CherryGUI::Image(texture, 240.0f, 150.0f);
}
```

Result :

<picture>
  <img width=380 src="https://static.infinite.si/cherrydocs/1.6/all/media/netfetch.png">
</picture>

> Cherry will automaticly put images, medias or http files into a cache in temporary folder to avoid making too many fetches and preserve resources.

### Notice for Microsoft Windows users

The **net** feature of Cherry requires **CURL**. You must have CURL installed and visible to CMake to build the project with net features.

#### Recommended Setup (vcpkg)

1. **Install CURL**:
   Open a terminal in your `vcpkg` directory and run:
```powershell
vcpkg install curl:x64-windows
```

3. Configure Environment Variable:
To allow CMake and the build.bat script to find CURL without modifying the project files, set a system environment variable:

Variable Name: CMAKE_TOOLCHAIN_FILE

Variable Value: [path to vcpkg]/vcpkg/scripts/buildsystems/vcpkg.cmake

3. Clean and Build:
If you have already attempted to build, delete the build/ folder to clear the CMake cache and try rebooting. Then, run your build.bat script again.