#pragma once

#include "common.h"
#include "memory/allocator.h"

DEFINE(struct, StackTraceEntry);

DEFINE(struct, StackTrace) {
    IAllocator allocator;
    PStackTraceEntry entry;
};

typedef FUNCTION(FWalker, void, void* context, Ctring filename, Ctring function, int line);

void stacktrace_new(PStackTrace self, IAllocator allocator);
void stacktrace_free(PStackTrace self);
void stacktrace_push(PStackTrace self, Ctring filename, Ctring function, int line);
void stacktrace_walk(CStackTrace self, void* context, FWalker walker);
