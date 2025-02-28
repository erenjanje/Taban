#include "trace.h"

#include "memory/allocator.h"

struct StackTraceEntry {
    char const* filename;
    char const* function;
    int line;
    PStackTraceEntry next;
};

void stacktrace_new(PStackTrace self, IAllocator const allocator) {
    *self = (StackTrace){.allocator = allocator, .entry = NULL};
}

void stacktrace_free(PStackTrace self) {
    var entry = self->entry;
    while (entry) {
        let next = entry->next;
        DEALLOCATE(self->allocator, entry);
        entry = next;
    }
}

void stacktrace_push(PStackTrace const self, Ctring const filename, Ctring const function, int const line) {
    let pushed = (PStackTraceEntry)ALLOCATE(self->allocator, sizeof(StackTraceEntry));
    if (pushed == NULL) {
        return;
    }
    *pushed = (StackTraceEntry){.filename = filename, .function = function, .line = line, .next = self->entry};
    self->entry = pushed;
}

void stacktrace_walk(CStackTrace const self, void* const context, FWalker const walker) {
    var entry = self->entry;
    while (entry) {
        let next = entry->next;
        walker(context, entry->filename, entry->function, entry->line);
        entry = next;
    }
}