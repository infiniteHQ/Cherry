cmake_minimum_required(VERSION 3.16)
project(libcherry)

set(CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Include Vulkan
find_package(Vulkan REQUIRED)

# Include cef-cmake configuration
if(CHERRY_CEF)
include("${CMAKE_CURRENT_SOURCE_DIR}/lib/cef_cmake/cmake/cef_cmake.cmake")
endif()

# Add subdirectories for dependencies
add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/stb-image)
add_subdirectory(lib/sdl2)

if(CHERRY_CEF)
add_subdirectory(lib/cef_cmake)
endif()

# Source files
file(GLOB_RECURSE CHERRY_SRC CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/platform/*.h" "${PROJECT_SOURCE_DIR}/platform/*.hpp" "${PROJECT_SOURCE_DIR}/platform/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/components/*.h" "${PROJECT_SOURCE_DIR}/components/*.hpp" "${PROJECT_SOURCE_DIR}/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/kit/*.h" "${PROJECT_SOURCE_DIR}/kit/*.hpp" "${PROJECT_SOURCE_DIR}/kit/*.cpp"
)

# Create shared library target
add_library(cherry SHARED ${CHERRY_SRC})

# Link dependencies
target_link_libraries(cherry PRIVATE Vulkan::Vulkan)
target_link_libraries(cherry PUBLIC SDL2)
target_link_libraries(cherry PRIVATE imgui)
target_link_libraries(cherry PRIVATE glm)
target_link_libraries(cherry PRIVATE stbimage)
target_link_libraries(cherry PRIVATE spdlog)

if(CHERRY_CEF)
target_link_libraries(cherry PUBLIC cefdll_wrapper)
endif()

# Include directories
target_include_directories(cherry PUBLIC "${Vulkan_INCLUDE_DIRS}")
target_include_directories(cherry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb_image")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/spdlog/include")

# Optional features
if(CHERRY_NET)
    find_package(CURL REQUIRED)
    target_compile_definitions(cherry PUBLIC CHERRY_NET)
    target_link_libraries(cherry PRIVATE CURL::libcurl)
    target_include_directories(cherry PRIVATE ${CURL_INCLUDE_DIRS})
endif()
