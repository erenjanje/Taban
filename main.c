#include <math.h>
#include <stdio.h>

#include "array.h"
#include "error/result.h"
#include "memory/pool.h"

IResult sqroot(double* ret, double x) {
    PROLOGUE;
    if (x < 0.0f) {
        THROW(runtime_error("Cannot take root of negative value", 1, heap_allocator));
    }
    *ret = sqrt(x);
    EPILOGUE;
}

IResult doit(double* ret, double a, double b) {
    PROLOGUE;
    TRY(sqroot(ret, a + b));
    EPILOGUE;
}

IResult bar(double* ret, double a, double b) {
    PROLOGUE;
    TRY(doit(ret, a, b));
    EPILOGUE;
}

int main() {
    // let pool = pool_create(0, heap_allocator);
    // var arr = array_new(int)(2, allocator_from_pool(pool));
    // array_push(int)(&arr, 0xdeadbeef);
    // array_push(int)(&arr, 123);
    // printf("%8x %d\n", *array_at(int)(arr, 0), *array_at(int)(arr, 1));
    // pool_destroy(pool);
    double result = 0.0;
    let res = bar(&result, 1.0f, -3.0f);
    return 0;
}
