Cherry gives you the possibility to choose what you want to use. If you don't want the markdown renderer, audio engine, or sound. You can disable them. By default, nothing is enabled; you need to enable each feature you want directly from the `CMakeLists.txt` file of your project.

By default, only the minimal render engine and some basic utilities are included. Cherry is not bloated by default.

`This is how you can configure your custom Cherry into your projects :`
```cpp
# ---------------------------------------------------------------------------
# Cherry UI Library
# ---------------------------------------------------------------------------
option(CHERRY_STATIC "Link Cherry statically"                           OFF)
option(CHERRY_DEBUG "Enable Cherry debug tools"                          ON)

option(CHERRY_IMGUI_EXPORT "Disable workspace saves"                     ON)
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller"      OFF)
option(CHERRY_MULTIAPP "Use multiple apps in one executable"             ON)

option(CHERRY_ENABLE_NET "Enable net features"                          OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features"                      OFF)
option(CHERRY_ENABLE_MARKDOWN "Enable markdown parsing and render"       ON)
option(CHERRY_ENABLE_SCRIPTING "Enable script engine"                    ON)
option(CHERRY_ENABLE_KIT "Enable base component kit"                     ON)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager"   OFF)
option(CHERRY_ENABLE_NODES "Enable nodal systems"                       OFF)
option(CHERRY_ENABLE_WORKSPACESAVES "Enable workspace saves"            OFF)
option(CHERRY_ENABLE_COMPUTE_SHADERS "Enable compute shaders"           OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cherry cherry_build)
```
