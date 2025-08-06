cmake_minimum_required(VERSION 3.13)

# Export symbols on Windows automatically
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(RealEngine VERSION 0.1.0 LANGUAGES CXX)

# C++ standard configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect engine sources
file(GLOB_RECURSE SRC_RENDER CONFIGURE_DEPENDS
    "src/Render/*.cpp"
    "src/Render/*.h"
)
file(GLOB_RECURSE SRC_RESOURCES CONFIGURE_DEPENDS
    "src/Resources/*.cpp"
    "src/Resources/*.h"
)
file(GLOB_RECURSE SRC_MODULES CONFIGURE_DEPENDS
    "src/Modules/*.cpp"
    "src/Modules/*.h"
)
file(GLOB_RECURSE SRC_EVENTS CONFIGURE_DEPENDS
    "src/Render/Events/*.cpp"
    "src/Render/Events/*.h"
)
file(GLOB_RECURSE SRC_INPUT CONFIGURE_DEPENDS
    "src/Input/*.cpp"
    "src/Input/*.h"
)
file(GLOB_RECURSE SRC_SHADER_UTILS CONFIGURE_DEPENDS
    "src/Render/ShaderUtils/*.cpp"
    "src/Render/ShaderUtils/*.h"
)

set(ENGINE_SOURCES
    src/main.cpp
    ${SRC_RENDER}
    ${SRC_RESOURCES}
    ${SRC_MODULES}
    ${SRC_EVENTS}
    ${SRC_INPUT}
    ${SRC_SHADER_UTILS}
)

add_executable(RealEngine ${ENGINE_SOURCES})

target_include_directories(RealEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm
)

# Source groups for IDEs
source_group("Render" FILES ${SRC_RENDER})
source_group("Resources" FILES ${SRC_RESOURCES})
source_group("Modules" FILES ${SRC_MODULES})
source_group("Events" FILES ${SRC_EVENTS})
source_group("Input" FILES ${SRC_INPUT})
source_group("ShaderUtils" FILES ${SRC_SHADER_UTILS})

# GLFW
add_subdirectory(dependencies/glfw)

target_link_libraries(RealEngine PRIVATE glfw)

# GLAD
add_subdirectory(dependencies/glad)

target_link_libraries(RealEngine PRIVATE glad)

# ImGui
file(GLOB IMGUI_HEADERS CONFIGURE_DEPENDS
    dependencies/imgui/imgui.h
    dependencies/imgui/backends/imgui_impl_glfw.h
    dependencies/imgui/backends/imgui_impl_opengl3.h
)
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS
    dependencies/imgui/imgui.cpp
    dependencies/imgui/backends/imgui_impl_glfw.cpp
    dependencies/imgui/backends/imgui_impl_opengl3.cpp
    dependencies/imgui/imgui_demo.cpp
    dependencies/imgui/imgui_tables.cpp
    dependencies/imgui/imgui_widgets.cpp
    dependencies/imgui/imgui_draw.cpp
)
add_library(ImGui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC dependencies/imgui)

target_link_libraries(ImGui PUBLIC glad glfw)

target_link_libraries(RealEngine PRIVATE ImGui)

# Assimp
add_subdirectory(dependencies/assimp ${CMAKE_BINARY_DIR}/assimp)

target_link_libraries(RealEngine PRIVATE assimp)

# Runtime output
set_target_properties(RealEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy resources after build
add_custom_command(TARGET RealEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:RealEngine>/res
)