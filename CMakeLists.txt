cmake_minimum_required(VERSION 3.30)

project(taban)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -fanalyzer -Wanalyzer-too-complex)
endif ()

set(CMAKE_C_STANDARD 99)

###########################################################

set(files "havuz.c" "havuz.h" "allocator.c" "allocator.h")

add_executable(taban_test "main.c" ${files})

add_library(taban ${files})


