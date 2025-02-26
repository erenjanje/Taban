#pragma once

#define var __auto_type
#define let __auto_type const

#include <stddef.h>
#include <stdint.h>

#define array_size(arr) (sizeof((*arr)) / sizeof((*arr)[0]))
