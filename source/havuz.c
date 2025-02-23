#include "havuz.h"

#undef havuz_allocate
#undef havuz_new

#include <iso646.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Region Region;
struct Region {
    size_t size;
    size_t top;
    Region* next;
    uint8_t data[];
};

struct Pool {
    Allocator self_allocator;
    Allocator const* allocator;
    Region* first_region;
    Region* last_region;
};

#define CREATE_REGION(region, region_size, allocator, ...)                  \
    do {                                                                    \
        region = generic_allocate(allocator, sizeof(Region) + region_size); \
        if (region == NULL) {                                               \
            __VA_ARGS__;                                                    \
        }                                                                   \
        region->size = region_size;                                         \
        region->top = 0;                                                    \
        region->next = NULL;                                                \
    } while (0)

static bool havuz_expand(Pool* pool, size_t size);

Pool* havuz_create(size_t const initial_size, Allocator const* const allocator) {
    Pool* const pool = allocator->methods->allocate(allocator, sizeof(Pool));
    if (pool == NULL) {
        return NULL;
    }
    pool->self_allocator = (Allocator){
        .data = pool,
        .methods = &havuz_methods,
    };
    pool->allocator = allocator;
    CREATE_REGION(pool->first_region, initial_size, allocator, {
        generic_deallocate(pool->allocator, pool);
        return NULL;
    });
    pool->last_region = pool->first_region;
    return pool;
}

void havuz_destroy(Pool* const pool) {
    Region* current = pool->first_region;
    while (current != NULL) {
        Region* next = current->next;
#ifndef NDEBUG
        memset(current->data, 0xFE, current->size);
#endif
        generic_deallocate(pool->allocator, current);
        current = next;
    }
    generic_deallocate(pool->allocator, pool);
}

void* havuz_allocate(Pool* const pool, size_t const size) {
    if (not havuz_reserve(pool, size)) {
        return NULL;
    }
    pool->last_region->top += size;
    return pool->last_region->data + pool->last_region->top - size;
}

void* havuz_new(Pool* pool, size_t const size, void const* const value) {
    void* ret = havuz_allocate(pool, size);
    if (ret == NULL) {
        return NULL;
    }
    memcpy(ret, value, size);
    return ret;
}

bool havuz_reserve(Pool* const pool, size_t const size) {
    size_t const remaining = pool->last_region->size - pool->last_region->top;
    if (remaining < size) {
        return havuz_expand(pool, size);
    }
    return true;
}
Allocator* havuz_allocator(Pool* pool) {
    return &pool->self_allocator;
}

bool havuz_expand(Pool* const pool, size_t const size) {
    size_t const last_region_size = pool->last_region->size == 0 ? 1 : pool->last_region->size;
    size_t const new_region_size = size > last_region_size ? size : last_region_size * 2;
    Region* new_region;
    CREATE_REGION(new_region, new_region_size, pool->allocator, { return false; });
    pool->last_region->next = new_region;
    pool->last_region = new_region;
    return true;
}

/*******************************************************************************************/

static void* havuz_generic_allocate(Allocator const* const self, size_t const size) {
    if (self->methods != &havuz_methods) {
        return NULL;
    }
    return havuz_allocate(self->data, size);
}

static void havuz_generic_deallocate(Allocator const* self, void* const data) {
    (void)self;
    (void)data;
}

AllocatorMethods const havuz_methods = {
    .allocate = &havuz_generic_allocate,
    .deallocate = &havuz_generic_deallocate,
};