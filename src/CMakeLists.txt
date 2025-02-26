set(files "havuz.c" "havuz.h" "allocator.c" "allocator.h")

add_executable(taban_test "main.c" ${files})

add_library(taban ${files})
