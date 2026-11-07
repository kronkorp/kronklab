#include "kronklab/kronklab.h"
#include <stddef.h>
#include <stdio.h>

Test(test1, test_assert_eq)
{
    AssertEq(1, 1, "1 is not equal to 1");
}

Test(test1, test_assert_ne)
{
    AssertNe(1, 1, "1 is equal to 1");
}

Test(test_segv, segv)
{
    int * a = 0;
    *a = 0;

    AssertEq(1, 1, "1 and 1 should be equal...");
}

Test(test_segv, segv2)
{
    AssertNull(NULL, "Wait ??");
}

Test(test_str, good_1)
{
    char str[] = "caca";
    char str2[] = "caca";

    AssertStrEq(str, str2, "%s != %s", str, str2);
}

Bench(test_bench, t1)
{
    for (size_t i = 0; i < 10000000 ; ++i) {
        ;
    }
}

Bench(test_bench, t2)
{
    BenchStart();
    for (size_t i = 0; i < 10000000 ; ++i) {
        ;
    }
    BenchEnd();
    BenchReport();
}
