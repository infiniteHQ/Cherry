cmake_minimum_required(VERSION 3.16)
project(libcherry LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(CHERRY_STATIC "Link Cherry statically"                           OFF)
option(CHERRY_DEBUG "Enable Cherry debug tools"                         OFF)

option(CHERRY_IMGUI_EXPORT "Disable workspace saves"                    OFF)
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller"      OFF)
option(CHERRY_MULTIAPP "Use multiple apps in one executable"            OFF)

option(CHERRY_ENABLE_SHADERS "Enable vulkan shaders system"             OFF)
option(CHERRY_ENABLE_NET "Enable net features"                          OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features"                      OFF)
option(CHERRY_ENABLE_KIT "Enable base component kit"                     ON)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager"   OFF)
option(CHERRY_ENABLE_NODES "Disable nodal systems"                      OFF)
option(CHERRY_ENABLE_WORKSPACESAVES "Disable workspace saves"           OFF)


# ---------------------------------------------------------------------------
# Compiler flags
# ---------------------------------------------------------------------------
if (NOT MSVC)
    add_compile_options(-O3 -fPIC)
else()
    add_compile_options(/utf-8)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_output)
if(lto_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# ---------------------------------------------------------------------------
# Library type
# ---------------------------------------------------------------------------
if(CHERRY_STATIC)
    set(CHERRY_LIB_TYPE STATIC)
else()
    set(CHERRY_LIB_TYPE SHARED)
endif()

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)
add_subdirectory(lib/stb-image)

if(CHERRY_STATIC)
set(SDL2_STATIC ON CACHE BOOL "Build SDL2 as static lib" FORCE)
endif()
add_subdirectory(lib/sdl2)

if(CHERRY_ENABLE_NET)
    add_subdirectory(lib/httpcl)
endif()

if(CHERRY_ENABLE_AUDIO)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/miniaudio miniaudio_build)
endif()

# ---------------------------------------------------------------------------
# Cherry source files
# ---------------------------------------------------------------------------
file(GLOB_RECURSE CHERRY_SRC CONFIGURE_DEPENDS
    "platform/*.[ch]pp" "platform/*.c*" 
    "src/*.[ch]pp" "src/*.c*"
    "components/*.[ch]pp" "components/*.c*"
    "kit/*.[ch]pp" "kit/*.c*"
)

add_library(cherry ${CHERRY_LIB_TYPE} ${CHERRY_SRC})

if(CHERRY_ENABLE_LINUXDRM)
target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_LINUXDRM)
endif()


# ---------------------------------------------------------------------------
# DLL export/import macros
# ---------------------------------------------------------------------------
if(CHERRY_STATIC)
    target_compile_definitions(cherry PUBLIC CHERRY_STATIC_LIB)
else()
    target_compile_definitions(cherry PRIVATE CHERRY_EXPORT_SYM)
    target_compile_definitions(cherry PUBLIC CHERRY_SHARED)
endif()

# ---------------------------------------------------------------------------
# Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(cherry PRIVATE Vulkan::Vulkan glm stbimage)

# ---------------------------------------------------------------------------
# ImGui export/import macros
# ---------------------------------------------------------------------------
if(CHERRY_IMGUI_EXPORT)
    if(CHERRY_STATIC)
        target_compile_definitions(imgui PUBLIC IMGUI_STATIC_LIB)
    else()
        target_compile_definitions(imgui PRIVATE IMGUI_EXPORT_SYM)
        target_compile_definitions(imgui PUBLIC IMGUI_SHARED)
    endif()
endif()
target_link_libraries(cherry PUBLIC imgui)

target_link_libraries(cherry PUBLIC SDL2-static)

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
target_include_directories(cherry PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/lib/stb_image"
    "${Vulkan_INCLUDE_DIRS}"
)

# ---------------------------------------------------------------------------
# Optional features
# ---------------------------------------------------------------------------
if(CHERRY_ENABLE_AUDIO)
    target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_AUDIO)
    target_link_libraries(cherry PUBLIC miniaudio)
endif()

if(CHERRY_MULTIAPP)
    target_compile_definitions(cherry PUBLIC CHERRY_MULTIAPP)
endif()

if(CHERRY_ENABLE_KIT)
    target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_KIT)
endif()

if(CHERRY_ENABLE_NET)
    target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_NET)
    target_link_libraries(cherry PUBLIC httpcl)
    if(WIN32)
        target_link_libraries(cherry PUBLIC winhttp)
    endif()
endif()