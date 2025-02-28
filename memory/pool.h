#pragma once

#include <stdbool.h>

#include "allocator.h"
#include "common.h"

DECLARE(struct, Pool);

PPool pool_create(size_t initial_size, IAllocator allocator);
void pool_destroy(PPool pool);
void* pool_allocate_(PPool pool, size_t size);
void* pool_new_(PPool pool, size_t size, const void* value);
bool pool_reserve(PPool pool, size_t size);
Allocator* allocator_from_pool(PPool pool);

#define pool_allocate(pool, T) ((T*)pool_allocate_((pool), sizeof(T)))
#define pool_new_array(pool, T, length) (T(*)[length]) pool_allocate_((pool), sizeof(T) * (length))
#define pool_new(pool, T, value) ((T*)pool_new_((pool), sizeof(T), (T[]){(value)}))
