cmake_minimum_required(VERSION 3.13)

# Enable compile_commands.json generation for clangd and other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json for clangd" FORCE)

project(RealEngine VERSION 0.1.0 LANGUAGES CXX)

# Platform detection
if(WIN32)
    set(REALENGINE_PLATFORM_WINDOWS ON)
elseif(UNIX AND NOT APPLE)
    set(REALENGINE_PLATFORM_LINUX ON)
elseif(APPLE)
    set(REALENGINE_PLATFORM_MACOS ON)
endif()

# C++ standard configuration (set globally, but also per-target for clarity)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect engine sources
# Note: Using CONFIGURE_DEPENDS helps with incremental builds when files are added/removed
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

# Separate source files from headers for better dependency tracking
# Only include .cpp files in the executable, headers are for IDE organization only
set(ENGINE_SOURCES
    src/main.cpp
)

# Filter out headers from source lists (only include .cpp files)
foreach(FILE ${SRC_RENDER} ${SRC_RESOURCES} ${SRC_MODULES} ${SRC_EVENTS} ${SRC_INPUT} ${SRC_SHADER_UTILS})
    if(FILE MATCHES "\\.cpp$")
        list(APPEND ENGINE_SOURCES ${FILE})
    endif()
endforeach()

# Create the main executable
add_executable(RealEngine ${ENGINE_SOURCES})

# Set C++ standard per target (explicit is better than implicit)
set_target_properties(RealEngine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Platform-specific settings for Windows
if(REALENGINE_PLATFORM_WINDOWS)
    # Export symbols on Windows (only for this target, not globally)
    set_target_properties(RealEngine PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    
    # Windows-specific compiler options
    target_compile_options(RealEngine PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>  # Warning level 4
        $<$<CXX_COMPILER_ID:MSVC>:/MP>  # Multi-processor compilation
        $<$<CXX_COMPILER_ID:MSVC>:/bigobj>  # Large object files support
    )
endif()

# Linux-specific compiler options
if(REALENGINE_PLATFORM_LINUX)
    target_compile_options(RealEngine PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        $<$<CXX_COMPILER_ID:Clang>:-Wall>
    )
endif()

# Include directories
target_include_directories(RealEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm
)

# Source groups for IDEs (Visual Studio, Xcode, etc.)
source_group("Render" FILES ${SRC_RENDER})
source_group("Resources" FILES ${SRC_RESOURCES})
source_group("Modules" FILES ${SRC_MODULES})
source_group("Events" FILES ${SRC_EVENTS})
source_group("Input" FILES ${SRC_INPUT})
source_group("ShaderUtils" FILES ${SRC_SHADER_UTILS})

# ============================================================================
# Dependencies
# ============================================================================

# GLFW - Window and input handling
# Disable GLFW examples, tests, and docs to speed up builds
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Generate GLFW install target" FORCE)

add_subdirectory(dependencies/glfw)
target_link_libraries(RealEngine PRIVATE glfw)

# GLAD - OpenGL loader
add_subdirectory(dependencies/glad)
target_link_libraries(RealEngine PRIVATE glad)

# GLM - Math library (header-only, but we add it as a subdirectory for consistency)
add_subdirectory(dependencies/glm)

# ImGui - Immediate mode GUI
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

# ImGui target configuration
target_include_directories(ImGui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui>
    $<INSTALL_INTERFACE:include>
)

# ImGui depends on GLAD and GLFW
target_link_libraries(ImGui PUBLIC glad glfw)

# Set C++ standard for ImGui as well
set_target_properties(ImGui PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Platform-specific options for ImGui
if(REALENGINE_PLATFORM_WINDOWS)
    target_compile_options(ImGui PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
endif()

target_link_libraries(RealEngine PRIVATE ImGui)

# Assimp - 3D model loading
# Disable Assimp tools, tests, and samples to speed up builds
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build Assimp tools" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build Assimp tests" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Build Assimp samples" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Generate Assimp install target" FORCE)
# Disable warnings as errors for Assimp (Clang is stricter and causes build failures)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Treat Assimp warnings as errors" FORCE)

# Build Assimp in a separate binary directory to avoid conflicts
add_subdirectory(dependencies/assimp ${CMAKE_BINARY_DIR}/assimp)

# Link Assimp with proper dependency handling
target_link_libraries(RealEngine PRIVATE assimp)

# Platform-specific system libraries
if(REALENGINE_PLATFORM_LINUX)
    # Linux-specific system libraries
    find_package(Threads REQUIRED)
    target_link_libraries(RealEngine PRIVATE Threads::Threads)
    
    # OpenGL library on Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(RealEngine PRIVATE OpenGL::GL)
endif()

if(REALENGINE_PLATFORM_WINDOWS)
    # Windows-specific libraries (OpenGL is typically provided by the driver)
    # Add any Windows-specific system libraries here if needed
endif()

# ============================================================================
# Build configuration
# ============================================================================

# Runtime output directory
set_target_properties(RealEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy resources after build (cross-platform)
add_custom_command(TARGET RealEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:RealEngine>/res
    COMMENT "Copying resources to output directory"
)

# Generate compile_commands.json for Visual Studio generators (workaround)
# Visual Studio generators don't support CMAKE_EXPORT_COMPILE_COMMANDS directly
if(CMAKE_EXPORT_COMPILE_COMMANDS AND (CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "NMake"))
    # Create a custom target that generates compile_commands.json from the build system
    # This is a workaround for Visual Studio generators
    file(WRITE ${CMAKE_BINARY_DIR}/generate_compile_commands.cmake
        "execute_process(
            COMMAND ${CMAKE_COMMAND} -E echo \"Note: compile_commands.json generation for Visual Studio generators requires building the project first.\"
            COMMAND ${CMAKE_COMMAND} -E echo \"Consider using Ninja generator for better compile_commands.json support.\"
        )"
    )
    message(STATUS "Note: Visual Studio generator detected. compile_commands.json will be limited.")
    message(STATUS "      Consider using: cmake -G Ninja -B build")
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "=== RealEngine Build Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "Export compile commands: ${CMAKE_EXPORT_COMPILE_COMMANDS}")
message(STATUS "======================================")
message(STATUS "")
