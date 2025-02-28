#include "allocator.h"

#include <stdlib.h>

static void* heap_allocator_allocate(IAllocator const self, size_t const size) {
    (void)self;
    return malloc(size);
}

static void heap_allocator_deallocate(IAllocator const self, void* const ptr) {
    (void)self;
    free(ptr);
}

static void* heap_allocator_reallocate(IAllocator const self, void* const ptr, size_t const size) {
    (void)self;
    let ret = realloc(ptr, size);
    if (ret == NULL) {
        free(ptr);
        return NULL;
    }
    return ret;
}

AllocatorMethods const heap_allocator_methods = {
    .allocate = &heap_allocator_allocate,
    .deallocate = &heap_allocator_deallocate,
    .reallocate = &heap_allocator_reallocate,
};

static Allocator heap_allocator__ = {
    .methods = &heap_allocator_methods,
};

IAllocator const heap_allocator = &heap_allocator__;
