cmake_minimum_required(VERSION 3.18)

project(2d-rendering-engine-test)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(2d-rendering-engine-test
    src/main.cpp
    src/Animation.cpp
    src/AnimationPlayer.cpp
    src/Collider.cpp
    src/Collision.cpp
    src/Configuration.cpp
    src/KeyboardState.cpp
    src/Level.cpp
    src/Renderer.cpp
    src/Tileset.cpp)
target_link_libraries(2d-rendering-engine-test ${SDL2_LIBRARIES})
