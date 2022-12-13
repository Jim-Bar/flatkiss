find_package(SDL2 REQUIRED)

add_library(${LIBRARY_MEDIA} STATIC
    lib${NAME_PROJECT}/${NAME_MEDIA}/event_handler.cpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/event_handler.hpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/key.cpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/key.hpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/media.hpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/renderer.cpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/renderer.hpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/texture.cpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/texture.hpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/texture_atlas.cpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/texture_atlas.hpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/window.cpp
    lib${NAME_PROJECT}/${NAME_MEDIA}/window.hpp
)

target_include_directories(${LIBRARY_MEDIA} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
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
