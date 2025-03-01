#pragma once

#include "common.h"
#include "error/result.h"

#define Array(T) $(PArray, T)
#define CArray(T) $(CArray, T)
#define Array_NAME(T) $(Array, T)
#define array_new(T) $(array_new, T)
#define array_delete(T) $(array_delete, T)
#define array_push(T) $(array_push, T)
#define array_at(T) $(array_at, T)
#define array_index(T) $(array_index, T)
#define array_length(T) $(array_length, T)

#define DECLARE_ARRAY(T)                                                           \
    DEFINE(struct, Array_NAME(T));                                                 \
    Array(T) array_new(T)(size_t initial_length, IAllocator allocator);            \
    void array_delete(T)(Array(T) self);                                           \
    void array_push(T)(Array(T) * self, T value);                                  \
    DECLARE_FAILABLE_FUNCTION(array_at(T)(T * *ret, Array(T) self, size_t index)); \
    T* array_index(T)(Array(T) self, size_t index);                                \
    size_t array_length(T)(CArray(T) self);

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
