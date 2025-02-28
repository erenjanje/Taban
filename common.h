#pragma once

#define var __auto_type
#define let __auto_type const

#include <stddef.h>
#include <stdint.h>

#define array_size(arr) (sizeof((*arr)) / sizeof((*arr)[0]))

#define $(name, T) name##_##T

#define DECLARE(type, name) DECLARE_##type(name)
#define DECLARE_struct(name)            \
    typedef struct name name, *P##name; \
    typedef struct name const *C##name
#define DECLARE_union(name)            \
    typedef union name name, *P##name; \
    typedef union name const *C##name
#define DECLARE_interface(name)         \
    typedef struct name name, *I##name; \
    typedef struct name const *C##name

#define DEFINE(type, name) \
    DECLARE(type, name);   \
    DEFINE_##type(name)

#define DEFINE_struct(name) struct name
#define DEFINE_union(name) union name
#define DEFINE_interface(name) struct name

#define FUNCTION(name, result, ...) result (*name)(__VA_ARGS__)

typedef char *Ztring;
typedef char const *Ctring;
