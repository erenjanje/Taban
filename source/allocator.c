#include "allocator.h"

#include <stdlib.h>

static void* heap_allocator_allocate(Allocator const* const self, size_t const size) {
    (void)self;
    return malloc(size);
}

static void heap_allocator_deallocate(Allocator const* const self, void* const ptr) {
    (void)self;
    free(ptr);
}

AllocatorMethods const heap_allocator_methods = {
    .allocate = &heap_allocator_allocate,
    .deallocate = &heap_allocator_deallocate,
};

static Allocator const heap_allocator__ = {
    .methods = &heap_allocator_methods,
};

const Allocator* const heap_allocator = &heap_allocator__;
