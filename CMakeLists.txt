cmake_minimum_required(VERSION 3.30)

project(taban)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -fanalyzer -Wanalyzer-too-complex -Wno-unused-variable)
endif ()

set(CMAKE_C_STANDARD 99)

###########################################################

set(files
        "memory/pool.c" "memory/pool.h"
        "memory/allocator.c" "memory/allocator.h"
        "common.c" "common.h")

add_executable(taban_test "main.c" ${files})

add_library(taban ${files})

target_include_directories(taban_test PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(taban PRIVATE ${CMAKE_SOURCE_DIR})

