cmake_minimum_required(VERSION 3.16)
project(libcherry LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(CHERRY_STATIC "Build cherry as a static library" OFF)
option(CHERRY_CEF "Enable CEF support" OFF)
option(CHERRY_ENABLE_NET "Enable networking" OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio" OFF)
option(CHERRY_ENABLE_WEBVIEW "Enable webview" OFF)

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

if(CHERRY_STATIC)
    set(CHERRY_LIB_TYPE STATIC)
    add_definitions(-DCHERRY_STATIC_LIB)
else()
    set(CHERRY_LIB_TYPE SHARED)
    add_definitions(-DCHERRY_SHARED)
endif()

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/stb-image)
set(SDL2_STATIC ON CACHE BOOL "Build SDL2 as static lib" FORCE)
add_subdirectory(lib/sdl2)

if(CHERRY_CEF)
    add_subdirectory(lib/cef-project)
endif()

if(CHERRY_ENABLE_NET)
    add_subdirectory(lib/httpcl)
endif()

if(CHERRY_ENABLE_AUDIO)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/miniaudio miniaudio_build)
endif()

# Cherry source files
file(GLOB_RECURSE CHERRY_SRC CONFIGURE_DEPENDS
    "platform/*.[ch]pp" "platform/*.c*" 
    "src/*.[ch]pp" "src/*.c*"
    "components/*.[ch]pp" "components/*.c*"
    "kit/*.[ch]pp" "kit/*.c*"
)

add_library(cherry ${CHERRY_LIB_TYPE} ${CHERRY_SRC})

set_target_properties(cherry PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_link_libraries(cherry PRIVATE Vulkan::Vulkan glm stbimage)
target_link_libraries(cherry PUBLIC imgui)

if(CHERRY_STATIC)
    set(CHERRY_LIB_TYPE STATIC)
    add_definitions(-DCHERRY_STATIC_LIB)
    target_link_libraries(cherry PUBLIC SDL2-static)
else()
    set(CHERRY_LIB_TYPE SHARED)
    add_definitions(-DCHERRY_SHARED)
    target_link_libraries(cherry PUBLIC SDL2)
endif()

# Includes publics
target_include_directories(cherry PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/lib/stb_image"
    "${PROJECT_SOURCE_DIR}/lib/spdlog/include"
    "${Vulkan_INCLUDE_DIRS}"
)

# Audio
if(CHERRY_ENABLE_AUDIO)
    target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_AUDIO)
    target_link_libraries(cherry PUBLIC miniaudio)
endif()

# Réseau
if(CHERRY_ENABLE_NET)
    target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_NET)
    target_link_libraries(cherry PUBLIC httpcl)
    if(WIN32)
        target_link_libraries(cherry PUBLIC winhttp)
    endif()
endif()

# CEF
if(CHERRY_CEF)
    target_compile_definitions(cherry PUBLIC CHERRY_CEF)
    target_include_directories(cherry PUBLIC ${CEF_ROOT})

    set(CEF_RELEASE_DIR "${CMAKE_BINARY_DIR}/Release")
    if(WIN32)
        set(CEF_LIBRARIES "${CEF_RELEASE_DIR}/libcef.lib")
        set(CEF_EXTRA_LIBRARIES
            "${CEF_RELEASE_DIR}/libEGL.dll"
            "${CEF_RELEASE_DIR}/libGLESv2.dll"
            "${CEF_RELEASE_DIR}/vk_swiftshader.dll"
            "${CEF_RELEASE_DIR}/vulkan-1.dll"
        )
    else()
        set(CEF_LIBRARIES "${CEF_RELEASE_DIR}/libcef.so")
        set(CEF_EXTRA_LIBRARIES
            "${CEF_RELEASE_DIR}/libEGL.so"
            "${CEF_RELEASE_DIR}/libGLESv2.so"
            "${CEF_RELEASE_DIR}/libvk_swiftshader.so"
            "${CEF_RELEASE_DIR}/libvulkan.so.1"
        )
    endif()

    target_link_libraries(cherry PUBLIC ${CEF_LIBRARIES} libcef_dll_wrapper)

    file(GLOB CEF_RESOURCES
        "${CEF_RELEASE_DIR}/icudtl.dat"
        "${CEF_RELEASE_DIR}/*.pak"
        "${CEF_RELEASE_DIR}/*.bin"
        "${CEF_RELEASE_DIR}/*.json"
    )
    file(COPY ${CEF_RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/Resources")
endif()
