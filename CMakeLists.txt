cmake_minimum_required(VERSION 3.16)
project(libwalnut)

# ========= setup ================
set(CMAKE_CXX_STANDARD 17)

# for cmake modules if u wnat to add some libraries
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# flags for gcc or clang
set(GCC_COVERAGE_COMPILE_FLAGS "")

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS	"${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
endif()
# ===============================

# ========= deps ================
# vulkan loader
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(../glfw)

# glm
add_subdirectory(../glm)

# imgui
add_subdirectory(../imgui)

#stb-image
add_subdirectory(../stb-image)
# ===============================

# ========== walnut =============
# find all source files and headers
file(
	GLOB_RECURSE # recursive
	WALNUT_SRC # variable to store the source files and headers
	CONFIGURE_DEPENDS # make a dependency
	"${PROJECT_SOURCE_DIR}/Platform/*.h" "${PROJECT_SOURCE_DIR}/Platform/*.cpp"
	"${PROJECT_SOURCE_DIR}/Source/*.h" "${PROJECT_SOURCE_DIR}/Source/*.cpp"
)

add_library(walnut STATIC ${WALNUT_SRC})
# =================================

# ========= linking deps ==========
target_link_libraries(walnut PUBLIC Vulkan::Vulkan)
target_link_libraries(walnut PUBLIC glfw)
target_link_libraries(walnut PUBLIC imgui)
target_link_libraries(walnut PUBLIC glm)
target_link_libraries(walnut PUBLIC stbimage)
target_link_libraries(walnut PUBLIC spdlog::spdlog)
# =================================

# ========== include dir ==========
target_include_directories(walnut PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(walnut PUBLIC "${PROJECT_SOURCE_DIR}/../stb_image")
# =================================