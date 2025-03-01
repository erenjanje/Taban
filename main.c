#include <math.h>
#include <stdio.h>

#include "array.h"
#include "error/result.h"
#include "memory/pool.h"

DEFINE_FAILABLE_FUNCTION(sqroot(double* ret, double x), {
    if (x < 0.0) {
        THROW(runtime_error("Cannot take root of negative value", 1, heap_allocator));
    }
    *ret = sqrt(x);
})

DEFINE_FAILABLE_FUNCTION(doit(double* ret, double a, double b), { TRY(sqroot(ret, a + b)); })

DEFINE_FAILABLE_FUNCTION(bar(double* ret, double a, double b), { TRY(doit(ret, a, b)); })

int main() {
    let pool = pool_create(0, heap_allocator);
    var arr = array_new(int)(0, heap_allocator);
    array_push(int)(&arr, 0xdeadbeef);
    array_push(int)(&arr, 123);
    var out = (int*)NULL;
    var o = (int*)NULL;
    IGNORE(array_at(int)(&out, arr, 0));
    IGNORE(array_at(int)(&o, arr, 2));
    var d = 0.0;
    let err = bar(&d, 1.0, -3.0);
    result_addStackTrace(err, __FILE__, __func__, __LINE__);
    printf("%s\n", result_print(err, allocator_from_pool(pool)));

    printf("%8x\n", *out);
    pool_destroy(pool);
}
