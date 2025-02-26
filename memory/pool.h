#pragma once

#include <stdbool.h>

#include "allocator.h"
#include "common.h"

typedef struct Pool Pool;

Pool* pool_create(size_t initial_size, Allocator const* allocator);
void pool_destroy(Pool* pool);
void* pool_allocate_(Pool* pool, size_t size);
void* pool_new_(Pool* pool, size_t size, const void* value);
bool pool_reserve(Pool* pool, size_t size);
Allocator* allocator_from_pool(Pool* pool);

#define pool_allocate(pool, T) ((T*)pool_allocate_((pool), sizeof(T)))
#define pool_new_array(pool, T, length) (T(*)[length]) pool_allocate_((pool), sizeof(T) * (length))
#define pool_new(pool, T, value) ((T*)pool_new_((pool), sizeof(T), (T[]){(value)}))
