#include "private.h"
#include <kronklab/kronklab.h>
#include <signal.h>
#include <stdio.h>

void run_bench(
    struct __kl_funcStruct *current,
    unsigned long *results
)
{
    timestamp now = kl_high_monotonic();
    int sig = 0;
    int code = kl_run_child(current, &sig);

    if (code == -1) {
        klError("Failed to run test %s:%s\n", current->suite, current->name);
        ++results[2];
        return;
    } else {
        if (sig) {
            int sign = sig;
            if (sign == SIGSEGV) {
                klError("Segmentation fault while running %s:%s\n", current->suite, current->name);
            } 
            else if (sign == SIGABRT) {
                klError("Bench %s:%s aborted (Standard assert failed or abort() called)\n", current->suite, current->name);
            } 
            else if (sign == SIGFPE) {
                klError("Bench %s:%s: Floating point exception (Division by zero)\n", current->suite, current->name);
            } 
            else {
                klError("Bench %s:%s killed by signal %d\n", current->suite, current->name, sign);
            }
        } else {
            if (code == 0) {
                klGood("Successfully benched %s:%s. Runned in %zu us.\n", current->suite, current->name, kl_high_monotonic() - now);
            } else {
                klError("Bench %s:%s failed.\n", current->suite, current->name);
            }
        }
    }
}
