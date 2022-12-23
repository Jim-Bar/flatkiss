add_library(${LIBRARY_DATA} STATIC
    lib${NAME_PROJECT}/${NAME_DATA}/data.hpp
    lib${NAME_PROJECT}/${NAME_DATA}/file_loader.cpp
    lib${NAME_PROJECT}/${NAME_DATA}/file_loader.hpp
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
