add_library(${LIBRARY_MODEL} STATIC
    lib${PROJECT_NAME}/${NAME_MODEL}/action.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/action.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/action_sprite_mapper.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/action_sprite_mapper.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/animation.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/animation.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/animation_player.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/animation_player.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/cardinal_direction.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/cardinal_direction.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/character.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/character.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/collider.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/collider.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/ellipse.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/ellipse.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/level.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/level.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/navigator.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/navigator.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/position.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/position.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/positioned_ellipse.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/positioned_ellipse.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/positioned_rectangle.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/positioned_rectangle.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/positioned_solid.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/positioned_solid.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/rectangle.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/rectangle.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/solid.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/solid.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/spriteset.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/spriteset.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/tile_solid_mapper.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/tile_solid_mapper.hpp
    lib${PROJECT_NAME}/${NAME_MODEL}/vector.cpp
    lib${PROJECT_NAME}/${NAME_MODEL}/vector.hpp
)

target_include_directories(${LIBRARY_MODEL} PUBLIC
    lib${PROJECT_NAME}/${NAME_MODEL}
)

# Setting the C++ version, from Modern CMake:
# https://cliutils.gitlab.io/modern-cmake/chapters/features/cpp11.html
target_compile_features(${LIBRARY_MODEL} PUBLIC cxx_std_20)
set_target_properties(${LIBRARY_MODEL} PROPERTIES CXX_EXTENSIONS OFF)
