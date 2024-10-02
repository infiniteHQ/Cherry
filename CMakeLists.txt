cmake_minimum_required(VERSION 3.16)
project(libuikit)

# ========= setup ================
set(CMAKE_CXX_STANDARD 17)

# for cmake modules if u want to add some libraries
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# flags for gcc or clang
set(GCC_COVERAGE_COMPILE_FLAGS "")

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
endif()
# ===============================

# ========= deps ================
# Find Vulkan
find_package(Vulkan REQUIRED)

# glm
add_subdirectory(lib/glm)

# imgui
add_subdirectory(lib/imgui)

# spdlog
add_subdirectory(lib/spdlog)

# stb-image
add_subdirectory(lib/stb-image)

add_subdirectory(lib/sdl2)

# ===============================

# ========== uikit =============
# find all source files and headers
file(
    GLOB_RECURSE # recursive
    UIKIT_SRC # variable to store the source files and headers
    CONFIGURE_DEPENDS # make a dependency
    "${PROJECT_SOURCE_DIR}/platform/*.h" "${PROJECT_SOURCE_DIR}/platform/*.hpp" "${PROJECT_SOURCE_DIR}/platform/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/components/*.h" "${PROJECT_SOURCE_DIR}/components/*.hpp" "${PROJECT_SOURCE_DIR}/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/kit/*.h" "${PROJECT_SOURCE_DIR}/kit/*.hpp" "${PROJECT_SOURCE_DIR}/kit/*.cpp"
)

add_library(uikit STATIC ${UIKIT_SRC})
# =================================

# ========= linking deps ==========
target_link_libraries(uikit PUBLIC Vulkan::Vulkan)
target_link_libraries(uikit PUBLIC imgui)
target_link_libraries(uikit PUBLIC glm)
target_link_libraries(uikit PUBLIC stbimage)
target_link_libraries(uikit PUBLIC spdlog)
target_link_libraries(uikit PUBLIC SDL2)

# =================================

# ========== include dir ==========
target_include_directories(uikit PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(uikit PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb_image")
# =================================
