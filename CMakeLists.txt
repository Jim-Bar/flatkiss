cmake_minimum_required(VERSION 3.18)

project(2d-rendering-engine-test)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(2d-rendering-engine-test main.cpp Configuration.cpp)
target_link_libraries(2d-rendering-engine-test ${SDL2_LIBRARIES})
