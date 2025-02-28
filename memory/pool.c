#include "pool.h"

#undef pool_allocate
#undef pool_new

#include <iso646.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define POOL_CURRENT_POINTER(region) ((region)->data + (region)->top)

DEFINE(struct, Metadata) {
    size_t size;
};

DEFINE(struct, Region) {
    size_t size;
    size_t top;
    Region* next;
    uint8_t data[];
};

struct Pool {
    Allocator parent;
    IAllocator allocator;
    Region* first_region;
    Region* last_region;
};

AllocatorMethods const pool_methods;

#define CREATE_REGION(region, region_size, allocator, ...)          \
    do {                                                            \
        region = ALLOCATE(allocator, sizeof(Region) + region_size); \
        if (region == NULL) {                                       \
            __VA_ARGS__;                                            \
        }                                                           \
        region->size = region_size;                                 \
        region->top = 0;                                            \
        region->next = NULL;                                        \
    } while (0)

static bool pool_expand(PPool pool, size_t size);

PPool pool_create(size_t const initial_size, IAllocator const allocator) {
    let pool = (PPool)allocator->methods->allocate(allocator, sizeof(Pool));
    if (pool == NULL) {
        return NULL;
    }
    pool->parent = (Allocator){.methods = &pool_methods};
    pool->allocator = allocator;
    CREATE_REGION(pool->first_region, initial_size, allocator, {
        DEALLOCATE(pool->allocator, pool);
        return NULL;
    });
    pool->last_region = pool->first_region;
    return pool;
}

void pool_destroy(PPool const pool) {
    Region* current = pool->first_region;
    while (current != NULL) {
        Region* next = current->next;
#ifndef NDEBUG
        memset(current->data, 0xFE, current->size);
#endif
        DEALLOCATE(pool->allocator, current);
        current = next;
    }
    DEALLOCATE(pool->allocator, pool);
}

void* pool_allocate_(PPool const pool, size_t const size) {
    let size_with_metadata = sizeof(Metadata) + size;
    if (not pool_reserve(pool, size_with_metadata)) {
        return NULL;
    }

    let metadata_pointer = (Metadata*)POOL_CURRENT_POINTER(pool->last_region);
    pool->last_region->top += sizeof(Metadata);
    let ret = POOL_CURRENT_POINTER(pool->last_region);
    pool->last_region->top += size;
    *metadata_pointer = (Metadata){.size = size};
    return ret;
}

void* pool_new_(PPool pool, size_t const size, void const* const value) {
    void* ret = pool_allocate_(pool, size);
    if (ret == NULL) {
        return NULL;
    }
    memcpy(ret, value, size);
    return ret;
}

bool pool_reserve(PPool const pool, size_t const size) {
    size_t const remaining = pool->last_region->size - pool->last_region->top;
    if (remaining < size) {
        return pool_expand(pool, size);
    }
    return true;
}

Allocator* allocator_from_pool(PPool pool) {
    return &pool->parent;
}

bool pool_expand(PPool const pool, size_t const size) {
    size_t const last_region_size = pool->last_region->size == 0 ? 1 : pool->last_region->size;
    size_t const new_region_size = size > last_region_size ? size : last_region_size * 2;
    Region* new_region;
    CREATE_REGION(new_region, new_region_size, pool->allocator, { return false; });
    pool->last_region->next = new_region;
    pool->last_region = new_region;
    return true;
}

/*******************************************************************************************/

static void* pool_generic_allocate(IAllocator const self, size_t const size) {
    return pool_allocate_((PPool)self, size);
}

static void pool_generic_deallocate(IAllocator self, void* const data) {
    (void)self;
    (void)data;
}

static void* pool_generic_reallocate(IAllocator self, void* const data, size_t const size) {
    (void)data;
    (void)size;
    return pool_generic_allocate(self, size);
}

AllocatorMethods const pool_methods = {
    .allocate = &pool_generic_allocate,
    .deallocate = &pool_generic_deallocate,
    .reallocate = &pool_generic_reallocate,
};