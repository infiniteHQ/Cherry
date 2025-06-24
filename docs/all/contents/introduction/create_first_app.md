Once you have installed and configured your environment with Cherry, we will see how to create your first application with Cherry!

 <banner type="note">For this example, we will base our first application on the [Cherry example project](https://github.com/infiniteHQ/CherryDesktopTemplate) that you can use directly as a starting point for your own project.</banner>

### Add Cherry
There are several ways to add Cherry to your project.

#### Git submodules
You can directly add Cherry using a git submodule, in a folder like '/lib', for example.

``` bash
mkdir lib
```

``` bash
git submodule add https://github.com/infiniteHQ/Cherry lib/cherry
```
<banner type="warning">From now on, we will assume that Cherry is located in `[project path]/lib/cherry`. Make sure to adjust this example if needed.</banner>

### Add main source

We will now add an entry source and include Cherry in it.

```bash
mkdir src
```

```bash
touch src/main.cpp
```

 Let’s now edit `main.cpp` to start our application:

```cpp
#include "../../cherry.hpp"

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}
```

Here is a base to connect to the Cherry runtime. `CherryRun` is a macro that contains the starter for the Cherry thread. However, you still need to add the application creation function so that Cherry can properly create its runtime using a configuration.

```cpp
CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

    // All configs here...

  return new CherryApplication(config);
}
```

Add this function to our `main.cpp`, anywhere. There’s no need to call the function: Cherry will automatically find it to create its instance.

We will now create a simple render function to display something.

```cpp
void Render(){
    CherryKit::TitleOne("Hello World");
}
```

This simple render function calls a graphical component from the component kit.

We will now add some useful configurations from the [available configurations](https://i.n:content_name=learn&page_name=create_configure_app&section=all) directly in our create application function.


```cpp
CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

   // We will start by defining the main render function of our application.
   config.SetMainRenderCallback(Render);

   // Then, we will define the window size of our application.
   config.SetDefaultHeight(900);
   config.SetDefaultWidth(900);


  return new CherryApplication(config);
}
```

### Create compilation file
Now, let's create a compilation file with cmake

```bash
touch CMakeLists.txt
```

We will first define our hello project and specify the C++ version as well as the build location.

```cpp 
cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
```

We will then add the basic Cherry configurations.

```javascript
# Build options
option(CHERRY_STATIC "Link Cherry statically" ON) # Advanced usage only: keep this ON to link Cherry statically
option(CHERRY_DEBUG "Enable Cherry debug tools" OFF)
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller" OFF)

# Cherry Options & Addons
option(CHERRY_ENABLE_NET "Enable net features" OFF)
option(CHERRY_ENABLE_CEF "Enable chrome embedded framwork (CEF)" OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features" OFF)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager" OFF)

# Cherry Optimizations
option(CHERRY_DISABLE_NODES "Disable nodal systems" OFF)
option(CHERRY_DISABLE_WORKSPACESAVES "Disable workspace saves" OFF)
```

Finally, we will create the executable and link our Cherry library to it:

```cpp 
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../ cherry_build)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE cherry)
```

### Create a compilation script
You can now create build scripts to simplify the build process.

###### For linux
For Linux, we will create a ```build.sh```ile as follows:

```sh
#!/bin/sh
rm build/example
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc) install
cd ..
```
###### For Microsoft Windows
 For Windows, we will create a ```build.bat``` script as follows:

```bat
@echo off
setlocal enabledelayedexpansion

mkdir build
cd build

cmake -G "Visual Studio 17" -A x64 ..

for /f %%i in ('powershell -command "(Get-WmiObject -Class Win32_Processor).NumberOfLogicalProcessors"') do set THREADS=%%i

cmake --build . --config Release -- /m:%THREADS%
xcopy /E /Y /I bin\Release\* bin\
rmdir /S /Q bin\Release

endlocal
```

### Conclusion

###### main.cpp
```cpp
#include "../../cherry.hpp"

void Render(){
    CherryKit::TitleOne("Hello World");
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

   // We will start by defining the main render function of our application.
   config.SetMainRenderCallback(Render);

   // Then, we will define the window size of our application.
   config.SetDefaultHeight(900);
   config.SetDefaultWidth(900);


  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}
```

###### CMakeLists.txt

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
option(CHERRY_ENABLE_CEF "Enable chrome embedded framwork (CEF)" OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features" OFF)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager" OFF)

# Cherry Optimizations
option(CHERRY_DISABLE_NODES "Disable nodal systems" OFF)
option(CHERRY_DISABLE_WORKSPACESAVES "Disable workspace saves" OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../ cherry_build)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE cherry)
```
###### build.sh
```sh
#!/bin/sh
rm build/example
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc) install
cd ..
```
###### build.bat
```bat
@echo off
setlocal enabledelayedexpansion

mkdir build
cd build

cmake -G "Visual Studio 17" -A x64 ..

for /f %%i in ('powershell -command "(Get-WmiObject -Class Win32_Processor).NumberOfLogicalProcessors"') do set THREADS=%%i

cmake --build . --config Release -- /m:%THREADS%
xcopy /E /Y /I bin\Release\* bin\
rmdir /S /Q bin\Release

endlocal
```

### Let's compile !
All that’s left is to compile!

#### On Linux

```sh
bash build.sh
```

#### On Windows
```sh
build.bat
```
### Last step, test it !
We now just need to run the compiled executable.

#### On Linux
```bash
bash ./build/bin/hello
```

#### On Windows
```bash
.\build\bin\hello.exe
```

And if everything works well, our window must be opened.

### Whats now ?

Congrats you just finished a simple example, you can access directly by this [repository](https://github.com/infiniteHQ/CherryDesktopTemplate).

Now you can learn more about the [Cherry API]() or learn and find components and themes on the [CherryKit]()