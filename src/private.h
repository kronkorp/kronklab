#ifndef __kl_PRIVATE_H
    #define __kl_PRIVATE_H
    #include <stdint.h>
    #include "kronklab/kronklab.h"
    #define kl_TOTAL 0
    #define kl_GOOD 1
    #define kl_FAILED 2
    #define kl_CRASHED 3 

#define kl_CHILD_FLAG "--kl-run"

#ifdef _WIN32
    /* No __start_/__stop_ on PE: sentinels bracket the ".kl$m" entries (linker sorts by '$' suffix). */
    extern const struct __kl_funcStruct __kl_sentinel_begin, __kl_sentinel_end;
    #define KL_BEGIN ((struct __kl_funcStruct *)&__kl_sentinel_begin + 1)
    #define KL_END ((struct __kl_funcStruct *)&__kl_sentinel_end)
#else
    #define KL_BEGIN (&__start___kl_funcStruct)
    #define KL_END (&__stop___kl_funcStruct)
#endif

/* Runs f in a child process. Returns its exit code (-1 if spawn failed); *sig = fatal signal or 0. */
int kl_run_child(struct __kl_funcStruct *f, int *sig);
void run_test(struct __kl_funcStruct *current, unsigned long *results);
void run_bench(struct __kl_funcStruct *current, unsigned long *results);

#endif /* __kl_PRIVATE_H */
