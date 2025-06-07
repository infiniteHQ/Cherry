cmake_minimum_required(VERSION 3.16)
project(libcherry)

set(CMAKE_CXX_STANDARD 20)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3")
endif()

if (MSVC)
    add_compile_options(/utf-8)
endif()


if(CHERRY_CEF)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/MT)
endif()
endif()

find_package(Vulkan REQUIRED)

add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/stb-image)
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

file(GLOB_RECURSE CHERRY_SRC CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/platform/*.h" "${PROJECT_SOURCE_DIR}/platform/*.hpp" "${PROJECT_SOURCE_DIR}/platform/*.cpp" "${PROJECT_SOURCE_DIR}/platform/*.cc"
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/components/*.h" "${PROJECT_SOURCE_DIR}/components/*.hpp" "${PROJECT_SOURCE_DIR}/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/kit/*.h" "${PROJECT_SOURCE_DIR}/kit/*.hpp" "${PROJECT_SOURCE_DIR}/kit/*.cpp"
)

add_library(cherry SHARED ${CHERRY_SRC})

set_target_properties(cherry PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_link_libraries(cherry PRIVATE Vulkan::Vulkan)
target_link_libraries(cherry PUBLIC SDL2)
target_link_libraries(cherry PUBLIC imgui)
target_link_libraries(cherry PRIVATE glm)
target_link_libraries(cherry PRIVATE stbimage)
target_link_libraries(cherry PRIVATE spdlog)
if(CHERRY_ENABLE_WEBVIEW)

endif()


target_include_directories(cherry PUBLIC "${Vulkan_INCLUDE_DIRS}")
target_include_directories(cherry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb_image")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/spdlog/include")

if(CHERRY_ENABLE_AUDIO)
    target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_AUDIO)
    target_link_libraries(cherry PUBLIC miniaudio)
endif()

if(CHERRY_ENABLE_NET)
    target_compile_definitions(cherry PUBLIC CHERRY_ENABLE_NET)
    target_link_libraries(cherry PUBLIC httpcl)

    if(WIN32)
        target_link_libraries(cherry PUBLIC winhttp)
    endif()
endif()


if(CHERRY_CEF)
  
if(MSVC)
target_include_directories(cherry PUBLIC ${CEF_ROOT})
target_compile_definitions(cherry PUBLIC CHERRY_CEF)

set(CEF_RELEASE_DIR "${CMAKE_BINARY_DIR}/Release")
set(CEF_LIBRARIES "${CEF_RELEASE_DIR}/libcef.lib")
target_link_libraries(cherry PUBLIC ${CEF_LIBRARIES})

message(WARNING "RUNTIME : ${CEF_LIBRARIES}")

set(CEF_EXTRA_LIBRARIES
    "${CEF_RELEASE_DIR}/libEGL.dll"
    "${CEF_RELEASE_DIR}/libGLESv2.dll"
    "${CEF_RELEASE_DIR}/vk_swiftshader.dll"
    "${CEF_RELEASE_DIR}/vulkan-1.dll"
)
target_link_libraries(cherry PUBLIC libcef_dll_wrapper)

file(GLOB CEF_RESOURCES
    "${CEF_RELEASE_DIR}/icudtl.dat"
    "${CEF_RELEASE_DIR}/*.pak"
    "${CEF_RELEASE_DIR}/*.bin"
    "${CEF_RELEASE_DIR}/*.json"
)
file(COPY ${CEF_RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/Resources")
else()
target_include_directories(cherry PUBLIC ${CEF_ROOT})
target_compile_definitions(cherry PUBLIC CHERRY_CEF)

set(CEF_RELEASE_DIR "${CMAKE_BINARY_DIR}/Release")
set(CEF_LIBRARIES "${CEF_RELEASE_DIR}/libcef.so")
target_link_libraries(cherry PUBLIC ${CEF_LIBRARIES})

set(CEF_EXTRA_LIBRARIES
    "${CEF_RELEASE_DIR}/libEGL.so"
    "${CEF_RELEASE_DIR}/libGLESv2.so"
    "${CEF_RELEASE_DIR}/libvk_swiftshader.so"
    "${CEF_RELEASE_DIR}/libvulkan.so.1"
)
target_link_libraries(cherry PUBLIC libcef_dll_wrapper)

file(GLOB CEF_RESOURCES
    "${CEF_RELEASE_DIR}/icudtl.dat"
    "${CEF_RELEASE_DIR}/*.pak"
    "${CEF_RELEASE_DIR}/*.bin"
    "${CEF_RELEASE_DIR}/*.json"
)
file(COPY ${CEF_RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/Resources")
endif()
endif()