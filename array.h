#pragma once

#include "common.h"
#include "memory/allocator.h"

#define Array(T) $(Array, T)
#define array_new(T) $(array_new, T)
#define array_delete(T) $(array_delete, T)
#define array_push(T) $(array_push, T)
#define array_at(T) $(array_at, T)

#define DECLARE_ARRAY(T)                                                      \
    typedef struct Array(T) * Array(T);                                       \
    Array(T) array_new(T)(size_t initial_length, IAllocator allocator); \
    void array_delete(T)(Array(T) self);                                      \
    void array_push(T)(Array(T) * self, T value);                             \
    T* array_at(T)(Array(T) self, size_t index);

#define ARRAY_TYPES(X, sep) X(int) sep X(float)
#define ARRAYS(begin, X, sep, end) \
    begin ARRAY_TYPES(X, sep)      \
    end

ARRAYS(, DECLARE_ARRAY, , )

#ifndef ARRAY_IMPLEMENTATION
#undef ARRAYS
#undef ARRAY_TYPES
#undef DECLARE_ARRAY
#endif
