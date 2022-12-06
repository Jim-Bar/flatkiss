cmake_minimum_required(VERSION 3.22)

project(flatkiss)

# List of the library targets to build.
set(NAME_MEDIA media)
set(LIBRARY_MEDIA ${PROJECT_NAME}-${NAME_MEDIA})
set(NAME_MODEL model)
set(LIBRARY_MODEL ${PROJECT_NAME}-${NAME_MODEL})

# Enable clang-tidy using the .clang-tidy file for all the targets.
find_program(CLANG_TIDY_IS_AVAILABLE clang-tidy)
if(CLANG_TIDY_IS_AVAILABLE)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
else()
    message(WARNING "Linter rules are disabled because clang-tidy was not found")
endif()

# Including header files in the targets because from Modern CMake: "The headers will be, for most intents and purposes,
# ignored; the only reason to list them is to get them to show up in IDEs". Refer to:
# https://cliutils.gitlab.io/modern-cmake/chapters/basics.html
add_subdirectory(${PROJECT_NAME})
add_subdirectory(lib${LIBRARY_MEDIA})
add_subdirectory(lib${LIBRARY_MODEL})
