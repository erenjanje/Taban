cmake_minimum_required(VERSION 3.30)

project(havuz)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -fanalyzer)
endif ()

set(CMAKE_C_STANDARD 99)

add_subdirectory(source)

