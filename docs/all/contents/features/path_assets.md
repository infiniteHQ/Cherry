Cherry allows you to access various API functionalities that can help you manage content and different tools alongside your executable.

### GetPath
The `GetPath` function will return the path of the folder containing your application executable.

If your executable is located at `/home/you/test/yourprogram`, the `GetPath` function will return `/home/you/test/`. This way, you can add content, such as resource files, and package them with your executable. This makes it easy to include content like images, sounds, translations, and fonts alongside your program.

```cpp
// Cherry will look for the file "tick.wav" in a
// "resources" folder located near the executable
CherryApp.PlaySound(CherryPath("resources/audio/tick.wav"));
```

### Add resources folder
We will now see how to add a resource file and compile it with your executable.

First, create an `assets` folder and a `resources` folder inside it.

```cmd
mkdir assets
```
```cmd
mkdir assets/resources
```

Next, add in your build file the retrieval of the contents of the `assets` folder, which CMake will place close to your executable.

```c
add_custom_command(TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)
 ```

Here are the lines to add in your `CMakeLists.txt`. During compilation, the contents of the `assets` folder will be placed next to your executable.

###### Example

Here is an example of a `CMakeLists.txt`:

```c
cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(CHERRY_STATIC "Link Cherry statically" ON)
option(CHERRY_DEBUG "Enable Cherry debug tools" OFF)
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller" OFF)
option(CHERRY_ENABLE_NET "Enable net features" OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features" ON)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager" OFF)
option(CHERRY_DISABLE_NODES "Disable nodal systems" OFF)
option(CHERRY_DISABLE_WORKSPACESAVES "Disable workspace saves" OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../ cherry_build)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE cherry)

add_custom_command(TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)
 
```

Before compilation :
```txt
assets/
assets/resources
main.cpp
CMakeLists.txt
```

Once compiled, here is the content of the build folder:

```txt
bin/
bin/hello # programm
bin/resources
```

In conclusion, calling `Cherry::GetPath("resources/")` will give you access to the content of your assets located in the resources folder.

<banner type="note">To view and test the implementation of Paths & Assets, check out our [simple asset management example](https://github.com/infiniteHQ/Cherry/tree/main/examples/concepts/assets_simple) </banner>
