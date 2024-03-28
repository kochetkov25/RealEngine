set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

cmake_minimum_required(VERSION 3.13.0)

project(RealEngine VERSION 0.1.0)

add_executable(RealEngine src/main.cpp
src/Render/ShaderProgram.cpp
src/Render/ShaderProgram.h
src/Resources/ResourceManager.cpp
src/Resources/ResourceManager.h
src/Resources/stb_image.h
src/Render/Texture2D.h
src/Render/Texture2D.cpp
src/Render/Sprite.cpp
src/Render/Sprite.h
src/Render/AnimatedSprite.h
src/Render/AnimatedSprite.cpp
src/Render/Renderer.h
src/Render/Renderer.cpp
src/Render/Window.h
src/Render/Window.cpp
src/Render/VertexBuffer.h
src/Render/VertexBuffer.cpp
src/Render/VertexArray.h
src/Render/VertexArray.cpp
src/Modules/GUIModule.h
src/Modules/GUIModule.cpp
src/Render/Primitives.cpp
src/Render/Camera.h
src/Render/Camera.cpp)

add_subdirectory(dependencies/glfw)

target_link_libraries(RealEngine glfw)

add_subdirectory(dependencies/glad)

target_link_libraries(RealEngine glad)

include_directories(dependencies/glm)

set_target_properties(RealEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET RealEngine POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:RealEngine>/res)

file(GLOB_RECURSE SRC_RENDER
    "src/Render/*.cpp"
    "src/Render/*.h"
)

file(GLOB_RECURSE SRC_RESOURCES
    "src/Resources/*.cpp"
    "src/Resources/*.h"
)

file(GLOB_RECURSE SRC_MODULES
    "src/Modules/*.cpp"
    "src/Modules/*.h"
)

source_group("Render" FILES ${SRC_RENDER})

source_group("Resources" FILES ${SRC_RESOURCES})

source_group("Modules" FILES ${SRC_MODULES})












include_directories(dependencies/imgui)

set(IMGUI_INCLUDES
	dependencies/imgui/imgui.h
	dependencies/imgui/backends/imgui_impl_glfw.h
	dependencies/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	dependencies/imgui/imgui.cpp
	dependencies/imgui/backends/imgui_impl_glfw.cpp
	dependencies/imgui/backends/imgui_impl_opengl3.cpp
	dependencies/imgui/imgui_demo.cpp
	dependencies/imgui/imgui_tables.cpp
	dependencies/imgui/imgui_widgets.cpp
	dependencies/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_link_libraries(ImGui glad glfw)

target_link_libraries(RealEngine ImGui)
