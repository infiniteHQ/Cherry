cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build options
option(CHERRY_STATIC "Link Cherry statically"                            ON)
option(CHERRY_DEBUG "Enable Cherry debug tools"                         OFF)

option(CHERRY_IMGUI_EXPORT "Disable workspace saves"                    OFF)
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller"      OFF)
option(CHERRY_MULTIAPP "Use multiple apps in one executable"            OFF)

option(CHERRY_ENABLE_SHADERS "Enable vulkan shaders system"             OFF)
option(CHERRY_ENABLE_NET "Enable net features"                          OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features"                      OFF)
option(CHERRY_ENABLE_KIT "Enable base component kit"                    OFF)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager"   OFF)
option(CHERRY_ENABLE_NODES "Disable nodal systems"                      OFF)
option(CHERRY_ENABLE_WORKSPACESAVES "Disable workspace saves"           OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ cherry_build)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE cherry)