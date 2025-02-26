#include "pool.h"

#undef pool_allocate
#undef pool_new

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
    AllocatorMethods const* methods;
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

static bool pool_expand(Pool* pool, size_t size);

Pool* pool_create(size_t const initial_size, Allocator const* const allocator) {
    Pool* const pool = allocator->methods->allocate(allocator, sizeof(Pool));
    if (pool == NULL) {
        return NULL;
    }
    pool->methods = &pool_methods;
    pool->allocator = allocator;
    CREATE_REGION(pool->first_region, initial_size, allocator, {
        generic_deallocate(pool->allocator, pool);
        return NULL;
    });
    pool->last_region = pool->first_region;
    return pool;
}

void pool_destroy(Pool* const pool) {
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

void* pool_allocate_(Pool* const pool, size_t const size) {
    if (not pool_reserve(pool, size)) {
        return NULL;
    }
    pool->last_region->top += size;
    return pool->last_region->data + pool->last_region->top - size;
}

void* pool_new_(Pool* pool, size_t const size, void const* const value) {
    void* ret = pool_allocate_(pool, size);
    if (ret == NULL) {
        return NULL;
    }
    memcpy(ret, value, size);
    return ret;
}

bool pool_reserve(Pool* const pool, size_t const size) {
    size_t const remaining = pool->last_region->size - pool->last_region->top;
    if (remaining < size) {
        return pool_expand(pool, size);
    }
    return true;
}

Allocator* allocator_from_pool(Pool* pool) {
    return (Allocator*)pool;
}

bool pool_expand(Pool* const pool, size_t const size) {
    size_t const last_region_size = pool->last_region->size == 0 ? 1 : pool->last_region->size;
    size_t const new_region_size = size > last_region_size ? size : last_region_size * 2;
    Region* new_region;
    CREATE_REGION(new_region, new_region_size, pool->allocator, { return false; });
    pool->last_region->next = new_region;
    pool->last_region = new_region;
    return true;
}

/*******************************************************************************************/

static void* pool_generic_allocate(Allocator const* const self, size_t const size) {
    return pool_allocate_((Pool*)self, size);
}

static void pool_generic_deallocate(Allocator const* self, void* const data) {
    (void)self;
    (void)data;
}

AllocatorMethods const pool_methods = {
    .allocate = &pool_generic_allocate,
    .deallocate = &pool_generic_deallocate,
};