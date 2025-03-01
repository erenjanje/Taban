#pragma once

#include "common.h"
#include "error/trace.h"

#ifdef __GNUC__
#define NODISCARD __attribute__((warn_unused_result))
#else
#define NODISCARD
#endif

DEFINE(interface, Result) {
    StackTrace stacktrace;
    Ctring message;
    int code;
};

void result_new(IResult out, Ctring message, int code, IAllocator allocator);
void result_delete(IResult self);
void result_addStackTrace(IResult self, Ctring filename, Ctring function, int line);
void result_walkStackTrace(CResult self, void* context, FWalker walker);
Ztring result_print(CResult self, IAllocator allocator);

IResult runtime_error(Ctring message, int code, IAllocator allocator);

extern IResult const out_of_memory;

#define PROLOGUE IResult result = NULL
#define EPILOGUE \
    epilogue:    \
    return result

#define THROW(error, ...)                                        \
    do {                                                         \
        var err = (error);                                       \
        if (err == NULL) {                                       \
            err = out_of_memory;                                 \
        }                                                        \
        result_addStackTrace(err, __FILE__, __func__, __LINE__); \
        result = err;                                            \
        {                                                        \
            __VA_ARGS__                                          \
        }                                                        \
        goto epilogue;                                           \
    } while (0)

#define SUCCESS(...)    \
    do {                \
        result = OK;    \
        {               \
            __VA_ARGS__ \
        }               \
        goto epilogue;  \
    } while (0)

#define TRY(expr, ...)                                                  \
    do {                                                                \
        if ((result = (expr))) {                                        \
            result_addStackTrace(result, __FILE__, __func__, __LINE__); \
            {                                                           \
                __VA_ARGS__                                             \
            }                                                           \
            goto epilogue;                                              \
        }                                                               \
    } while (0)

#define IGNORE(expr, ...)        \
    do {                         \
        let res_ = (expr);       \
        if (res_) {              \
            result_delete(res_); \
        }                        \
    } while (0)

#define OK NULL

#define DEFINE_FAILABLE_FUNCTION(func, ...) \
    NODISCARD IResult func {                \
        PROLOGUE;                           \
        {__VA_ARGS__} EPILOGUE;             \
    }

#define DECLARE_FAILABLE_FUNCTION(func) NODISCARD IResult func
