#define ARRAY_IMPLEMENTATION
#include "array.h"

#define ARRAY_SIZE(T, length) (sizeof(struct Array(T)) + (length) * sizeof(T))

#define DEFINE_ARRAY(T)                                                                    \
    struct Array(T) {                                                                      \
        IAllocator allocator;                                                              \
        size_t length;                                                                     \
        size_t capacity;                                                                   \
        T data[];                                                                          \
    };                                                                                     \
    Array(T) array_new(T)(size_t const initial_length, IAllocator const allocator) { \
        let ret = (Array(T))ALLOCATE(allocator, ARRAY_SIZE(T, initial_length));            \
        ret->allocator = allocator;                                                        \
        ret->length = 0;                                                                   \
        ret->capacity = initial_length;                                                    \
        return ret;                                                                        \
    }                                                                                      \
    void array_delete(T)(Array(T) const self) {                                            \
        DEALLOCATE(self->allocator, self);                                                 \
    }                                                                                      \
    void array_push(T)(Array(T)* const self_, T const value) {                             \
        var self = *self_;                                                                 \
        if (self->length == self->capacity) {                                              \
            let new_capacity = self->capacity == 0 ? 1 : (self->capacity * 2);             \
            self = REALLOCATE(self->allocator, self, ARRAY_SIZE(T, new_capacity));         \
            self->capacity = new_capacity;                                                 \
        }                                                                                  \
        self->data[self->length] = value;                                                  \
        self->length += 1;                                                                 \
        *self_ = self;                                                                     \
    }                                                                                      \
    T* array_at(T)(Array(T) const self, size_t const index) {                              \
        return (index > self->length) ? NULL : &self->data[index];                         \
    }

ARRAYS(, DEFINE_ARRAY, , )
