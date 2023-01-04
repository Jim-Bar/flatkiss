add_library(${LIBRARY_LOGIC} STATIC
    lib${NAME_PROJECT}/${NAME_LOGIC}/character_controller.cpp
    lib${NAME_PROJECT}/${NAME_LOGIC}/character_controller.hpp
    lib${NAME_PROJECT}/${NAME_LOGIC}/collider.cpp
    lib${NAME_PROJECT}/${NAME_LOGIC}/collider.hpp
    lib${NAME_PROJECT}/${NAME_LOGIC}/logic.cpp
    lib${NAME_PROJECT}/${NAME_LOGIC}/logic.hpp
    lib${NAME_PROJECT}/${NAME_LOGIC}/navigator.cpp
    lib${NAME_PROJECT}/${NAME_LOGIC}/navigator.hpp
)

target_include_directories(${LIBRARY_LOGIC} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${LIBRARY_LOGIC}
    ${LIBRARY_MEDIA}
    ${LIBRARY_MODEL}
)

# Setting the C++ version, from Modern CMake:
# https://cliutils.gitlab.io/modern-cmake/chapters/features/cpp11.html
target_compile_features(${LIBRARY_LOGIC} PUBLIC cxx_std_20)
set_target_properties(${LIBRARY_LOGIC} PROPERTIES CXX_EXTENSIONS OFF)
