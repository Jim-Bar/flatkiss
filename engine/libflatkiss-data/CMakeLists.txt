add_library(${LIBRARY_DATA} STATIC
    lib${NAME_PROJECT}/${NAME_DATA}/data.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/data.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_action_sprite_mapper.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_action_sprite_mapper.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_animation_player.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_animation_player.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_character_template.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_character_template.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_level.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_level.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_solid.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_solid.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_spriteset.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_spriteset.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_tile_solid_mapper.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/loader_tile_solid_mapper.hpp
)

target_include_directories(${LIBRARY_DATA} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${LIBRARY_DATA}
    ${LIBRARY_MODEL}
)

# Setting the C++ version, from Modern CMake:
# https://cliutils.gitlab.io/modern-cmake/chapters/features/cpp11.html
target_compile_features(${LIBRARY_DATA} PUBLIC cxx_std_20)
set_target_properties(${LIBRARY_DATA} PROPERTIES CXX_EXTENSIONS OFF)
