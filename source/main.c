#include <stdio.h>

#include "havuz.h"

int main() {
    Pool* const pool = havuz_create(0, heap_allocator);
    Pool* const other_pool = havuz_create(0, havuz_allocator(pool));
    int const* const x = havuz_new(other_pool, int, 31);
    printf("%d\n", *x);
    havuz_destroy(other_pool);
    havuz_destroy(pool);
    printf("%08x\n", *x);
    return 0;
}
