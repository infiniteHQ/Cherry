cmake_minimum_required(VERSION 3.16)
project(libcherry)

set(CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
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

file(GLOB_RECURSE CHERRY_SRC CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/platform/*.h" "${PROJECT_SOURCE_DIR}/platform/*.hpp" "${PROJECT_SOURCE_DIR}/platform/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/components/*.h" "${PROJECT_SOURCE_DIR}/components/*.hpp" "${PROJECT_SOURCE_DIR}/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/kit/*.h" "${PROJECT_SOURCE_DIR}/kit/*.hpp" "${PROJECT_SOURCE_DIR}/kit/*.cpp"
)

add_library(cherry SHARED ${CHERRY_SRC})

target_link_libraries(cherry PRIVATE Vulkan::Vulkan)
target_link_libraries(cherry PUBLIC SDL2)
target_link_libraries(cherry PRIVATE imgui)
target_link_libraries(cherry PRIVATE glm)
target_link_libraries(cherry PRIVATE stbimage)
target_link_libraries(cherry PUBLIC spdlog)

target_include_directories(cherry PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(cherry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb_image")


if(CHERRY_CEF)
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/cef-project")

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
target_link_libraries(cherry PUBLIC ${CEF_EXTRA_LIBRARIES})

file(GLOB CEF_RESOURCES
    "${CEF_RELEASE_DIR}/icudtl.dat"
    "${CEF_RELEASE_DIR}/*.pak"
    "${CEF_RELEASE_DIR}/*.bin"
    "${CEF_RELEASE_DIR}/*.json"
)
file(COPY ${CEF_RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/Resources")
endif()