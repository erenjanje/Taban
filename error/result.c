#include "result.h"

#include <stdio.h>
#include <string.h>

void result_new(IResult const out, Ctring const message, int const code, IAllocator const allocator) {
    *out = (Result){.stacktrace = {0}, .message = message, .code = code};
    stacktrace_new(&out->stacktrace, allocator);
}

void result_delete(IResult self) {
    if (self != out_of_memory) {
        stacktrace_free(&self->stacktrace);
    }
    DEALLOCATE(self->stacktrace.allocator, self);
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

DEFINE(struct, PrintContext) {
    IAllocator allocator;
    Ztring* ret;
    size_t length;
};
static void
result_print_trace_walker(void* const context_, Ctring const filename, Ctring const function, int const line) {
    // TODO: Use string_append and/or string_format when implemented
    let context = (PrintContext*)context_;
    let added_length = snprintf(NULL, 0, "%32s (%s:%d)\n", function, filename, line);
    let ret = (Ztring)ALLOCATE(context->allocator, context->length + added_length + 1);
    snprintf(ret, context->length + added_length + 1, "%s%32s (%s:%d)\n", *context->ret, function, filename, line);
    DEALLOCATE(context->allocator, *context->ret);
    *context->ret = ret;
    context->length += added_length;
}

Ztring result_print(CResult const self, IAllocator const allocator) {
    // TODO: First calculate the max function name length and then align function names with the result
    let message_length = snprintf(NULL, 0, "Error: %s\n", self->message);
    var ret = (Ztring)ALLOCATE(allocator, message_length + 1);
    snprintf(ret, message_length + 1, "Error: %s\n", self->message);
    if (self == out_of_memory) {
        ret = ALLOCATE(allocator, sizeof("out_of_memory"));
        memcpy(ret, "out_of_memory", sizeof("out_of_memory"));
    } else {
        var context = (PrintContext){.allocator = allocator, .ret = &ret, .length = message_length};
        result_walkStackTrace(self, &context, &result_print_trace_walker);
    }
    return ret;
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