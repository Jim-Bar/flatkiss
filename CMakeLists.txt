cmake_minimum_required(VERSION 3.18)

project(FlatKiss)

configure_file(
    src/Main.hpp.in
    src/Main.hpp
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Animation.cpp
    src/AnimationPlayer.cpp
    src/Collider.cpp
    src/Collision.cpp
    src/Configuration.cpp
    src/KeyboardState.cpp
    src/Level.cpp
    src/Position.cpp
    src/PositionedRectangle.cpp
    src/Rectangle.cpp
    src/Renderer.cpp
    src/Tileset.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
