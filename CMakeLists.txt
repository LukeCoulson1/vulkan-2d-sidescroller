cmake_minimum_required(VERSION 3.10)
project(vulkan-2d-sidescroller)

set(CMAKE_CXX_STANDARD 17)

# Find Vulkan package
find_package(Vulkan REQUIRED)

# Find GLFW3 via vcpkg
find_package(glfw3 CONFIG REQUIRED)

# Find GLM via vcpkg
find_package(glm CONFIG REQUIRED)

# Find OpenGL (required for FreeGLUT)
find_package(OpenGL REQUIRED)

# Find FreeGLUT via vcpkg
find_package(FreeGLUT CONFIG REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/game/Game.cpp
    src/renderer/VulkanRenderer.cpp
    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/utils/Utils.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories (optional, vcpkg usually handles this)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    src
    include
)

# Link Vulkan, GLFW, GLM, OpenGL, and FreeGLUT (for menu and text)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw glm::glm OpenGL::GL FreeGLUT::freeglut)