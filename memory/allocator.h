#pragma once

#include "common.h"

DEFINE(interface, Allocator);

typedef struct AllocatorMethods AllocatorMethods;
struct AllocatorMethods {
    FUNCTION(allocate, void*, IAllocator self, size_t size);
    FUNCTION(deallocate, void, IAllocator self, void* data);
    FUNCTION(reallocate, void*, IAllocator self, void* data, size_t size);
};

struct Allocator {
    AllocatorMethods const* methods;
};

extern IAllocator const heap_allocator;

#define ALLOCATE(allocator, size) allocator->methods->allocate((allocator), (size))
#define DEALLOCATE(allocator, data) allocator->methods->deallocate((allocator), (data))
#define REALLOCATE(allocator, data, size) allocator->methods->reallocate((allocator), (data), (size))
