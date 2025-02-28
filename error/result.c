#include "result.h"

void result_new(IResult const out, Ctring const message, int const code, IAllocator const allocator) {
    *out = (Result){.stacktrace = {0}, .message = message, .code = code};
    stacktrace_new(&out->stacktrace, allocator);
}

void result_delete(IResult self) {
    if (self != out_of_memory) {
        stacktrace_free(&self->stacktrace);
    }
}

void result_addStackTrace(IResult self, Ctring filename, Ctring function, int line) {
    if (self != out_of_memory) {
        stacktrace_push(&self->stacktrace, filename, function, line);
    }
}

void result_walkStackTrace(CResult const self, void* const context, FWalker const walker) {
    if (self != out_of_memory) {
        stacktrace_walk(&self->stacktrace, context, walker);
    }
}

IResult runtime_error(Ctring const message, int const code, IAllocator const allocator) {
    let ret = (IResult)ALLOCATE(allocator, sizeof(Result));
    result_new(ret, message, code, allocator);
    return ret;
}

static Result out_of_memory_ = {
    .stacktrace = {.allocator = NULL, .entry = NULL},
    .message = "Out of memory",
    .code = 1,
};

IResult const out_of_memory = &out_of_memory_;