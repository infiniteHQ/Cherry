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
# Find SDL2
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2 library
# Note: SDL2 is linked later with `uikit`
# target_link_libraries(uikit PUBLIC ${SDL2_LIBRARIES})

# glm
add_subdirectory(lib/glm)

# imgui
add_subdirectory(lib/imgui)

# spdlog
add_subdirectory(lib/spdlog)

# stb-image
add_subdirectory(lib/stb-image)
# ===============================

# ========== uikit =============
# find all source files and headers
file(
    GLOB_RECURSE # recursive
    UIKIT_SRC # variable to store the source files and headers
    CONFIGURE_DEPENDS # make a dependency
    "${PROJECT_SOURCE_DIR}/platform/*.h" "${PROJECT_SOURCE_DIR}/platform/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/components/*.h" "${PROJECT_SOURCE_DIR}/components/*.cpp"
)

add_library(uikit STATIC ${UIKIT_SRC})
# =================================

# ========= linking deps ==========
target_link_libraries(uikit PUBLIC Vulkan::Vulkan)
target_link_libraries(uikit PUBLIC imgui)
target_link_libraries(uikit PUBLIC glm)
target_link_libraries(uikit PUBLIC stbimage)
target_link_libraries(uikit PUBLIC spdlog)

# Link SDL2 library
target_link_libraries(uikit PUBLIC ${SDL2_LIBRARIES})
# =================================

# ========== include dir ==========
target_include_directories(uikit PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(uikit PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb_image")
# =================================
