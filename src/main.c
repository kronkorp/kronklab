#include "../include/kronklab/kronklab.h"
#include "private.h"
#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
__kl_PLACE(".kl$a") const struct __kl_funcStruct __kl_sentinel_begin = {"", "", __kl_test, NULL};
__kl_PLACE(".kl$z") const struct __kl_funcStruct __kl_sentinel_end = {"", "", __kl_test, NULL};
#endif

int main(int argc, char **argv)
{
    struct __kl_funcStruct *current = KL_BEGIN;
    struct __kl_funcStruct *end = KL_END;
    char *csuite = NULL;
    unsigned long results[4] = {0, 0, 0, 0};
    timestamp start;

#ifdef _WIN32
    if (argc == 3 && !strcmp(argv[1], kl_CHILD_FLAG)) {
        current[strtoul(argv[2], NULL, 10)].func();
        return 0;
    }
    SetConsoleOutputCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(h, &mode)) {
        SetConsoleMode(h, mode | 0x4); /* ENABLE_VIRTUAL_TERMINAL_PROCESSING: ANSI colors */
    }
#endif
    (void)argc;
    (void)argv;
    start = kl_monotonic();

    while (current < end) {
        if (!csuite || strncmp(current->suite, csuite, 32)) {
            if (csuite) {
                printf("\n");
            }
            printf("========= %.32s =========\n\n", current->suite);
        }
        csuite = current->suite;
        switch (current->type) {
            case __kl_test:
                klInfo("Running test n°%.2lu: %s:%s\n", results[0] + 1, current->suite, current->name);
                run_test(current, results);
                ++results[0];
                break;
            case __kl_bench:
                klInfo("Benching %s:%s\n", current->suite, current->name);
                run_bench(current, results);
                break;
        }
        printf("\n");
        ++current;
    }
    printf("\n========= TEST REPORT =========\n\n");
    printf("[REPORT] total [%lu] | passed ["klGreen"%lu"klNormal"] | failed ["klRed"%lu"klNormal"] | crashed ["klRed"%lu"klNormal"]\n", results[kl_TOTAL], results[kl_GOOD], results[kl_FAILED], results[kl_CRASHED]);
    printf("[REPORT] Took [%zu] milliseconds\n", kl_monotonic() - start);
    return results[kl_TOTAL] - results[kl_GOOD];
}
