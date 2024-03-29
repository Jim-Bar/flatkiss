cmake_minimum_required(VERSION 3.22)

project(FlatKiss)

# List of the library targets to build.
string(TOLOWER ${PROJECT_NAME} NAME_PROJECT)
set(NAME_DATA data)
set(LIBRARY_DATA ${NAME_PROJECT}-${NAME_DATA})
set(NAME_LOGIC logic)
set(LIBRARY_LOGIC ${NAME_PROJECT}-${NAME_LOGIC})
set(NAME_MEDIA media)
set(LIBRARY_MEDIA ${NAME_PROJECT}-${NAME_MEDIA})
set(NAME_MODEL model)
set(LIBRARY_MODEL ${NAME_PROJECT}-${NAME_MODEL})

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
add_subdirectory(${NAME_PROJECT})
add_subdirectory(lib${LIBRARY_DATA})
add_subdirectory(lib${LIBRARY_LOGIC})
add_subdirectory(lib${LIBRARY_MEDIA})
add_subdirectory(lib${LIBRARY_MODEL})
