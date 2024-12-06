cmake_minimum_required(VERSION 3.16)
project(libcherry)

# Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
set(GCC_COVERAGE_COMPILE_FLAGS "")

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -fPIC")
endif()

# Find Vulkan on the system
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# Add libraries
add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/stb-image)
add_subdirectory(lib/sdl2)

# Add source files
file(
    GLOB_RECURSE
    CHERRY_SRC
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/platform/*.h" "${PROJECT_SOURCE_DIR}/platform/*.hpp" "${PROJECT_SOURCE_DIR}/platform/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/components/*.h" "${PROJECT_SOURCE_DIR}/components/*.hpp" "${PROJECT_SOURCE_DIR}/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/kit/*.h" "${PROJECT_SOURCE_DIR}/kit/*.hpp" "${PROJECT_SOURCE_DIR}/kit/*.cpp"
)

# Create cherry as a shared library
add_library(cherry SHARED ${CHERRY_SRC})

# Linking
target_link_libraries(cherry PRIVATE Vulkan::Vulkan)
target_link_libraries(cherry PRIVATE imgui)
target_link_libraries(cherry PRIVATE glm)
target_link_libraries(cherry PRIVATE stbimage)
target_link_libraries(cherry PUBLIC spdlog)
target_link_libraries(cherry PUBLIC SDL2)

target_include_directories(cherry PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(cherry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb_image")
