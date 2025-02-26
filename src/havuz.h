#ifndef HAVUZ_H
#define HAVUZ_H

#include "allocator.h"

typedef struct Pool Pool;

#include <stdbool.h>
#include <stddef.h>

Pool* havuz_create(size_t initial_size, Allocator const* allocator);
void havuz_destroy(Pool* pool);
void* havuz_allocate(Pool* pool, size_t size);
void* havuz_new(Pool* pool, size_t size, const void* value);
bool havuz_reserve(Pool* pool, size_t size);
Allocator* havuz_allocator(Pool* pool);

#define havuz_allocate(pool, T) ((T*)havuz_allocate((pool), sizeof(T)))
#define havuz_new(pool, T, value) ((T*)havuz_new((pool), sizeof(T), (T[]){(value)}))

extern AllocatorMethods const havuz_methods;

#endif
