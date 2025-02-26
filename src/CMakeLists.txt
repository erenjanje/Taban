set(files "havuz.c" "havuz.h" "allocator.c" "allocator.h")

add_executable(arena_test "main.c" ${files})

add_library(arena ${files})
