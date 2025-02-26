#pragma once

#include "common.h"

typedef struct Allocator Allocator;

typedef struct AllocatorMethods AllocatorMethods;
struct AllocatorMethods {
    void* (*allocate)(Allocator const* self, size_t size);
    void (*deallocate)(Allocator const* self, void* data);
};

struct Allocator {
    AllocatorMethods const* methods;
};

extern const Allocator* const heap_allocator;

#define generic_allocate(allocator, size) allocator->methods->allocate(allocator, size)
#define generic_deallocate(allocator, data) allocator->methods->deallocate(allocator, data)
