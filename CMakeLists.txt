cmake_minimum_required(VERSION 3.10)
project(monolith)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Add ImGui source files
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES
    monolith.cpp
    gui.cpp
    recoil_system_updated.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
)

# Add executable
add_executable(monolith ${SOURCES})

# Include directories
target_include_directories(monolith PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${IMGUI_DIR}
)

# Link libraries
target_link_libraries(monolith
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    dl
)

# Add compile definitions
target_compile_definitions(monolith PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)
