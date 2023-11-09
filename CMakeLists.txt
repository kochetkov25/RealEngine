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
src/Game/Game.h
src/Game/Game.cpp)

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

file(GLOB_RECURSE SRC_GAME
    "src/Game/*.cpp"
    "src/Game/*.h"
)

source_group("Render" FILES ${SRC_RENDER})

source_group("Resources" FILES ${SRC_RESOURCES})

source_group("Game" FILES ${SRC_GAME})