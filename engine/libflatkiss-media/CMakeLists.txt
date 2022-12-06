find_package(SDL2 REQUIRED)

add_library(${LIBRARY_MEDIA} STATIC
    lib${PROJECT_NAME}/${NAME_MEDIA}/keyboard_state.cpp
    lib${PROJECT_NAME}/${NAME_MEDIA}/keyboard_state.hpp
    lib${PROJECT_NAME}/${NAME_MEDIA}/renderer.cpp
    lib${PROJECT_NAME}/${NAME_MEDIA}/renderer.hpp
    lib${PROJECT_NAME}/${NAME_MEDIA}/texture.cpp
    lib${PROJECT_NAME}/${NAME_MEDIA}/texture.hpp
)

target_include_directories(${LIBRARY_MEDIA} PUBLIC
    lib${PROJECT_NAME}/${NAME_MEDIA}
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${LIBRARY_MEDIA}
    ${LIBRARY_MODEL}
    ${SDL2_LIBRARIES}
)

# Setting the C++ version, from Modern CMake:
# https://cliutils.gitlab.io/modern-cmake/chapters/features/cpp11.html
target_compile_features(${LIBRARY_MEDIA} PUBLIC cxx_std_20)
set_target_properties(${LIBRARY_MEDIA} PROPERTIES CXX_EXTENSIONS OFF)
