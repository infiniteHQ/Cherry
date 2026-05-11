This section explains how to compile and configure the build of your Cherry application. This step is crucial as it allows you to select which Cherry components to include or exclude in order to optimize your build.

### Create cmake file
The first step is to create the `CMakeLists.txt` file in your current directory.

###### Linux
```bash
touch CMakeLists.txt
```
###### Microsoft Windows
```bash
copy NUL CMakeLists.txt
```

##### Adding our project
Once created, we will add the basic configurations for our project.
```cpp 
cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
```

We create a project named "hello," specify that we are using the C++20 standard, and define the build directory for our project.

##### Configure the build
We will now add some configurations to create a precise build that includes only what we need.
To do this, we can directly use CMake options that will be applied to our project.
Here is the list of all available configurations:

###### Build options
```cpp
# CHERRY_STATIC will compile our library as static if ON, and as dynamic if OFF
option(CHERRY_STATIC "Link Cherry statically" ON) # Advanced usage only: keep this ON to link Cherry statically
```
```cpp
# CHERRY_DEBUG will add diagnostic and debug tools if ON
option(CHERRY_DEBUG "Enable Cherry debug tools" OFF)
```
```cpp
# CHERRY_PACKAGE will package all assets and libraries into a bundle if ON
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller" OFF)
```

###### Options & Addons
```cpp
# If ON, this will enable networking features in Cherry (e.g., HTTP asset fetching)
option(CHERRY_ENABLE_NET "Enable net features" OFF)
```

```cpp
option(CHERRY_ENABLE_AUDIO "Enable audio features" OFF)
```
```cpp
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager" OFF)
```

###### Optimizations
Cherry allows you to remove certain parts to optimize the build.
<banner type="warning"> some components are essential for specific Cherry features.
Only disable options if you fully understand their impact!</banner>

```cpp
option(CHERRY_DISABLE_NODES "Disable nodal systems" OFF)
```
```cpp
option(CHERRY_DISABLE_WORKSPACESAVES "Disable workspace saves" OFF)
```


### Conclusion 
Here is what a minimal CMake file should look like:

```cpp 
cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build options
option(CHERRY_STATIC "Link Cherry statically" ON) # Advanced usage only: keep this ON to link Cherry statically
option(CHERRY_DEBUG "Enable Cherry debug tools" OFF)
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller" OFF)

# Cherry Options & Addons
option(CHERRY_ENABLE_NET "Enable net features" OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features" OFF)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager" OFF)

# Cherry Optimizations
option(CHERRY_DISABLE_NODES "Disable nodal systems" OFF)
option(CHERRY_DISABLE_WORKSPACESAVES "Disable workspace saves" OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../ cherry_build)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE cherry)
```

### Adding asset folder
If you use assets for Cherry, you can manage building your project with its assets folder by adding these lines:

```cpp
add_custom_command(TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)
```

This will result in copying your project's assets folder into the build directory!