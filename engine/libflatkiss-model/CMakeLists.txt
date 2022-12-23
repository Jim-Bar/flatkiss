add_library(${LIBRARY_MODEL} STATIC
    lib${NAME_PROJECT}/${NAME_MODEL}/action.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/action.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/action_sprite_mapper.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/action_sprite_mapper.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/animation.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/animation.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/animation_player.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/animation_player.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/cardinal_direction.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/cardinal_direction.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/character.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/character.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/collider.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/collider.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/ellipse.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/ellipse.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/level.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/level.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/model.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/model.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/navigator.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/navigator.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/position.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/position.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/positioned_ellipse.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/positioned_ellipse.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/positioned_rectangle.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/positioned_rectangle.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/positioned_solid.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/positioned_solid.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/rectangle.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/rectangle.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/solid.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/solid.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/spriteset.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/spriteset.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/tile_solid_mapper.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/tile_solid_mapper.hpp
    lib${NAME_PROJECT}/${NAME_MODEL}/vector.cpp
    lib${NAME_PROJECT}/${NAME_MODEL}/vector.hpp
)

target_include_directories(${LIBRARY_MODEL} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# Setting the C++ version, from Modern CMake:
# https://cliutils.gitlab.io/modern-cmake/chapters/features/cpp11.html
target_compile_features(${LIBRARY_MODEL} PUBLIC cxx_std_20)
set_target_properties(${LIBRARY_MODEL} PROPERTIES CXX_EXTENSIONS OFF)
